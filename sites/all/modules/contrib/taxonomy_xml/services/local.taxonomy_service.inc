<?php
/* 
 */

/**
 * @file declare how to import a taxonomy from local sources.
 */

/**
 * hook_taxonomy_service_info()
 */ 
function local_taxonomy_service_info() {
  $services = array();
  $services['local'] = array(
    'provider' => 'Server filesystem',
    'name' => 'Local files',
    'id' => 'local',
    'description' => '
      Choose from available files stored on the server.
    ',
    // Define the name of the form function that returns service-specific UI
    'import_form_callback' => 'local_taxonomy_service_form',
  );
  return $services;
}

/**
 * A sub-form that provides UI additions to the taxonomy import form
 */
function local_taxonomy_service_form($form_values, $service_info) {
  $form = array();
 
  // List available files 
  $search_paths = array(
    drupal_get_path('module', 'taxonomy_xml') . '/samples',
    // add more?
  );
  $files = array();
  foreach ($search_paths as $search_path) {
    $found = file_scan_directory($search_path, '|.*|');
    foreach ($found as $file) {
      $files[$search_path][$file->uri] = $file->filename;
    }
  }
  
  $form['filepath'] = array(
    '#type' => 'select',
    '#title' => t('File to import'),
    '#options' => $files,
  );
  $formats = taxonomy_xml_formats();
  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Format of file'),
    '#default_value' => $form_values['format'],
    '#options' => $formats,
  );
  $form['submit'] = array(
    '#type' => 'submit',    
    '#value' => t('Import from '. $service_info['name']),
    '#submit' => array('local_taxonomy_service_form_submit'),
  );

  return $form;
}

/**
 * What to do when loading from this service
 */
function local_taxonomy_service_form_submit($form, &$form_state) {
  // Invoke service and parse response
  $service_id = $form_state['values']['service_id'];
  $parameters = $form_state['values'];
  $parameters['format'] = $parameters[$service_id]['format'];

  $filepath = $parameters[$service_id]['filepath'];
  $text = file_get_contents($filepath);
  taxonomy_xml_invoke_import($text, $parameters, $filepath);
}