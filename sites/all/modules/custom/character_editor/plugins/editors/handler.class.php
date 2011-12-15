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