<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Modal form'),
  'description' => t('Open the field\'s part of the node form in a pop up.'),
  'js' => 'slickgrid.editors.js',
  'css' => 'slickgrid.editors.css',
  'handler' => array(
    'class' => 'slickgrid_editors'
  ),
  // As soon as there's an error, stop processing
  'break_on_errors' => true,
  // Processing functions
  'update' => 'ctools_modal_form_wrapper',
  'preprocess' => 'slickgrid_editor_modal_form_preprocess',
  'postprocess' => 'slickgrid_editor_modal_form_postprocess'
); // No specified field_types, so this plugin can be used for all fields

function slickgrid_editor_modal_form_preprocess($editor, &$form_state){
  ctools_include('modal');
  ctools_include('ajax');
  $form_state = $_POST;
  // Additional form_state settings required for ctools modal forms
  $form_state['ajax'] = true;
  $form_state['title'] = t('Update');
}

function slickgrid_editor_modal_form_postprocess($editor, $form_state){
  if(!empty($form_state['executed'])){ // Form has succesfully completed
    $editor->output = array();
    // Reload the data from the view
    // Command to update the grid with the new values
    $response = array(
      'errors' => $editor->errors,
      'updated' => $editor->updated,
      'field_name' => $editor->field_name,
      'op' => 'update'
    );
    slickgrid_callback_get_messages($response);
    $editor->output[] = array(
      'command' => 'modal_slickgrid',
      'response' => $response
    );
    // Issue command to close the dialog
    $editor->output[] = ctools_modal_command_dismiss();
  }
  print ajax_render($editor->output);
  exit();
}