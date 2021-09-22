<?php
// Get the last edit time of the original file
$filename = basename(__FILE__);
$original_filename = DRUPAL_ROOT . '/modules/field/' . $filename;
$replacement_filename = __FILE__ . '.original.inc';
if(filemtime($original_filename) > filemtime($replacement_filename)){
  // The file has been changed, or we have just installed this module.  Lets
  // get the content of taxonomy.module, tweak it, and save it to a file
  drupal_set_message(t('Reloading original field.attach.inc'));
  // Contents of the file.
  $field_attach_string = file_get_contents($original_filename);
  // Replace the name of the offending functions, appending "_original" onto
  // them (this allows them to be used if still required).
  $functions_to_replace = array(
    'field_attach_delete'
  );
  foreach($functions_to_replace as $function){
    $field_attach_string = str_replace('function ' . $function . '(', 'function ' . $function . '_original(', $field_attach_string);
  }
  // Save the content of taxonomy.module for easy loading
  file_put_contents($replacement_filename, $field_attach_string);
}
// Load the new file
include $replacement_filename;
// Load include file with the replacement functions in
module_load_include('attach.inc.replace.inc', 'field');

/**
class FieldValidationException extends FieldException {
 */