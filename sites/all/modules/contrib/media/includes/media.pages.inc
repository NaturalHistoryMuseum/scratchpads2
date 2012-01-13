<?php

/**
 * @file
 * Common pages for the Media module.
 */

/**
 *  Menu callback; display a thumbnail for an AJAX preview.
 */
function media_preview_ajax() {
  $url = $_GET['url'];
  $uri = media_parse_to_uri($url);
  if ($uri) {
    $file = file_uri_to_object($uri);
    drupal_json_output(array(
      'preview' => theme('media_admin_thumbnail', array('file' => $file)),
    ));
  }
  else {
    drupal_json_output(array(
      'error' => t('Invalid URL.'),
    ));
  }
  die();
}

/**
 * Menu callback; Edit multiple files on the same page using multiform module.
 *
 * @see media_file_operation_edit_multiple()
 */
function media_file_page_edit_multiple($files) {
  // @todo When http://drupal.org/node/1227706 is fixed, filter the $files
  // array using file_access($file, 'edit').

  if (empty($files)) {
    return MENU_ACCESS_DENIED;
  }

  $forms = array();
  foreach ($files as $file) {
    // To maintain unique form_ids, append the file id.
    $forms[] = array('media_edit_' . $file->fid, $file);
  }

  $form = call_user_func_array('multiform_get_form', $forms);
  $form['#attributes']['class'][] = 'media-multiedit-form';

  // Add the title to each 'subform'.
  foreach (element_children($form['multiform']) as $key) {
    $fid = $form['multiform'][$key]['fid']['#value'];
    $file = $files[$fid];
    $title = t('<em>Edit @type</em> @title', array('@type' => $file->type, '@title' => $file->filename));
    $form['multiform'][$key]['#prefix'] = '<h2>' . $title . '</h2>';
  }

  if (isset($form['buttons']['Delete'])) {
    $form['buttons']['Delete']['#access'] = FALSE;
  }

  return $form;
}

/**
 * Form callback for adding media via an upload form.
 * @todo: should use the AJAX uploader
 */
