<?php 

/**
 * @file
 * Extend Linkit with files.
 */


/**
 * Implements hook_linkit_load_plugins().
 */
function linkit_file_linkit_load_plugins($string) {
  $matches = array();
  
  // Get files.
  $result = db_select('file_managed', 'f')
    ->fields('f', array('filename', 'uri', 'filemime'))
    ->condition('f.filename', '%' . db_like($string) . '%', 'LIKE')
    ->condition('f.status' , '1')
    ->condition('f.uri', 'public://%', 'LIKE')
    ->execute();

  $public_path = variable_get('file_public_path', conf_path() . '/files');
  foreach ($result AS $file) {
    $matches['file'][] = array(
      'title' => $file->filename,
      'path' => $public_path . '/' . file_uri_target($file->uri),
      'information' => array(
        'type' => 'File',
        'mimetype' => $file->filemime,
      ),
    );
  }

  return $matches;
}

/**
 * Implements hook_linkit_get_search_styled_link().
 */
function linkit_file_linkit_get_search_styled_link($string) {
  return;
}

/**
 * Implements hook_linkit_info_plugins().
 * 
 * This is used by linkit_permissions
 */
function linkit_file_linkit_info_plugins() {
  $return['linkit_file'] = array(
    'type' => 'file',
  );
  return $return;
}