<?php
namespace ScratchpadsVocabularies;
require_once str_replace('remap.modification.inc', 'classes.inc', __FILE__);

/**
 * RemapModification
 * 
 * This class implements ModificationInterface to provide a modification
 * that requires the user to remap old values to new ones.
 */
class RemapModification implements ModificationInterface{

  const BATCH_SIZE = 10;

  private $explanation;

  private $receiver;

  private $update;

  /**
   * __construct
   * 
   * Create a new RemapModification
   * 
   * $receiver: The receiver this modification applies to,
   * $update: The update this modification is for
   * $explanation: The human readable explanation
   */
  public function __construct($receiver, $update, $explanation){
    $this->explanation = $explanation;
    $this->receiver = $receiver;
    $this->update = $update;
  }

  /**
   * explanation
   *
   * Returns a human readable string that explains what
   * modification is required.
   */
  public function explanation(){
    return $this->explanation;
  }

  /**
   * form
   *
   * Returns a form array that is used by the user
   * to perform any required actions before the update
   * can be applied.
   * 
   * Expects $id to be an array defining 'vocabulary_id' and
   * 'update_id'.
  */
  public function form($id, $form, &$form_state){
    $new_items = $this->receiver->structure()->selectStructure();
    $action_table = array();
    $even = FALSE;
    $elem_id = 0;
    foreach($this->update->items() as $item_id => $item_label){
      $elem_id++;
      $affected_list = $this->receiver->contentModifiedBy($this->update, $item_id);
      $affected = array();
      foreach($affected_list as $type => $affected_items){
        $affected[] = check_plain($type) . ': ' . count($affected_items);
      }
      $action_table[] = array(
        check_plain("$item_label"),
        array(
          "map_" . $elem_id => array(
            '#type' => 'select',
            '#multiple' => FALSE,
            '#options' => $new_items,
            '#default_value' => reset(array_keys($new_items)),
            '#vocabulary_id' => $id['vocabulary_id'],
            '#update_id' => $id['update_id'],
            '#elem_id' => $item_id
          ),
          "remove_" . $elem_id => array(
            '#type' => 'checkbox',
            '#title' => t('Remove value when field is not required'),
            '#vocabulary_id' => $id['vocabulary_id'],
            '#update_id' => $id['update_id'],
            '#elem_id' => $item_id
          )
        ),
        implode('<br/>', $affected)
      );
    }
    return _scratchpads_vocabularies_form_table(array(
      t('Old value'),
      t('New value'),
      t('Number of affected entries')
    ), $action_table);
  }

  /**
   * validate
   *
   * Validate the submited form
   * 
   * Expects $id to be an array defining 'vocabulary_id' and
   * 'update_id'.
  */
  public function validate($id, $form, &$form_state){
    return TRUE;
  }

  /**
   * submit
   *
   * submit the form, and return a list of operations that can be
   * used for batches.
   * 
   * Expects $id to be an array defining 'vocabulary_id' and
   * 'update_id'.
   * 
  */
  public function submit($id, $form, &$form_state){
    $map = $this->mapFromForm($form, $form_state, $id['vocabulary_id'], $id['update_id']);
    // Create a batch operation for every n entities
    $operations = array();
    $entities = $this->receiver->contentModifiedBy($this->update);
    foreach($entities as $type => $entity_ids){
      $type_chunks = array_chunk($entity_ids, RemapModification::BATCH_SIZE);
      foreach($type_chunks as $type_chunk){
        $operations[] = array(
          '_scratchpads_vocabularies_remap',
          array(
            $id['vocabulary_id'],
            $id['update_id'],
            $type,
            $type_chunk,
            $map
          )
        );
      }
    }
    return $operations;
  }

  /**
   * mapFromForm
   *
   * This is a helper method to retrive our map of values from the submitted form.
   * We can't use our item ids for the element names, as Drupal will replace spaces
   * by underscores which may generate collisions (we cannot make any assumptions
   * since those values are provided externally).
   *
   * We can't directly look into the form for corresponding items, because we are
   * embeded in a larger form that we do not know.
   *
   * So we need to scan the form to gather our elements back.
   */
  public function mapFromForm($form, $form_state, $vocabulary_id, $update_id){
    $map = array();
    foreach($form as $key => $value){
      if(is_array($value) && isset($value['#vocabulary_id'])){
        if($value['#vocabulary_id'] == $vocabulary_id && $value['#update_id'] == $update_id){
          if(preg_match('/^map_/', $key)){
            $map[$value['#elem_id']]['value'] = $form_state['values'][$key];
          }else if(preg_match('/^remove_/', $key)){
            $map[$value['#elem_id']]['only_if_required'] = !empty($form_state['values'][$key]);
          }
        }
      }else if(is_array($value)){
        $map = array_merge($map, $this->mapFromForm($value, $form_state, $vocabulary_id, $update_id));
      }
    }
    return $map;
  }
}
