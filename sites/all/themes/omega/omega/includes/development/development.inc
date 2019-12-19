<?php

/**
 * @file
 * Main extension file for the 'development' extension.
 */

// The development extension has some additional tasks that need to run on every
// page load (@see _omega_extension_development_boot()).
if (drupal_get_bootstrap_phase() >= DRUPAL_BOOTSTRAP_DATABASE && $GLOBALS['theme'] === $GLOBALS['theme_key'] && ($GLOBALS['theme'] == 'omega' || (!empty($GLOBALS['base_theme_info']) && $GLOBALS['base_theme_info'][0]->name == 'omega'))) {
  if (isset($GLOBALS['user']->uid) && function_exists('user_access')) {
    if (user_access('administer site configuration') && omega_extension_enabled('development')) {
      _omega_extension_development_boot();
    }
  }
}

/**
 * Implements hook_extension_EXTENSION_registry_alter().
 */
function omega_extension_development_theme_registry_alter(&$registry) {
  if (omega_theme_get_setting('omega_demo_regions', TRUE) || omega_theme_get_setting('omega_livereload', TRUE) || omega_theme_get_setting('omega_browser_width_indicator', FALSE)) {
    $registry['html']['preprocess functions'][] = 'omega_extension_development_preprocess_html';

    if (omega_theme_get_setting('omega_demo_regions', TRUE)) {
      $registry['region']['preprocess functions'][] = 'omega_extension_development_preprocess_region';
    }
  }
}

/**
 * Implements hook_extension_EXTENSION_preproces_html().
 */
function omega_extension_development_preprocess_html(&$variables) {
  $path = drupal_get_path('theme', 'omega');

  if (omega_theme_get_setting('omega_livereload', TRUE)) {
    // LiveReload obviously only works if CSS aggregation is disabled.
    if (!variable_get('preprocess_css', FALSE)) {
      $port = omega_theme_get_setting('omega_livereload_port', '35729');
      $host = omega_theme_get_setting('omega_livereload_host', 'localhost');
      $script = omega_theme_get_setting('omega_livereload_script', "http://$host:$port/livereload.js");
      $parsed = parse_url($script);

      $query = array('snipver' => '1');
      if (!isset($parsed['port']) || $parsed['port'] != $port) {
        // If no port is specified the livereload.js script will use the default
        // port (35729).
        $query['port'] = $port;
      }

      if (!isset($parsed['host']) || $parsed['host'] != $host) {
        // If no host is specified livereload.js will use the host that it is
        // served from (e.g. if livereload.js is served from example.com it will
        // try to connect to a livereload server on example.com.
        $query['host'] = $host;
      }

      // We need to add livereload.js as external script with an absolute path
      // because otherwise Drupal core messes with the query string.
      $script = empty($query) ? $script : $script . '?' . http_build_query($query);

      // Check if livereload.js can be loaded.
      if (@file_get_contents($script) === FALSE) {
        if (user_access('administer site configuration') && flood_is_allowed('omega_' . $GLOBALS['theme'] . '_livereload_error', 3, 3600)) {
          flood_register_event('omega_' . $GLOBALS['theme'] . '_livereload_error');
          drupal_set_message(t('LiveReload is currently enabled but could not be loaded. Please ensure that livereload.js is being served at the <a href="!link">configured path</a> (@script).', array(
            '@script' => $script,
            '!link' => url('admin/appearance/settings/' . $GLOBALS['theme']),
          )), 'error');
        }
      }
      else {
        drupal_add_js($script, array(
          'preprocess' => FALSE,
          'type' => 'external',
          'scope' => 'footer',
          'weight' => 1000,
        ));
      }
    }
    // If CSS aggregation is enabled, show a warning.
    elseif (user_access('administer site configuration') && flood_is_allowed('omega_' . $GLOBALS['theme'] . '_aggregation_and_livereload', 3, 3600)) {
      flood_register_event('omega_' . $GLOBALS['theme'] . '_aggregation_and_livereload');
      drupal_set_message(t('LiveReload does not work with aggregated CSS files. Please <a href="!link">disable CSS aggregation</a> if you want to use LiveReload for development.', array(
        '!link' => url('admin/config/development/performance'),
      )), 'warning');
    }
  }

  if (omega_theme_get_setting('omega_browser_width_indicator', FALSE) || omega_theme_get_setting('omega_demo_regions', TRUE)) {
    drupal_add_css($path . '/css/omega.development.css', array(
      'group' => CSS_THEME,
      'weight' => -10,
      'every_page' => TRUE,
    ));

    if (omega_theme_get_setting('omega_browser_width_indicator', FALSE)) {
      if (!module_exists('overlay') || (!$mode = overlay_get_mode()) || $mode == 'parent') {
        $variables['attributes_array']['class'][] = 'omega-browser-width-indicator';
        drupal_add_js($path . '/js/omega.indicator.min.js', array(
          'group' => JS_THEME,
          'weight' => -10,
          'every_page' => TRUE,
        ));
      }
    }
  }
}

