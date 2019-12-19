<?php

/**
 * @file
 * Main extension file for the 'assets' extension.
 */

/**
 * Implements hook_extension_EXTENSION_registry_alter().
 */
function omega_extension_assets_theme_registry_alter(&$registry) {
  if ($settings = omega_theme_get_setting('omega_libraries', array())) {
    foreach (omega_theme_libraries_info() as $library => $info) {
      if (!empty($settings[$library]['status'])) {
        $registry['html']['libraries'][$library] = array(
          'attached' => array(),
          'settings' => $settings[$library],
          'callbacks' => isset($info['callbacks']) ? array_unique($info['callbacks']) : FALSE,
          'path' => $info['path'],
        );

        if (isset($settings[$library]['variant']) && ($variant = $settings[$library]['variant']) && isset($info['variants'][$variant])) {
          $files = _omega_extension_assets_prepare_library_files($library, $info, $info['variants'][$variant]['files']);
          $registry['html']['libraries'][$library]['attached'] = $files;
        }
        else {
          $files = _omega_extension_assets_prepare_library_files($library, $info, $info['files']);
          $registry['html']['libraries'][$library]['attached'] = $files;
        }
      }
    }
  }

  if (!empty($registry['html']['libraries'])) {
    $registry['html']['preprocess functions'][] = 'omega_extension_assets_attach_libraries';
  }
}

/**
 * Extension callback for attaching enabled libraries.
 */
function omega_extension_assets_attach_libraries(&$variables) {
  $registry = theme_get_registry();

  // Check if there are any enabled libraries.
  $libraries = module_exists('devel_themer') ? $registry['html']['original']['libraries'] : $registry['html']['libraries'];
  foreach ($libraries as $library => $info) {
    drupal_process_attached(array('#attached' => $info['attached']));

    if (!empty($info['callbacks'])) {
      foreach ($info['callbacks'] as $callback) {
        $callback($library, $info['settings'], $info);
      }
    }
  }
}

/**
 * Library callback for checking if CSS aggregation is enabled.
 */
function omega_extension_assets_requirements_css_aggregation($library, $settings) {
  if ((!variable_get('preprocess_css', FALSE) && (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'update'))) {
    if (user_access('administer site configuration') && flood_is_allowed('omega_' . $GLOBALS['theme'] . '_aggregation_required', 3, 3600, $library)) {
      $libraries = omega_theme_libraries_info();
      $info = $libraries[$library];

      // Tell the user that the library does not work without CSS aggregation.
      flood_register_event('omega_' . $GLOBALS['theme'] . '_aggregation_required', 3600, $library);
      drupal_set_message(t('The %library library requires aggregated CSS files to work properly. You can enable CSS aggregation in the <a href="!url">performance settings</a>.', array('!url' => url('admin/config/development/performance'), '%library' => $info['name'])), 'warning');
    }
  }
}

/**
 * Library callback for loading
 */
function omega_extension_assets_load_pie_selectors($library, $settings, $info) {
  if (empty($settings['selectors'])) {
    return;
  }

  $htc = base_path() . "{$info['path']}/PIE.htc";
  $destination = file_create_url('public://omega/' . $GLOBALS['theme']);
  $destination = substr($destination, strlen($GLOBALS['base_url']) + 1);
  if (empty($settings['variant'])) {
    // Save the generated CSS in the public file system.
    $file = $destination . '/pie-selectors.css';
    if (!file_exists($file)) {
      $contents = implode(",", $settings['selectors']) . "{behavior:url($htc)}";
      file_prepare_directory($destination, FILE_CREATE_DIRECTORY);
      file_unmanaged_save_data($contents, $file, FILE_EXISTS_REPLACE);
    }

    drupal_add_css($file, array(
      'browsers' => array('IE' => '(gte IE 6)&(lte IE 8)', '!IE' => FALSE),
      'weight' => 100,
      'every_page' => TRUE,
    ));
  }
  elseif ($settings['variant'] == 'js') {
    // Save the generated JS in the public file system.
    $file = $destination . '/pie-selectors.js';
    if (!file_exists($file)) {
      $contents = '$(function(){Drupal.behaviors.css3pie={attach:function(context,settings){if(window.PIE){$("' . implode(",", $settings['selectors']) . '", context).each(function(){PIE.attach(this)})}}}})(jQuery);';
      file_prepare_directory($destination, FILE_CREATE_DIRECTORY);
      file_unmanaged_save_data($contents, $file, FILE_EXISTS_REPLACE);
    }

    drupal_add_js($file, array(
      'browsers' => array('IE' => '(gte IE 6)&(lte IE 8)', '!IE' => FALSE),
      'weight' => 100,
      'every_page' => TRUE,
    ));
  }
}

/**
 * Resolves library file paths before they get written to the theme registry.
 */
function _omega_extension_assets_prepare_library_files($library, $info, $files) {
  foreach (array('css', 'js') as $type) {
    if (empty($files[$type])) {
      continue;
    }

    foreach ($files[$type] as $file => $data) {
      $path = is_array($data) ? $file : $data;
      $path = "{$info['path']}/$path";
      if (is_array($data)) {
        $values = array_values($files[$type]);
        $keys = array_keys($files[$type]);
        $offset = array_search($file, $keys);
        $keys[$offset] = $path;
        $files[$type] = array_combine($keys, $values);
      }
      else {
        $files[$type][$file] = $path;
      }
    }
  }

  return $files;
}
