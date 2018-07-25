<?php

/**
 * Base class for slickgrid editor.
 */
class slickgrid_editors{

  var $plugin;

  var $field_name;
  // Entity info
  var $entity_type;

  var $entities = array();
  // Variables for storing processing data
  var $errors = array();

  var $updated = array();

  var $output;
  // View data
  var $field_id;

  var $view_name;

  var $display_id;
  // Function for handling errors
  var $error_callback;

  function __construct($plugin){
    $this->plugin = $plugin;
    // Entity variables
    $this->entity_type = $_POST['entity_type'];
    // Load all the entities    
    $this->entities = entity_load($this->entity_type, $_POST['entity_ids']);
    // Field variables
    $this->field_name = $_POST['field_name'];
    // Views variables
    $this->field_id = $_POST['field_id'];
    $this->view = $_POST['view'];
    $this->display_id = $_POST['display_id'];
    // Register the error handler if one exists
    if(isset($this->plugin['error']) && function_exists($this->plugin['error'])){
      $this->error_callback = $this->plugin['error'];
    }
  }

  /**
   * 
   * Do the actual update - passes the update to the plugin's process functions
   */
  function update(){
    if(function_exists($this->plugin['process'])){
      $this->plugin['process']($this);
    }
    // Are there any form errors?
    if($errors = form_get_errors()){
      foreach($errors as $error){
        // Form errors will apply for all entities
        foreach($this->entities as $entity){
          list($id) = entity_extract_ids($this->entity_type, $entity);
          $this->set_error($id, $error);
        }
      }
    }
    return $this->get_result();
  }

  /**
   * 
   * Log an error to the editor
   * @param entity id $id
   * @param strign - error $error
   * @param where in the process the error is - either submit or validate $op
   */
  function set_error($id, $error, $op = 'validate'){
    // Register the error
    $this->errors[$id] = t('%title can not be edited: <strong>@error</strong>', array(
      '%title' => $this->entities[$id]->title,
      '@error' => $error
    ));
    // Remove the entity so it won't be updated
    unset($this->entities[$id]);
    // If there's an error callback, call it
    if(isset($this->error_callback)){
      call_user_func_array($this->error_callback, array(
        $id,
        $this->errors[$id],
        $op
      ));
    }
  }
  // Set an update item
  function set_updated($id, $vid){
    $this->updated[$id] = array(
      'vid' => $vid
    );
  }

  function get_result(){
    // If items have been updated, reload the values from the view
    if(count($this->updated)){
      $view = slickgrid_get_view($this->view, $this->display_id, array_keys($this->updated));
      foreach($view->result as $count => $row){
        if(isset($this->updated[$row->{$view->base_field}])){
          $this->updated[$row->{$view->base_field}]['value'] = $view->render_field($this->field_id, $count);
        }
      }
    }
    // Clear the message queue
    drupal_get_messages();
    // Add our own messages
    if($count_updated = count($this->updated)){
      drupal_set_message(format_plural($count_updated, 'One item was updated succesfully.', '@count items were updated succesfully.'));
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