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
  'handler' => array(
    'class' => 'slickgrid_editors'
  )
);

/**
 * 
 * Process the inline cell update callback
 * @param object $editor
 */
function slickgrid_plugin_inline_cell_process($editor){
  foreach($editor->entities as $entity){
    // Get the entity ids
    list($id, $vid, $bundle_name) = entity_extract_ids($editor->entity_type, $entity);
    // Create an instance of the field
    if(!$instance = field_info_instance($editor->entity_type, $editor->field_id, $bundle_name)){
      $editor->errors[$id] = t('Field does not exist for this bundle');
      continue;
    }
    // Is the field required?
    if($instance['required'] && empty($_POST['value'])){
      $editor->errors[$id] = t('@field_label field is required.', array(
        '@field' => $instance['label']
      ));
      continue;
    }
    // Inline editor can only be used for flat values, so populating the values like this works fine
    if($langcode = field_language($editor->entity_type, $entity, $editor->field_id)){
      $entity->{$editor->field_id} = array(
        $langcode => array(
          array(
            'value' => $_POST['value']
          )
        )
      );
    }
    // Perform field_level validation.
    try{
      field_attach_validate($editor->entity_type, $entity);
    }
    catch(FieldValidationException $e){
      // Record the errors
      foreach($e->errors as $field_name => $field_errors){
        foreach($field_errors as $langcode => $errors){
          $editor->errors[$id] = $errors[0][0]['message'];
        }
      }
    }
    // If there hasn't been any errors, save it
    if(!$editor->errors[$id]){
      // Save the entity
      entity_save($editor->entity_type, $entity);
      $editor->updated[$id] = array(
        'vid' => $vid
      );
    }
  }
}

