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
  'field_types' => array_keys(field_info_formatter_types()), // Works for all fields
  'process' => 'slickgrid_plugin_modal_form_process',
  'error' => 'slickgrid_plugin_modal_form_error',
  'form_id' => 'slickgrid_editor_form'
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
  $form_state['values'] = $_POST;
  // Additional form_state settings required for ctools modal forms
  $editor->entity_info = entity_get_info($editor->entity_type);
  $form_state['title'] = format_plural(count($editor->entities), 'Update 1 %type', 'Update @count %types', array(
    '%type' => strtolower($editor->entity_info['label'])
  ));
  $form_state['ajax'] = true;
  // Pass the editor object into the form_state
  $form_state['editor'] = $editor;
  $output = ctools_modal_form_wrapper($editor->plugin['form_id'], $form_state);
  if(!empty($form_state['executed'])){ // Form has succesfully completed  	
    // We're going to exit the process, so get editor result manually
    $result = $form_state['editor']->get_result();
    slickgrid_callback_add_messages($result);
    // Build a ctools ajax output array
    $output = array(
      array(
        'command' => 'slickgrid',
        'response' => array(
          'result' => $result
        )
      )
    );
  }
  print ajax_render($output);
  exit();
}

function slickgrid_plugin_modal_form_error($id, $error, $op){
  // If we're not in the submission process print error & exit
  if($op != 'submit'){
    print ctools_modal_render(t('Error'), $error);
    exit();
  }
}