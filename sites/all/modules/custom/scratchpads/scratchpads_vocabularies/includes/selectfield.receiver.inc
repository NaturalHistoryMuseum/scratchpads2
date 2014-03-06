<?php
namespace ScratchpadsVocabularies;
use Exception;
use EntityFieldQuery;
require_once str_replace('selectfield.receiver.inc', 'receiver.inc', __FILE__);
require_once str_replace('selectfield.receiver.inc', 'remap.modification.inc', __FILE__);

/**
 * SelectFieldReceiver
 * 
 * Implements a VocabularyReceiver which stores the vocabulary
 * as the allowed values of a select field.
 * 
 * The current implementation does *not* use API for updating
 * fields and content, as those are very expensive operations.
 * 
 * We know this to be safe in the context this was written -
 * but might need updating in the future. In the mean time
 * it's important that caches are cleared when the update
 * operations are finished.
 * 
 */
class SelectFieldReceiver extends VocabularyReceiver{

  private $field;

  private $views = NULL;

  /**
   * __construct
   * 
   * Create a new instance from a field name or array structure
   */
  public function __construct($field){
    // Load the field data and perform safety check
    if(is_array($field)){
      $this->field = $field;
    }else{
      $this->field = field_info_field($field);
      if(!$this->field){throw new Exception(t("Unknown field %name", array(
          '%name' => $field
        )));}
    }
    if(!is_array($this->field) || empty($this->field['type']) || $this->field['type'] != 'list_text' || !isset($this->field['settings']['allowed_values']) || !is_array($this->field['settings']['allowed_values'])){throw new Exception(t("Could not parse field information"));}
    // Prepare the object and load the persistent data
    parent::__construct($this->field['field_name']);
  }

  /**
   * __wakeup
   *
   * Re-populate $field
   */
  public function __wakeup(){
    parent::__wakeup();
    $this->field = field_info_field($this->id());
  }

  /**
   * description
   */
  public function description(){
    return t("Saves the vocabulary to be used by field %field", array(
      '%field' => $this->field['field_name']
    ));
  }

  /**
   * structureInUse
   * 
   * Return the structure as it is currently used
   */
  protected function structureInUse(){
    return new VocabularyStructure($this->field['settings']['allowed_values']);
  }

  /**
   * mergeUpdate
   *
   * Merge the given update into this receiver's update list.
   * In some cases, if an update has been structurally applied,
   * then some modification of the field will be required.
   *
   */
  protected function mergeUpdate($update, $new_structure){
    // Maps standard action, for existing update type to new update type.
    // Define what to do with existing update and with new update.
    $actions = array(
      VocabularyUpdate::T_ADD => array(
        VocabularyUpdate::T_REMOVE => 'remove',
        VocabularyUpdate::T_RENAME => 'update',
        VocabularyUpdate::T_REPARENT => 'update'
      ),
      VocabularyUpdate::T_RENAME => array(
        VocabularyUpdate::T_REMOVE => 'replace-old',
        VocabularyUpdate::T_RENAME => 'update',
        VocabularyUpdate::T_REPARENT => 'replace'
      ),
      VocabularyUpdate::T_REPARENT => array(
        VocabularyUpdate::T_REMOVE => 'replace-old',
        VocabularyUpdate::T_RENAME => 'update',
        VocabularyUpdate::T_REPARENT => 'update'
      )
    );
    // Loop over update items
    $new_items = $update->items();
    $new_updates = array();
    $old_structure = $this->structure()->flatStructure();
    foreach($this->pendingUpdates as $existing_update){
      foreach($existing_update->items() as $id => $label){
        if(!isset($new_items[$id])){
          continue;
        }
        if(isset($actions[$existing_update->type()][$update->type()])){
          // Generic action
          switch($actions[$existing_update->type()][$update->type()]){
            case 'remove':
              $existing_update->removeItem($id);
              break;
            case 'update':
              $existing_update->updateItem($id, $new_items[$id]);
              break;
            case 'replace':
              $existing_update->removeItem($id);
              $new_updates[$update->type()][$id] = $new_items[$id];
              break;
            case 'replace-old':
              $existing_update->removeItem($id);
              $new_updates[$update->type()][$id] = $old_structure[$id];
              break;
          }
        }else if($existing_update->type() == VocabularyUpdate::T_REMOVE && $update->type() == VocabularyUpdate::T_ADD){
          // Special case
          $existing_update->removeItem($id);
          if($existing_update->status() == VocabularyUpdate::S_STRUCTURE_APPLIED){
            // Remove the 'DELETED' from the field definition.      
            $this->field['settings']['allowed_values'][$id] = preg_replace('/ ?\[DELETED\]/', '', $this->field['settings']['allowed_values'][$id]);
            $this->cheapUpdateField();
          }
          // Check if we should have a rename/reparent update
          $existing_parents = array_keys($this->structure()->itemParents($id));
          $new_parents = array_keys($new_structure->itemParents($id));
          if($existing_parents != $new_parents){
            $new_updates[VocabularyUpdate::T_REPARENT][$id] = $new_items[$id];
          }else if($label != $new_items[$id]){
            $new_updates[VocabularyUpdate::T_RENAME][$id] = $new_items[$id];
          }
        }else{
          throw new Exception("Update cannot be merge, inconsistent update");
        }
      }
    }
    // Add the new updates
    foreach($new_updates as $type => $items){
      $this->pendingUpdates[] = new VocabularyUpdate($type, $items);
    }
  }

