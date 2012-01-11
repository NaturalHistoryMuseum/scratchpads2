<?php

/**
 * @file
 * Integration with the features module.
 */

/**
 * Implements hook_features_export().
 */
function hierarchical_select_features_export($data, &$export, $module_name) {
  // Add hierarchical_select dependency.
  $export['dependencies']['hierarchical_select'] = 'hierarchical_select';

  // Retrieve dependencies from all information items.
  $dependencies = array();
  foreach (module_implements('hierarchical_select_config_info') as $module) {
    $configs = module_invoke($module, 'hierarchical_select_config_info');
    foreach ($configs as $config_id => $config) {
      $dependencies[$config_id] = $module;
    }
  }

  // Add features and dependencies.
  foreach ($data as $config_id) {
    $export['features']['hierarchical_select'][$config_id] = $config_id;
    if (isset($dependencies[$config_id])) {
      $module = $dependencies[$config_id];
      $export['dependencies'][$module] = $module;
    }
  }

  return array();
}

/**
 * Implements hook_features_export_options().
 */
function hierarchical_select_features_export_options() {
  // Retrieve all information items.
  $info_items = array();
  foreach (module_implements('hierarchical_select_config_info') as $module) {
    $info_items = array_merge_recursive($info_items, module_invoke($module, 'hierarchical_select_config_info'));
  }

  // Process the retrieved information into options.
  $options = array();
  foreach ($info_items as $id => $item) {
    $config_id = $item['config_id'];
    $options[$config_id] = $item['hierarchy type'] . ': ' . $item['hierarchy'] . ' - ' . $item['context type'] . (!empty($item['context']) ? ': ' . $item['context'] : '');
  }

  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function hierarchical_select_features_export_render($module, $data) {
  module_load_include('inc', 'hierarchical_select', 'includes/common');
  module_load_include('inc', 'hierarchical_select', 'hierarchical_select.admin');

  $code = array();
  $code[] = '$configs = array();';
  foreach ($data as $config_id) {
    $config = hierarchical_select_common_config_get($config_id);
    $config['config_id'] = $config_id;

    $code[] = _hierarchical_select_create_export_code($config);
    $code[] = "\$configs['{$config_id}'] = \$config;";
  }
  $code[] = "return \$configs;";
  $code = implode("\n", $code);

  return array('hierarchical_select_default_configs' => $code);
}

/**
 * Implements hook_features_revert().
 */
function hierarchical_select_features_revert($module) {
  hierarchical_select_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function hierarchical_select_features_rebuild($module) {
  $configs = module_invoke($module, 'hierarchical_select_default_configs');
  if (!empty($configs)) {
    foreach ($configs as $config_id => $config) {
      hierarchical_select_common_config_set($config_id, $config);
    }
  }
}
