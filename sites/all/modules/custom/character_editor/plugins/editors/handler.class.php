<?php

/**
 * Base class for export UI.
 */
class character_editor_handler extends slickgrid_editors{

  function get_result(){
    // Clear the message queue
    drupal_get_messages();
    // Add our own messages
    if($count_updated = count($this->updated)){
      drupal_set_message(format_plural($count_updated, 'Character was updated.', '@count characters were updated.'));
    }
    if($count_errors = count($this->errors)){
      $message = format_plural($count_errors, 'Update failed: there was an error', 'Update failed: There were @count errors.');
      $message .= theme('item_list', array(
        'items' => $this->errors
      ));
      drupal_set_message($message, 'error');
    }
    // Return array of data to be returned to the grid
    return array(
      'errors' => $this->errors,
      'updated' => $this->updated,
      'field_id' => $this->field_id,
      'op' => 'update'
    );
  }
}

/**
 * character_metadata_editor_handler
 *
 * Special handler for character metadata
 */
class character_metadata_editor_handler{

  /**
   * __construct
   */
  function __construct($definition){
    $this->errors = array();
    $this->metadata = array();
    if(isset($_POST['flag'])){
      $this->metadata['flag'] = $_POST['flag'];
    }
    if(isset($_POST['send_up'])){
      $this->metadata['sendUp'] = $_POST['send_up'];
    }
    if(isset($_POST['send_down'])){
      $this->metadata['sendDown'] = $_POST['send_down'];
    }
    // Get the character
    if(isset($_POST['column_id']) && preg_match('/^character_(\d+)_(\d+)$/', $_POST['column_id'], $matches)){
      $character_id = $matches[1];
      $relation_id = $matches[2]; // Relation to parent character
      $this->character_w = character_editor_wrapper('character_editor_character', $character_id);
      if(!$this->character_w){
        $this->errors[] = t('Could not load character.');
      }
    }else{
      $this->errors[] = t('No column defined ; could not update character flag');
    }
    // Get the entity
    if(isset($_POST['entity_id'])){
      $this->entity_id = $_POST['entity_id'];
      $this->entity_w = character_editor_wrapper($this->entity_id);
      if(!$this->entity_w){
        $this->errors[] = t('Could not get entity');
      }
    }else{
      $this->errors[] = t('Missing entitiy');
    }
  }

  /**
   * update
   */
  function update(){
    if(empty($this->errors)){
      $r = character_editor_set_character_value($this->character_w, $this->entity_w, NULL, $this->metadata);
      if(!$r){
        $this->errors[] = "Could not update or save the value";
      }
    }
    return $this->get_result();
  }

  /**
   * get_result
   */
  function get_result(){
    // Clear the message queue
    drupal_get_messages();
    $count_errors = count($this->errors);
    if($count_errors){
      $message = format_plural($count_errors, 'Update failed: there was an error', 'Update failed: There were @count errors.');
      $message .= theme('item_list', array(
        'items' => $this->errors
      ));
      drupal_set_message($message, 'error');
    }
    // Return array of data to be returned to the grid
    return array(
      'errors' => $this->errors,
      'updated' => $count_errors == 0 ? array(
        $this->entity_id
      ) : array(),
      'field_id' => $this->column_id,
      'op' => 'update'
    );
  }
}