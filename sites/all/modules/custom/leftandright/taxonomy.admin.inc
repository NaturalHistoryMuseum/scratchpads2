<?php
// Get the last edit time of the original file
$filename = basename(__FILE__);
$last_modified = filemtime(DRUPAL_ROOT . '/modules/taxonomy/' . $filename);
if($last_modified > variable_get('leftandright_'.$filename.'_last_modified', 0)){
  // Set the last modified time
  variable_set('leftandright_'.$filename.'_last_modified', $last_modified);
  // The file has been changed, or we have just installed this module.  Lets
  // get the content of taxonomy.module, tweak it, and save it to a variable
  drupal_set_message(t('Reloading original Drupal core taxonomy'));
  $lines = preg_split("/[\n\r]+/", file_get_contents(DRUPAL_ROOT . '/modules/taxonomy/' . $filename));
  // Pop the first line which only contains the start php tag "< ? p h p"
  array_shift($lines);
  $within_function = FALSE;
  // Look for taxonomy_get_tree and taxonomy_overview_terms, so that we can
  // remove them.
  foreach($lines as $key => $line){
    if(substr($line, 0, 33) == 'function taxonomy_overview_terms(' || substr($line, 0, 39) == 'function theme_taxonomy_overview_terms('){
      $within_function = TRUE;
    }    
    if($within_function){
      unset($lines[$key]);
      if(substr($line, 0, 1) == '}'){
        $within_function = FALSE;
      }
    }
  }
  // Save the content of taxonomy.module with our own taxonomy_get_tree (above)
  variable_set('leftandright_'.$filename.'_replacement', implode("\n", $lines));
}
// If the following fails, we're fucked.  Not sure what would cause it to fail,
// but everything is possible.
eval(variable_get('leftandright_'.$filename.'_replacement', ''));
// Load include file with the replacement functions in
module_load_include('admin.replace.inc', 'taxonomy');