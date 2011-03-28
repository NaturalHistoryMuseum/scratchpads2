<?php

/**
 * Base class for export UI.
 */
class slickgrid_editors{

  var $plugin;

  var $form_id;

  var $field_name;

  // Entity info
  var $entity_type;

  var $entity_ids = array();

  var $entities = array();

  // Keep a copy of $form_state
  var $form_state = array();

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
    $this->entity_ids = $_POST['entity_ids'];
    $this->entities = entity_load($this->entity_type, $this->entity_ids);
    // Field variables
    $this->field_name = $_POST['field_name'];
    // Views variables
    $this->field_id = $_POST['field_id'];
    $this->view_name = $_POST['view'];
    $this->display_id = $_POST['display_id'];
  }

  function update(){
    $form_state = array();
    $this->get_entity_info();
    if(function_exists($this->plugin['preprocess'])){
      $this->plugin['preprocess']($this, $form_state);
    }
    // Form state gets changed in the form processing so make a copy
    $this->form_state = $form_state;
    foreach($this->entities as $entity_id => $entity){
      // Form state gets changed in the form processing so reset to original
      $form_state = $this->form_state;
      $form_state['build_info']['args'] = array(
        $entity
      );
      // Keep a copy of the revision ID prior to being updated so we can use it for the undo 
      $revision_id = $entity->vid;
      $this->output = $this->plugin['update']($this->form_id, $form_state);
      // If not submitted, we are just getting the form so exit the loop
      if(!$form_state['submitted']){
        break;
      }elseif($err = form_get_errors()){
        if($this->plugin['break_on_errors']){
          break;
        }else{
          $this->errors[$entity_id] = $err;
        }
      }else{
        $this->updated[$entity_id] = array(
          'vid' => $revision_id
        );
      }
    } // end of foreach $entities
    if(count($this->updated)){
      $view = slickgrid_callback_get_view($form_state['view'], $form_state['display_id'], array_keys($data->updated));
      foreach($view->result as $count => $row){
        // TODO - this is using NID
        // TODO - this is getting all values, arg not being honoured
        if(isset($this->updated[$row->nid])){
          $this->updated[$row->nid]['value'] = $view->render_field($form_state['field_id'], $count);
        }
      }
    }
    // Clear the message queue
    drupal_get_messages();
    // Add our own messages
    if($count_updated = count($this->updated)){
      drupal_set_message(format_plural($count_updated, '@title was updated succesfully.', '@count items were updated succesfully.', array(
        '@title' => $entity->title
      )));
    }
    if($count_errors = count($this->errors)){
      drupal_set_message(format_plural($count_errors, 'There was 1 error.', 'There were @count errors.'), 'error');
    }
    if(function_exists($this->plugin['postprocess'])){
      $this->plugin['postprocess']($this, $form_state);
    }
    return array(
      'errors' => $this->errors,
      'updated' => $this->updated,
      'field_name' => $this->field_name
    );
  }

  function get_entity_info(){
    $entity_info = entity_get_info($this->entity_type);
    $entity = current($this->entities);
    if(isset($entity_info['bundles'][$entity->type]['edit'])){
      if(is_array($entity_info['bundles'][$entity->type]['edit']['include'])){
        module_load_include('inc', $entity_info['bundles'][$entity->type]['edit']['include']['module'], $entity_info['bundles'][$entity->type]['edit']['include']['file']);
      }
      $this->form_id = $entity_info['bundles'][$entity->type]['edit']['form_id'];
      return;
    }
    drupal_set_message(t('Sorry, no form information could be found for this entity type. Please contact your site administrator.'), 'error');
  }

  /**
   * 
   * Get a views filtered by NIDs 
   * @param string $view_name
   * @param string $display_id
   * @param array $nids
   */
  function get_view(){
    $view = views_get_view($this->view_name);
    $view->set_display($this->display_id);
    // Remove all existing arguments - we'll limit result set by NID only
    foreach($view->get_items('argument') as $id => $arg){
      $view->set_item($this->display_id, 'argument', $id, NULL);
    }
    // Add an argument to limit the view to only nids being updated	
    $options = array(
      'table' => 'node',
      'field' => nid,
      'break_phrase' => 1, // Allow multiple nids,
      'validate_argument_nid_type' => 'nid'
    );
    $view->add_item($this->display_id, 'argument', 'node', 'nid', $options);
    $view->set_arguments(array(
      implode('+', array_keys($this->updated))
    ));
    $view->pre_execute();
    $view->execute();
    $view->render();
    return $view;
  }
}