  /**
   * canUpgradeStructure
   *
   * Return FALSE if any view uses values that would get removed
   */
  public function canUpgradeStructure(VocabularyInterface $vocabulary){
    $current = $this->structure();
    $updated = $vocabulary->structure();
    $updates = $current->updatesToUpgrade($updated);
    foreach($updates as $update){
      if($update->type() == VocabularyUpdate::T_REMOVE){
        if($this->viewUsesValue($update)){return FALSE;}
      }
    }
    return TRUE;
  }

  /**
   * applyUpdate
   *
   * Apply the structural part of the given update.
   * 
   * XXX Problem with current approach is that things are not
   * kept in order. If something is added between v1 and v2,
   * the approach of incremental updates as applied here
   * inserts it last, not between v1 and v2.
   * 
   * I should just re-generate it all.
   *
   */
  protected function applyUpdate($update){
    if($update->type() == VocabularyUpdate::T_DATA){
      $update->setStatus(VocabularyUpdate::S_UPDATE_APPLIED);
      return;
    }
    $items = $update->items();
    $current = $this->field['settings']['allowed_values'];
    foreach($items as $id => $label){
      if($update->type() == VocabularyUpdate::T_REMOVE){
        // See if this value is actually used anywhere. If not, we can just delete it
        // without worry.
        if($this->contentModifiedBy($update, $id)){
          $this->field['settings']['allowed_values'][$id] .= ' [DELETED]';
        }else{
          unset($this->field['settings']['allowed_values'][$id]);
          $update->removeItem($id);
        }
      }else{
        $parents = $this->structure()->itemParents($id);
        if($parents){
          $label = implode('>', $parents) . ' > ' . $label;
        }
        $this->field['settings']['allowed_values'][$id] = $label;
      }
    }
    $this->cheapUpdateField();
    $left_items = $update->items();
    if($update->type() == VocabularyUpdate::T_REMOVE && !empty($left_items)){
      $update->setStatus(VocabularyUpdate::S_STRUCTURE_APPLIED);
      $update->setModification(new RemapModification($this, $update, t("Some terms could not be deleted because they are in use. Please re-map the deleted terms to a new value.")));
    }else{
      // add/rename/reparent don't need a content update, as the key is stored
      // in the field data, not the label.
      $update->setStatus(VocabularyUpdate::S_UPDATE_APPLIED);
    }
  }

  /**
   * applyModification
   * 
   * Apply the content modification part of a single update.
   * This onyl gets called for REMOVE updates, as other updates
   * are marked as completed at the structural stage.
   * 
   */
  protected function applyModification($limit_entities, $update, $map){
    // First remove all the values we don't want from allowed values
    $items = $update->items();
    $new_allowed_values = $this->field['settings']['allowed_values'];
    foreach($items as $id => $label){
      // Remove the option from the field definition
      unset($new_allowed_values[$id]);
    }
    // Find the global default value for this field
    if(count($new_allowed_values)){
      $allowed_keys = array_keys($new_allowed_values);
      $default = reset($allowed_keys);
    }else{
      $default = '';
    }
    foreach($items as $id => $label){
      $result = $this->contentModifiedBy($update, $id);
      foreach($result as $type => $values){
        if(!empty($limit_entities)){
          if(empty($limit_entities[$type])){
            continue;
          }
          $values = array_intersect($values, $limit_entities[$type]);
        }
        $entities = entity_load($type, $values);
        foreach($entities as $entity){
          $instance = field_info_instance($type, $this->field['field_name'], $entity->type);
          foreach($entity->{$this->field['field_name']} as $langcode => $values){
            foreach($values as $value_id => $value){
              if($value['value'] == $id){
                if(!$instance['required'] && (empty($map) || !empty($map[$id]['only_if_required']))){
                  unset($entity->{$this->field['field_name']}[$langcode][$value_id]);
                }else if(isset($map[$id]['value'])){
                  $entity->{$this->field['field_name']}[$langcode][$value_id]['value'] = $map[$id]['value'];
                }else if($instance['default_value'] !== NULL){
                  $entity->{$this->field['field_name']}[$langcode][$value_id]['value'] = $instance['default_value'];
                }else{
                  $entity->{$this->field['field_name']}[$langcode][$value_id]['value'] = $default;
                }
              }
            }
          }
          entity_save($type, $entity);
        }
      }
    }
    if(!$this->updateModifiesContent($update)){
      // If there is no more modifications, then we can remove the item from the allowed values,
      // and mark the update as completed.
      $this->field['settings']['allowed_values'] = $new_allowed_values;
      $this->cheapUpdateField();
      $update->setStatus(VocabularyUpdate::S_UPDATE_APPLIED);
    }
  }

