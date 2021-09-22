<?php

/**
 * Implements hook_features_export_options().
 */
function weight_features_export_options() {
  $settings = array_keys(_weight_get_settings());
  return drupal_map_assoc($settings);
}

/**
 * Implements hook_features_export().
 */
function weight_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['weight'] = 'weight';

  foreach ($data as $setting) {
    $export['features']['weight'][$setting] = $setting;
  }

  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function weight_features_export_render($module, $data) {
  $code = array();
  $code[] = '  $settings = array();';
  $code[] = '';

  foreach ($data as $name) {
    if ($settings = _weight_get_settings($name)) {
      $settings_export = features_var_export($settings, '  ');
      $code[] = "  \$settings['{$name}'] = {$settings_export};";
      $code[] = "";
    }
  }

  $code[] = '  return $settings;';
  $code = implode("\n", $code);

  return array('weight_features_default_settings' => $code);
}

/**
 * Implements hook_features_revert().
 */
function weight_features_revert($module) {
  weight_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function weight_features_rebuild($module) {
  if ($settings = features_get_default('weight', $module)) {
    weight_settings_save($settings);
  }
}