/**
 * Implements hook_extension_EXTENSION_preproces_region().
 */
function omega_extension_development_preprocess_region(&$variables) {
  if ($variables['debug'] = !empty($variables['elements']['#debug'])) {
    $class = drupal_html_class('region--debug--' . $variables['region']);
    drupal_add_css(".$class:before { content: \"{$variables['elements']['#name']}\"; }", array(
      'type' => 'inline',
      'group' => CSS_THEME,
      'weight' => 1000,
    ));

    $variables['attributes_array']['class'][] = 'region--debug';
    $variables['attributes_array']['class'][] = $class;

    // Ensure that the content is not completely empty.
    $variables['content'] = !empty($variables['content']) ? $variables['content'] : '&nbsp;';
  }
}

/**
 * Helper function for bootstrap related tasks that need to run on every page.
 */
function _omega_extension_development_boot() {
  // Managing debugging (flood) messages and a few development tasks. This also
  // lives outside of any function declaration to make sure that the code is
  // executed before any theme hooks.
  if (omega_theme_get_setting('omega_enable_export_reminder') && variable_get('theme_' . $GLOBALS['theme'] . '_settings') && flood_is_allowed('omega_' . $GLOBALS['theme'] . '_theme_settings_warning', 3)) {
    // Alert the user that the theme settings are served from a variable.
    flood_register_event('omega_' . $GLOBALS['theme'] . '_theme_settings_warning');
    drupal_set_message(t("The settings for this theme are currently served from a variable. You might want to export them to your .info file. This is optional and can be done the Drush command 'drush omega-export'."), 'warning');
  }

  // Rebuild the theme registry / aggregates on every page load if the
  // development extension is enabled and configured to do so.
  if (omega_theme_get_setting('omega_rebuild_theme_registry', FALSE)) {
    // Rebuild the theme data.
    system_rebuild_theme_data();
    // Rebuild the theme registry.
    drupal_theme_rebuild();

    if (flood_is_allowed('omega_' . $GLOBALS['theme'] . '_rebuild_registry_warning', 3)) {
      // Alert the user that the theme registry is being rebuilt on every
      // request.
      flood_register_event('omega_' . $GLOBALS['theme'] . '_rebuild_registry_warning');
      drupal_set_message(t('The theme registry is being rebuilt on every request. Remember to <a href="!url">turn off</a> this feature on production websites.', array("!url" => url('admin/appearance/settings/' . $GLOBALS['theme']))), 'warning');
    }
  }

  if (omega_theme_get_setting('omega_rebuild_aggregates', FALSE) && variable_get('preprocess_css', FALSE) && (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'update')) {
    foreach (array('css', 'js') as $type) {
      variable_del('drupal_' . $type . '_cache_files');

      foreach (file_scan_directory('public://' . $type . '', '/.*/') as $file) {
        // Delete files that are older than 20 seconds.
        if (REQUEST_TIME - filemtime($file->uri) > 20) {
          file_unmanaged_delete($file->uri);
        }
      };
    }

    if (flood_is_allowed('omega_' . $GLOBALS['theme'] . '_rebuild_aggregates_warning', 3)) {
      // Alert the user that the theme registry is being rebuilt on every
      // request.
      flood_register_event('omega_' . $GLOBALS['theme'] . '_rebuild_aggregates_warning');
      drupal_set_message(t('The CSS and JS aggregates are being rebuilt on every request. Remember to <a href="!url">turn off</a> this feature on production websites.', array("!url" => url('admin/appearance/settings/' . $GLOBALS['theme']))), 'warning');
    }
  }
}
