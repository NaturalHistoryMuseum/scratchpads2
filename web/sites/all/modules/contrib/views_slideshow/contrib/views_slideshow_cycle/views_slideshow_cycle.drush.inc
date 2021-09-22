<?php

/**
 * @file
 * Drush integration for Views Slideshow.
 *
 * Specifically downloading of required and optional JavaScript libraries.
 */

/**
 * Implements hook_drush_command().
 */
function views_slideshow_cycle_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['views-slideshow-cycle-cycle'] = array(
    'description' => dt('Download and install the jQuery Cycle library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. The path to install the jQuery Cycle library in. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('dl-cycle'),
  );
  $items['views-slideshow-cycle-json2'] = array(
    'description' => dt('Download and install the JSON2 library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. The path to install the JSON2 library in. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('dl-json2'),
  );
  $items['views-slideshow-cycle-hoverintent'] = array(
    'description' => dt('Download and install the jquery.hoverIntent library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. The path to install the jquery.hoverIntent library in. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('dl-hoverintent'),
  );
  $items['views-slideshow-cycle-pause'] = array(
    'description' => dt('Download and install the jQuery.pause library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. The path to install the jQuery.pause library in. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('dl-pause'),
  );
  $items['views-slideshow-cycle-easing'] = array(
    'description' => dt('Download and install the jQuery.easing library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional. The path to install the jQuery.easing library in. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('dl-easing'),
  );
  $items['views-slideshow-cycle-lib'] = array(
    'description' => dt('Download and install the jQuery Cycle, jQuery hoverIntent and JSON2 libraries.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('dl-cycle-lib'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function views_slideshow_cycle_drush_help($section) {
  switch ($section) {
    case 'drush:views-slideshow-cycle-cycle':
      return dt('Download and install the jQuery Cycle library from http://malsup.github.com/jquery.cycle.all.js, default location is libraries/jquery.cycle.');

    case 'drush:views-slideshow-cycle-hoverintent':
      return dt('Download and install the jQuery hoverIntent library from https://raw.githubusercontent.com/briancherne/jquery-hoverIntent/master/jquery.hoverIntent.js, default location is libraries/jquery.cycle.');

    case 'drush:views-slideshow-cycle-pause':
      return dt('Download and install the jQuery pause library from https://raw.githubusercontent.com/tobia/Pause/master/jquery.pause.js, default location is libraries/jquery.pause.');

    case 'drush:views-slideshow-cycle-easing':
      return dt('Download and install the jQuery pause library from http://gsgd.co.uk/sandbox/jquery/easing/jquery.easing.1.3.js, default location is libraries/jquery.easing.');

    case 'drush:views-slideshow-cycle-json2':
      return dt('Download and install the JSON2 library from https://github.com/douglascrockford/JSON-js/, default location is libraries/json2.');

    case 'drush:views-slideshow-cycle-lib':
      return dt('Download and install the jQuery Cycle, jQuery hoverIntent and JSON2 libraries.');
  }
}

/**
 * Command to download the jQuery Cycle library.
 */
function drush_views_slideshow_cycle_cycle() {
  _drush_views_slideshow_cycle_install_library(
    'jQuery Cycle',
    'libraries/jquery.cycle',
    'jquery.cycle.all.js',
    'http://malsup.github.com/jquery.cycle.all.js',
    func_get_args()
  );
}

/**
 * Command to download the JSON2 library.
 */
function drush_views_slideshow_cycle_json2() {
  _drush_views_slideshow_cycle_install_library(
    'JSON2',
    'libraries/json2',
    'json2.js',
    'https://raw.githubusercontent.com/douglascrockford/JSON-js/master/json2.js',
    func_get_args()
  );
}

/**
 * Command to download the jQuery.hoverIntent library.
 */
function drush_views_slideshow_cycle_hoverintent() {
  _drush_views_slideshow_cycle_install_library(
    'jQuery HoverIntent',
    'libraries/jquery.hoverIntent',
    'jquery.hoverIntent.js',
    'https://raw.githubusercontent.com/briancherne/jquery-hoverIntent/master/jquery.hoverIntent.js',
    func_get_args()
  );
}

/**
 * Command to download the jQuery.pause library.
 */
function drush_views_slideshow_cycle_pause() {
  _drush_views_slideshow_cycle_install_library(
    'jQuery Pause',
    'libraries/jquery.pause',
    'jquery.pause.js',
    'https://raw.githubusercontent.com/tobia/Pause/master/jquery.pause.js',
    func_get_args()
  );
}

/**
 * Command to download the jQuery.easing library.
 */
function drush_views_slideshow_cycle_easing() {
  _drush_views_slideshow_cycle_install_library(
    'jQuery easing',
    'libraries/jquery.easing',
    'jquery.easing.1.3.js',
    'http://gsgd.co.uk/sandbox/jquery/easing/jquery.easing.1.3.js',
    func_get_args()
  );
}

/**
 * Command to download all libraries.
 */
function drush_views_slideshow_cycle_lib() {
  drush_views_slideshow_cycle_cycle();
  drush_views_slideshow_cycle_json2();
  drush_views_slideshow_cycle_hoverintent();
  drush_views_slideshow_cycle_pause();
  drush_views_slideshow_cycle_easing();
}

/**
 * Implements drush_MODULE_pre_pm_enable().
 */
function drush_views_slideshow_cycle_pre_pm_enable() {
  $modules = drush_get_context('PM_ENABLE_MODULES');
  if (in_array('views_slideshow_cycle', $modules) && !drush_get_option('skip')) {
    drush_views_slideshow_cycle_lib();
  }
}

/**
 * Helper function to download a library in the given directory.
 */
function _drush_views_slideshow_cycle_install_library($name, $path, $filename, $url, $args) {
  // Check if path provided as an arg and use that instead of default if so.
  if (!empty($args[0])) {
    $path = $args[0];
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path, 0755, TRUE);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'success');
  }

  // Be sure we can write in the directory.
  $perms = substr(sprintf('%o', fileperms($path)), -4);
  if ($perms !== '0755') {
    drush_shell_exec('chmod 755 ' . $path);
  }

  $dir = getcwd();

  // Download the JavaScript file.
  if (is_file($path . '/' . $filename)) {
    drush_log(dt('@name appears to be already installed.', array(
      '@name' => $name,
    )), 'ok');
  }
  elseif (drush_op('chdir', $path) && drush_shell_exec('wget --no-check-certificate ' . $url)) {
    drush_log(dt('The latest version of @name has been downloaded to @path', array(
      '@name' => $name,
      '@path' => $path,
    )), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the @name library to @path', array(
      '@name' => $name,
      '@path' => $path,
    )), 'error');
  }

  chdir($dir);

  // Restore the previous permissions.
  drush_shell_exec('chmod ' . $perms . ' ' . $path);
}
