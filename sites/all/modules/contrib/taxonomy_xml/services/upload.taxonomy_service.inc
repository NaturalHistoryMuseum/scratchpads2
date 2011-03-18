<?php
/* 
 */

/**
 * @file declare how to import a taxonomy from file upload.
 */

/**
 * hook_taxonomy_service_info()
 */ 
function upload_taxonomy_service_info() {
  $services = array();
  $services['file_upload'] = array(
    'provider' => 'Your Desktop',
    'name' => 'Direct File Upload',
    'id' => 'file_upload',
    'description' => '
      Upload a file in one of the supported formats.
    ',
    // Define the name of the form function that returns service-specific UI
    'import_form_callback' => 'upload_taxonomy_service_form',
  );
  return $services;
}

/**
 * A sub-form that provides UI additions to the taxonomy import form
 */
function upload_taxonomy_service_form($form_values, $service_info) {
  $form = array();
  $form['file_upload'] = array(
    '#type' => 'file',
    '#title' => t('File to import'),
    '#description' => t('Click "Browse..." to select a local document to upload.'),
  );

  $formats = taxonomy_xml_formats();
  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Format of file'),
#    '#default_value' => variable_get('taxonomy_xml_format', 'xml_format'),
    '#options' => $formats,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import from '. $service_info['name']),
    '#submit' => array('upload_taxonomy_service_form_submit'),
  );
  return $form;
}

/**
 * What to do when loading from this service
 */
function upload_taxonomy_service_form_submit($form, &$form_state) {
  // Invoke service and parse response
  $service_id = $form_state['values']['service_id'];
  #$services =   $services_info = taxonomy_xml_services();
  #$service = $services[$service_id];

  $parameters = $form_state['values'];
  $parameters['format'] = $parameters[$service_id]['format'];

  // Retrieve submitted data and parse it
  global $user;
  // @todo there could be an issue when uploading as non-uid-1.
  // file_save_upload may not save unknown extensions.
  // Currently unsolved.
  if (! $file = file_save_upload('file_upload')) {
    form_set_error('upload_file', t('Vocabulary import failed: file was not uploaded.'));
    dpm($_FILES);
    return FALSE;
  }

  // The file object is a wierd wrapper instance.
  //
  //  Drupal7 is odd. How is this supposed to work?
  // http://drupal.org/node/822008
  //
  //
  $file->filepath = drupal_realpath($file->uri);
  $fd = fopen($file->filepath, "rb");
  if (!$fd) {
    form_set_error('upload_file', t('Vocabulary import failed: file %filename cannot be read.', array('%filename' => $file->filepath)));
    return FALSE;
  }
  else {
    $info = fstat($fd);
    $len = $info["size"];
    $text = fread($fd, $len);
    fclose($fd);
    drupal_set_message(t('Loaded file %filename. Now processing it.', array('%filename' => $file->filename)));
    $form_state['values']['file'] = $file;
    taxonomy_xml_invoke_import($text, $parameters, $file->filename);
  }
}