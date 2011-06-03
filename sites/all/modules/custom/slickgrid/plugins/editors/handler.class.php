<?php

/**
 * Base class for export UI.
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
  }

  function update(){
    if(function_exists($this->plugin['process'])){
      $this->plugin['process']($this);
    }
    return $this->get_result();
  }

  // Set an error item
  function set_error($id, $err){
    $this->errors[$id] = $err;
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
      drupal_set_message(format_plural($count_errors, 'There was an error.', 'There were @count errors.'), 'error');
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