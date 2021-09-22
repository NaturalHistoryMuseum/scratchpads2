<?php
/**
 * @file
 * Features hooks for the uuid_file features component.
 */

/**
 * Implements hook_features_export().
 */
function uuid_file_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['uuid_features'] = 'uuid_features';

  foreach ($data as $uuid) {
    $export['features']['uuid_file'][$uuid] = $uuid;
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_file_features_export_render($module, $data) {
  $translatables = $code = array();

  $code[] = '  $files = array();';
  $code[] = '';
  foreach ($data as $uuid) {
    $fid = uuid_get_serial_id('files', 'fid', $uuid);
    if (!$fid) {
      continue;
    }

    $file = field_file_load($fid);
    $file_parts = explode('.', $file['filepath']);
    $extension = array_pop($file_parts);

    $export = array(
      'uuid' => $uuid,
      'hash' => md5_file($file['filepath']),
      'extension' => $extension,
    );

    $code[] = '  $files[] = ' . features_var_export($export, '  ') . ';';
  }
  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $files;';
  $code = implode("\n", $code);
  return array('uuid_features_default_files' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_file_features_revert($module) {
  uuid_file_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 * Rebuilds files based on UUID from code defaults.
 */
function uuid_file_features_rebuild($module) {
  $files = module_invoke($module, 'uuid_features_default_files');
  if (!empty($files)) {
    $source_dir = drupal_get_path('module', $module) . '/uuid_file';
    foreach ($files as $data) {
      $source = $source_dir . '/' . $data['uuid'] . '.' . $data['extension'];
      $data = file_get_contents($source);

      // Copy the file and save to db.
      $file = file_save_data($data, NULL, FILE_EXISTS_REPLACE);
    }
  }
}
