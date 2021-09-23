<?php

/**
 * @file
 *   uuid_features module drush integration.
 */

/**
 * Implements hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function uuid_features_drush_command() {
  $items = array();

  $items['uuid-features-update-files'] = array(
    'callback' => 'uuid_features_command_update_files',
    'description' => "Update files for features modules that use the uuid_file component.",
    'arguments' => array(
      'feature' => 'Feature name to export.',
    ),
    'drupal dependencies' => array('features', 'uuid', 'uuid_features', 'filefield'),
    'aliases' => array('ufuf'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function uuid_features_drush_help($section) {
  switch ($section) {
    case 'drush:features':
      return dt("List all the available features for your site.");
  }
}

/**
 * Update files for features modules that use the uuid_file component.
 */
function uuid_features_command_update_files($feature = NULL) {
  if ($args = func_get_args()) {
    foreach ($args as $module) {
      if (($feature = feature_load($module, TRUE)) && module_exists($module)) {
        if (!empty($feature->info['features']['uuid_file'])) {
          $files = $feature->info['features']['uuid_file'];

          $dest = drupal_get_path('module', $module) . '/uuid_file';
          file_prepare_directory($dest, FILE_CREATE_DIRECTORY);

          foreach ($files as $uuid) {
            _uuid_features_drush_update_file($module, $uuid);
          }
        }
      }
      else if ($feature) {
        _features_drush_set_error($module, 'FEATURES_FEATURE_NOT_ENABLED');
      }
      else {
        _features_drush_set_error($module);
      }
    }
  }
  else {
    // By default just show contexts that are available.
    $rows = array(array(dt('Available features')));
    foreach (features_get_features(NULL, TRUE) as $name => $info) {
      $rows[] = array($name);
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Copy the file identified by $uuid into the feature specified by $module.
 *
 * @param string $module
 * @param string $uuid
 */
function _uuid_features_drush_update_file($module, $uuid) {
  $fid = uuid_get_serial_id('files', 'fid', $uuid);
  if (empty($fid) || !($file = field_file_load($fid))) {
    drush_set_error('UUID_FILE_NOT_FOUND', dt('The file specified by %uuid was not found.', array('%uuid' => $uuid)));
  }

  $root = drush_get_option(array('r', 'root'), drush_locate_root());
  if ($root) {
    $directory = drupal_get_path('module', $module) . '/uuid_file';
    if (!is_dir($directory)) {
      drush_op('mkdir', $directory);
    }
    $source = $file['filepath'];
    $file_parts = explode('.', $file['filepath']);
    $extension = array_pop($file_parts);

    $destination = $directory . '/' . $uuid . '.' . $extension;
    drush_op('copy', $source, $destination);
    drush_log(dt("Updated file: !uuid - !filename", array('!uuid' => $uuid, '!filename' => basename($destination))), 'ok');
  }
  else {
    drush_die(dt('Couldn\'t locate site root'));
  }
}
