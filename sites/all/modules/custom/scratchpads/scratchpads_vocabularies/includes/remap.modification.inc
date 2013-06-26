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

  const BATCH_SIZE = 5;

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
    foreach($this->update->items() as $item_id => $item_label){
      $affected_list = $this->receiver->contentModifiedBy($this->update, $item_id);
      $affected = array();
      foreach($affected_list as $type => $affected_items){
        $affected[] = check_plain($type) . ': ' . count($affected_items);
      }
      $action_table[] = array(
        check_plain("$item_label"),
        array(
          "update_" . implode('_', $id) . "_remap_" . $item_id => array(
            '#type' => 'select',
            '#multiple' => FALSE,
            '#options' => $new_items
          ),
          "update_" . implode('_', $id) . "_remove_" . $item_id => array(
            '#type' => 'checkbox',
            '#title' => t('Remove value when field is not required')
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
    // Create the map of values to replace
    $map = array();
    foreach($this->update->items() as $item_id => $value){
      $map[$item_id] = array(
        'value' => $form_state['values']["update_" . implode('_', $id) . "_remap_" . $item_id],
        'only_if_required' => !empty($form_state['values']['update_' . implode('_', $id) . '_remove_' . $item_id])
      );
    }
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
}
