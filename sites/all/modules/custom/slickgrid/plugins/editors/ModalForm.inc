<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Modal form'),
  'description' => t('Open the field\'s part of the node form in a pop up.'),
  'js' => array(
    'file' => 'slickgrid.editors.js'
  ),
  'css' => array(
    'file' => 'slickgrid.editors.css'
  ),
  'handler' => array(
    'class' => 'slickgrid_editors'
  ),
  'process' => 'slickgrid_plugin_modal_form_process'
); // No specified field_types, so this plugin can be used for all fields

/**
 * 
 * Retrieve form / process a modal form
 * This uses ctools ajax & the whole form so it works with all fields
 * @param object $editor
 */
function slickgrid_plugin_modal_form_process($editor){
  // Include the ctools stuff
  ctools_include('modal');
  ctools_include('ajax');
  $entity_info = entity_get_info($editor->entity_type);
  if(isset($entity_info['edit callback']) && function_exists($entity_info['edit callback'])){
    foreach($editor->entities as $entity){
      // Use a new copy of $_POST as $form_state for each loop as it gets altered by submission process
      $form_state = $_POST;
      // Additional form_state settings required for ctools modal forms
      $form_state['title'] = t('Update @label', array(
        '@label' => $entity->{$entity_info['entity keys']['label']}
      ));
      $form_state['ajax'] = true;
      $form_state['status'] = 1;
      list($id, $vid, $bundle_name) = entity_extract_ids($editor->entity_type, $entity);
      if(!$bundle_name){
        slickgrid_plugin_modal_form_error(t('No bundle name for this entity type'));
      }
      $form_id = call_user_func($entity_info['edit callback'], $bundle_name);
      // Pass the entity to the submission process as an argument (eg, for nodes)
      $form_state['build_info']['args'] = array(
        $entity
      );
      $output = ctools_modal_form_wrapper($form_id, $form_state);
      // If not submitted, we are just getting the form so exit the loop
      if(!$form_state['submitted'] || form_get_errors()){
        // We don't care about reporting form errors as it's ajax & the form will just fail validation
        break;
      }else{
        $editor->set_updated($id, $vid);
      }
    } // End of foreach 
    if(!empty($form_state['executed'])){ // Form has succesfully completed
      // We're going to exit the process, so get editor result manually
      $result = $editor->get_result();
      slickgrid_callback_get_messages($result);
      $output = array(
        array(
          'command' => 'slickgrid',
          'response' => array(
            'result' => $result
          )
        )
      );
      // Issue command to close the dialog
      $output[] = ctools_modal_command_dismiss();
    }
    print ajax_render($output);
    exit();
  }else{
    slickgrid_plugin_modal_form_error(t('Edit form not found'));
  }
}

/**
 * 
 * Error handler - render an error in the modal form & exit
 * @param string $err
 */
function slickgrid_plugin_modal_form_error($err){
  print ctools_modal_render(t('Error'), $err);
  exit();
}