  /**
   * updateModifiesContent
   * 
   * Return TRUE if the given update needs to modify content to
   * be fully applied, and FALSE otherwise
   * 
   */
  public function updateModifiesContent($update){
    if($update->type() !== VocabularyUpdate::T_REMOVE){return FALSE;}
    foreach($update->items() as $id => $label){
      $efq = new EntityFieldQuery();
      $result = $efq->entityCondition('bundle', $this->field['bundles'])->fieldCondition($this->field['field_name'], 'value', $id, 'COLLATE utf8_bin =')->execute();
      if(!empty($result)){return TRUE;}
    }
    return FALSE;
  }

  /**
   * contentModifiedBy
   * 
   * Return the content modified by applying the given update. If
   * $id is not NULL, then only return the content that is modified
   * when applying the given update for that particular item.
   * 
   * The returned array should be entity ids indexed by entity type.
   */
  public function contentModifiedBy($update, $id = NULL){
    if($update->type() !== VocabularyUpdate::T_REMOVE){return array();}
    if($id !== NULL){
      $item_ids = array(
        $id
      );
    }else{
      $item_ids = array_keys($update->items());
    }
    $result = array();
    foreach($item_ids as $item_id){
      $efq = new EntityFieldQuery();
      $r = $efq->entityCondition('bundle', $this->field['bundles'])->fieldCondition($this->field['field_name'], 'value', $item_id, 'COLLATE utf8_bin =')->execute();
      foreach($r as $type => $entity_ids){
        if(!isset($result[$type])){
          $result[$type] = array();
        }
        $result[$type] = array_unique(array_merge($result[$type], array_keys($entity_ids)));
      }
    }
    return $result;
  }

  /**
   * cheapUpdateField
   * 
   * Calling field_update_field is very expensive. In our specific context,
   * we know it's safe to change the field without calling the callbacks.
   * 
   */
  private function cheapUpdateField(){
    $data = $this->field;
    unset($data['columns'], $data['field_name'], $data['type'], $data['locked'], $data['module'], $data['cardinality'], $data['active'], $data['deleted']);
    unset($data['bundles']);
    $this->field['data'] = $data;
    $primary_key = array(
      'id'
    );
    drupal_write_record('field_config', $this->field, $primary_key);
  }

  /**
   * viewUsesValue
   *
   * Return TRUE if there are existing views that use any of the values in the given update
   */
  private function viewUsesValue($update){
    $vfname = $this->field['field_name'] . '_value';
    if($this->views === NULL){
      $this->views = array();
      $all_views = views_get_all_views();
      foreach($all_views as $view){
        foreach($view->display as $display_name => $display){
          if(isset($display->display_options['filters'][$vfname]['value'])){
            $this->views[$view->name]['view'] = $view;
            $this->views[$view->name]['displays'][] = $display_name;
          }
        }
      }
    }
    // Loop over the views
    foreach($this->views as $name => $info){
      $view = $info['view'];
      $save = FALSE;
      foreach($info['displays'] as $display){
        foreach($update->items() as $id => $label){
          if(isset($view->display[$display]->display_options['filters'][$vfname]['value'][$id])){return TRUE;}
        }
      }
    }
    return FALSE;
  }

  /**
   * resetViewsList
   *
   * Reset the list of views in case it changes between two invocations on this
   * field. This is mostly used for testing.
   */
  public function resetViewsList(){
    $this->views = NULL;
  }
}