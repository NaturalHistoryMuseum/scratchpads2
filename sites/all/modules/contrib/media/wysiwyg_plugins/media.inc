<?php

/**
 * @file
 * Define the WYSIWYG browser plugin.
 */

/**
 * Implementation of WYSIWYG's hook_INCLUDE_plugin().
 */
function media_media_plugin() {
  // Include the required browser JS.
  // @todo: wyswiyg should allow libraries and multiple js files
  // to be defined by this hook.
  // @see http://drupal.org/node/1039076
  media_include_browser_js();

  // Plugin definition
  $plugins['media'] = array(
    'title' => t(media_variable_get('wysiwyg_title')),
    'vendor url' => 'http://drupal.org/project/media',
    'icon path' => drupal_get_path('module', 'media') . '/images',
    'icon file' => 'wysiwyg-media.gif',
    'icon title' => t(media_variable_get('wysiwyg_icon_title')),
    // @todo: move this to the plugin directory for the wysiwyg plugin.
    'js path' => drupal_get_path('module', 'media') . '/js',
    'js file' => 'wysiwyg-media.js',
    'css file' => NULL,
    'css path' => NULL,
    'settings' => array(
      'global' => array(
        'types' => media_variable_get('wysiwyg_allowed_types'),
        'id' => 'media_wysiwyg',
      ),
    ),
  );

  return $plugins;
}

/**
 * Prepares the page to be able to launch the media browser.
 *
 * Defines default variables.
 */
function media_include_browser_js() {
  static $included;
  if ($included) {
    return;
  }
  $included = TRUE;
  module_load_include('inc', 'media', 'includes/media.browser');
  $javascript = media_browser_js();
  foreach ($javascript as $key => $definitions) {
    foreach ($definitions as $definition) {
      $function = 'drupal_add_' . $key;
      // Since the arguments to pass are variable, use call_user_func_array().
      // This will not handle all potential drupal_add_*() functions directly
      // but covers the js and library needed here, which are unlikely to be
      // expanded since this function is only a workaround for a wysiwyg limitation.
      call_user_func_array($function, $definition);
    }
  }
  // Add wysiwyg-specific settings.
  $settings = array('blacklist' => array('src', 'fid', 'view_mode', 'format'));
  drupal_add_js(array('media' => $settings), 'setting');
}
