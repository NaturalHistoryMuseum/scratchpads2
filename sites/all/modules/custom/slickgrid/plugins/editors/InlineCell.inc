<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Inline cell'),
  'description' => t('A simple inline cell editor.'),
  'js' => array(
    'file' => 'slickgrid.editors.js'
  ),
  'css' => array(
    'file' => 'slickgrid.editors.css'
  ),
  'field_types' => array(
    'text_default',
    'number_integer'
  ),
  'process' => 'slickgrid_plugin_inline_cell_process',
  'error' => 'slickgrid_plugin_inline_cell_error',
  'handler' => array(
    'class' => 'slickgrid_editors'
  ),
  'form_id' => 'slickgrid_editor_form'
);

/**
 * 
 * Process the inline cell update callback
 * @param object $editor
 */
function slickgrid_plugin_inline_cell_process($editor){
  $_POST['op'] = t('Save');
  $entity = current($editor->entities);
  $langcode = field_language($editor->entity_type, $entity, $editor->field_id);
  $_POST[$editor->field_id][$langcode] = array(
    array(
      'value' => $_POST['value']
    )
  );
  $form_state['programmed'] = true;
  // Additional form_state settings required for ctools modal forms
  $editor->entity_info = entity_get_info($editor->entity_type);
  $form_state['values'] = $_POST;
  $form_state['editor'] = $editor;
  // Form isn't being displayed - so no rendering or redirecting
  $form_state['re_render'] = true;
  $form_state['no_redirect'] = true;
  // Build & submit the form
  drupal_build_form($editor->plugin['form_id'], $form_state);
}