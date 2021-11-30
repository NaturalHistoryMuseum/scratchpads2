<?php

/**
 * @file
 * Drush integration for colorbox.
 */

/**
 * The Colorbox plugin URI.
 */
define('COLORBOX_DOWNLOAD_URI', 'https://github.com/jackmoore/colorbox/archive/1.x.zip');
define('COLORBOX_DOWNLOAD_PREFIX', 'colorbox-');

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @codingStandardsIgnoreLine
 * @return array
 *   An associative array describing your command(s).
 */
function colorbox_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['colorbox-plugin'] = array(
    'callback' => 'drush_colorbox_plugin',
    'description' => dt('Download and install the Colorbox plugin.'),
  // No bootstrap.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Colorbox plugin. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('colorboxplugin'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @codingStandardsIgnoreLine
 * @param string $section
 *   A string with the help section (prepend with 'drush:')
 *
 * @codingStandardsIgnoreLine
 * @return string
 *   A string with the help text for your command.
 */
function colorbox_drush_help($section) {
  switch ($section) {
    case 'drush:colorbox-plugin':
      return dt('Download and install the Colorbox plugin from jacklmoore.com/colorbox, default location is sites/all/libraries.');
  }
}

/**
 * Implements drush_MODULE_pre_pm_enable().
 */
function drush_colorbox_pre_pm_enable() {
  $modules = drush_get_context('PM_ENABLE_MODULES');
  if (in_array('colorbox', $modules) && !drush_get_option('skip')) {
    drush_colorbox_plugin();
  }
}

/**
 * Command to download the Colorbox plugin.
 */
function drush_colorbox_plugin() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Download colorbox plugin only if path is writable.
  if (is_writable($path)) {
    // Set the directory to the download location.
    $olddir = getcwd();
    chdir($path);

    // Download the zip archive.
    if ($filepath = drush_download_file(COLORBOX_DOWNLOAD_URI)) {
      $filename = basename($filepath);
      $dirname = COLORBOX_DOWNLOAD_PREFIX . basename($filepath, '.zip');

      // Remove any existing Colorbox plugin directory.
      if (is_dir($dirname) || is_dir('colorbox')) {
        drush_delete_dir($dirname, TRUE);
        drush_delete_dir('colorbox', TRUE);
        drush_log(dt('A existing Colorbox plugin was deleted from @path', array('@path' => $path)), 'notice');
      }

      // Decompress the zip archive.
      drush_tarball_extract($filename);

      // Change the directory name to "colorbox" if needed.
      if ($dirname != 'colorbox') {
        drush_move_dir($dirname, 'colorbox', TRUE);
        $dirname = 'colorbox';
      }
    }

    if (is_dir($dirname)) {
      drush_log(dt('Colorbox plugin has been installed in @path', array('@path' => $path)), 'success');
    }
    else {
      drush_log(dt('Drush was unable to install the Colorbox plugin to @path', array('@path' => $path)), 'error');
    }

    // Set working directory back to the previous working directory.
    chdir($olddir);
  }
  else {
    drush_log(dt('Drush was unable to install the Colorbox plugin because @path is not writable. If you enable the colorbox module before you install the plugin library, you may find that colorbox does not work until you reinstall the colorbox module.', array('@path' => $path)), 'warning');
  }
}
