<?php
/**
 * @file
 * Provides an inline cell editor
 */
// Plugin definition
$plugin = array(
  'title' => t('Inline cell'),
  'description' => t('A simple inline cell editor.'),
  'js' => 'slickgrid.editors.js',
  'css' => 'slickgrid.editors.css',
  'field_types' => array(
    'text_default',
    'title'
  ),
  'preprocess' => 'slickgrid_editor_inline_cell_preprocess',
  'update' => 'drupal_form_submit',
  'handler' => array(
    'class' => 'slickgrid_editors'
  )
);

function slickgrid_editor_inline_cell_preprocess($editor, &$form_state){
  $_POST['op'] = 'Save';
  $entity = current($editor->entities);
  $field_name = $editor->field_name;
  if(property_exists($entity, $editor->field_name) && is_array($entity->$field_name)){
    slickgrid_inline_cell_restructure_post($_POST[$field_name], $entity->$field_name);
  }
  $form_state = $_POST;
  $form_state['values'] = $_POST;
}

/**
 * 
 * Function to restructure the $_POST array to match the node structure
 * @param array $values
 * @param array $parents
 */
function slickgrid_inline_cell_restructure_post(&$values, $parents){
  foreach($parents as $key => $parent){
    if(is_array($parent)){
      $values[$key] = array();
      slickgrid_inline_cell_restructure_post($values[$key], $parent);
    }else{
      $values['value'] = $_POST['value'];
    }
  }
}