function media_add_upload($form, &$form_state, $params = array()) {
  // Set up file upload validators.
  $validators = array();

  // Validate file extensions. If there are no file extensions in $params and
  // there are no Media defaults, there is no file extension validation.
  if (!empty($params['file_extensions'])) {
    $validators['file_validate_extensions'] = array($params['file_extensions']);
  }
  elseif ($tmp = media_variable_get('file_extensions')) {
    $validators['file_validate_extensions'] = array($tmp);
  }

  // Validate file size but do not allow anything higher than file_upload_max_size().
  $max_filesize = file_upload_max_size();
  if (!empty($params['max_filesize']) && $params['max_filesize'] < $max_filesize) {
    $validators['file_validate_size'] = array(parse_size($params['max_filesize']));
  }
  elseif (($tmp = media_variable_get('max_filesize')) && $tmp < $max_filesize) {
    $validators['file_validate_size'] = array(parse_size($tmp));
  }
  else {
    $validators['file_validate_size'] = array($max_filesize);
  }

  // Add image validators.
  $params += array('min_resolution' => 0, 'max_resolution' => 0);
  if ($params['min_resolution'] || $params['max_resolution']) {
    $validators['file_validate_image_resolution'] = array($params['max_resolution'], $params['min_resolution']);
  }

  $form['#validators'] = $validators;

  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a new file'),
    '#description' => theme('file_upload_help', array('description' => '', 'upload_validators' => $validators)),
    '#upload_validators' => $validators,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Validate the generic file upload with the global media settings.
 */
function media_add_upload_validate($form, &$form_state) {
  // Save the file as a temporary file.
  $file = file_save_upload('upload', $form['#validators']);

  if ($file === NULL) {
    form_set_error('upload', t("No file appears to have been selected."));
  }
  elseif ($file === FALSE) {
    form_set_error('upload', t("Failed to upload the file."));
  }
  else {
    $form_state['values']['upload'] = $file;
  }
}

/**
 * Upload a file.
 */
function media_add_upload_submit($form, &$form_state) {
  $params = $form_state['build_info']['args'][0];
  $file = $form_state['values']['upload'];

  // The media browser widget does not use the 'display' field.
  $file->display = TRUE;

  // Change the file status from temporary to permanent.
  _media_save_file_permenently($file);

  // Determine what URI scheme this file should use.
  $scheme = !empty($params['uri_scheme']) && file_stream_wrapper_valid_scheme($params['uri_scheme']) ? $params['uri_scheme'] : file_default_scheme();
  $scheme .= '://';

  // Prepare the file's subdirectory path.
  $directory = '';
  if (!empty($params['file_directory'])) {
    $directory = token_replace($params['file_directory']) . '/';

    // If the directory isn't writable, or doesn't exist and can't be created,
    // the upload will fail.
    $prepare_directory = file_stream_wrapper_uri_normalize($scheme . $directory);
    if (!file_prepare_directory($prepare_directory, FILE_CREATE_DIRECTORY)) {
      drupal_set_message(t('The file directory @dir does not exist or is not writable. Please contact an administrator.', array('@dir' => $prepare_directory)), 'error');
      return;
    }
  }

  // Compose the file's permanent destination.
  $destination = file_stream_wrapper_uri_normalize($scheme . $directory . $file->filename);

  // Save the uploaded file to the permanent location.
  $file = file_move($file, $destination, FILE_EXISTS_RENAME);

  if ($file) {
    drupal_set_message(t('The file @name was uploaded', array('@name' => $file->filename)));
  }
  else {
    drupal_set_message(t('An error occurred and no file was uploaded.'), 'error');
    return;
  }

  $form_state['redirect'] = array('media/browser', array('query' => array('render' => 'media-popup', 'fid' => $file->fid)));
}

function media_add_upload_multiple($form, &$form_state, $params = array()) {
  $form = media_add_upload($form, $form_state, $params);
  unset($form['upload']['#title']);
  // The validators will be set from plupload anyway.  This isn't pretty, but don't
  // it to show up twice.
  unset($form['upload']['#description']);

  $form['upload']['#type'] = 'plupload';
  $form['submit']['#value'] = t('Start upload');
  return $form;
}

function media_add_upload_multiple_submit($form, &$form_state) {
  $scheme = variable_get('file_default_scheme', 'public') . '://';
  $saved_files = array();
  // We can't use file_save_upload() because of http://www.jacobsingh.name/content/tight-coupling-no-not
  foreach ($form_state['values']['upload'] as $uploaded_file) {
    if ($uploaded_file['status'] == 'done') {
      $source = $uploaded_file['tmppath'];
      $destination = file_stream_wrapper_uri_normalize($scheme . $uploaded_file['name']);
      // Rename it to its original name, and put it in its final home.
      // Note - not using file_move here because if we call file_get_mime
      // (in file_uri_to_object) while it has a .tmp extension, it horks.

      $destination = file_unmanaged_move($source, $destination, FILE_EXISTS_RENAME);

      $file = file_uri_to_object($destination);
      file_save($file);
      _media_save_file_permenently($file);

      $saved_files[] = $file;
    }
    else {
      // @todo: move this to element validate or something and clean up t().
      form_set_error('pud', "Upload of {$uploaded_file['name']} failed");
    }
  }
  // Get a list of fids to pass back.
  $fids = array();
  foreach ($saved_files as $file) {
    $fids[] = $file->fid;
  }
  $form_state['redirect'] = array('media/browser', array('query' => array('render' => 'media-popup', 'fid' => $fids)));
}

function media_add_remote($form, &$form_state) {
  // Totally prototyping code to show designs.
  $form['sources'] = array(
    '#type' => 'vertical_tabs',
    '#title' => 'Sources',
  );

  $form['sources']['paste'] = array(
    '#type' => 'fieldset',
    '#title' => 'Paste URL or embed code',
  );


  $providers = '';
  $providers .= '<img style="height:50px; margin:20px" src="http://www.macuser.com/2008/10/09/top_youtube_logo_31_Dec_06.jpg">';
  $providers .= '<img style="height:50px; margin:20px" src="http://jasonhilldesign.com/FlikrLogo.jpg">';
  $form['sources']['paste']['code'] = array(
    '#type' => 'textarea',
    '#title' => t('URL or embed code'),
    '#description' => t('The following providers are supported: <br/>' . $providers),
  );

  $form['sources']['youtube'] = array(
    '#type' => 'fieldset',
    '#title' => 'YouTube',
    '#description' => t(''),
    '#attributes' => array('style' => 'height: 300px; overflow:auto'),
  );

  $form['sources']['flikr'] = array(
    '#type' => 'fieldset',
    '#title' => 'Flikr',
  );

  $box = '<div style="width:100px; height:100px; border:1px solid blue; padding:10px; float:left; margin:5px;"> Video </div>';
  $boxes = '';
  for ($i = 0; $i < 10; $i++) {
    $boxes .= $box;
  }
  $form['sources']['youtube']['stupid'] = array(
    '#markup' => $boxes,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#attributes' => array('style' => 'float:right'),
    '#suffix' => '<br style="clear:both" />',
  );

  return $form;
}
