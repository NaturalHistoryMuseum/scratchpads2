<?php
namespace ScratchpadsVocabularies;
require_once str_replace('remap.modification.inc', 'classes.inc', __FILE__);
require_once str_replace('remap.modification.inc', 'selectfield.receiver.inc', __FILE__);

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
   * getForm
   */
  public function getForm($form, &$form_state){
    $new_items = $this->receiver->structure()->selectStructure();
    $action_table = array();
    $even = FALSE;
    $array_id = 0;
    foreach($this->update->items() as $item_id => $item_label){
      $affected_list = $this->receiver->contentModifiedBy($this->update, $item_id);
      $affected = array();
      foreach($affected_list as $type => $affected_items){
        $affected[] = check_plain($type) . ': ' . count($affected_items);
      }
      $action_table[] = array(
        check_plain("$item_label"),
        array(
          "remap_item_" . $array_id => array(
            '#type' => 'value',
            '#value' => $item_id
          ),
          "map_" . $array_id => array(
            '#type' => 'select',
            '#multiple' => FALSE,
            '#options' => $new_items,
            '#default_value' => reset(array_keys($new_items))
          ),
          "remove_" . $array_id => array(
            '#type' => 'checkbox',
            '#title' => t('Remove value when field is not required')
          )
        ),
        implode('<br/>', $affected)
      );
      $array_id++; // $item_id may have spaces so use this to generate unique array ids.
    }
    $form = array(
      'table' => _scratchpads_vocabularies_form_table(array(
        t('Old value'),
        t('New value'),
        t('Number of affected entries')
      ), $action_table),
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Apply')
      )
    );
    return $form;
  }

  /**
   * validateForm
   */
  public function validateForm($form, &$form_state){}

  /**
   * submitForm
   *
   * submit the form
   * 
   */
  public function submitForm($form, &$form_state){
    $map = array();
    foreach($form_state['values'] as $key => $value){
      if(preg_match('/^remap_item_(\d+)$/', $key, $matches)){
        $array_id = $matches[1];
        $item_id = $value;
        $map[$item_id]['value'] = $form_state['values']['map_' . $array_id];
        $map[$item_id]['only_if_required'] = $form_state['values']['remove_' . $array_id];
      }
    }
    // Create a batch operation for every n entities
    $operations = array();
    $entities = $this->receiver->contentModifiedBy($this->update);
    foreach($entities as $type => $entity_ids){
      $type_chunks = array_chunk($entity_ids, RemapModification::BATCH_SIZE);
      foreach($type_chunks as $type_chunk){
        $operations[] = array(
          'ScratchpadsVocabularies\_scratchpads_vocabularies_remap_batch',
          array(
            $this->receiver,
            $this->update,
            $type,
            $type_chunk,
            $map
          )
        );
      }
    }
    batch_set(array(
      'title' => t('Applying updates. Please do not close this window until the process is complete.'),
      'progress_message' => t('Completed @percentage% of updates, estimated time left @estimate'),
      'operations' => $operations,
      'finished' => 'ScratchpadsVocabularies\_scratchpads_vocabularies_remap_batch_finish',
      'file' => drupal_get_path('module', 'scratchpads_vocabularies') . '/includes/remap.modification.inc'
    ));
  }
}

/**
 * _scratchpads_vocabularies_remap_batch
 *
 * Batch callback for remaping values on entities
 */
function _scratchpads_vocabularies_remap_batch($receiver, $update, $type, $entities, $map){
  $receiver->applyModifications(array(
    $type => $entities
  ), array(
    $update
  ), $map);
}

/**
 * _scratchpads_vocabularies_remap_batch_finish
 */
function _scratchpads_vocabularies_remap_batch_finish(){
  // Changes may impact menus and so on, so do a full cache clear.
  drupal_flush_all_caches();
}