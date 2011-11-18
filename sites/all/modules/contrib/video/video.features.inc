<?php

// include the preset file
module_load_include('inc', 'video', 'modules/video_ui/video.preset');

/**
 * Implementation of hook_features_export_options().
 */
function video_features_export_options() {
  $options = array();
  foreach (video_preset_get_presets () as $preset) {
    $options[$preset['name']] = $preset['name'];
  }
  return $options;
}

/**
 * Implementation of hook_features_export().
 */
function video_features_export($data, &$export, $module_name = '') {
  foreach ($data as $preset) {
    $export['features']['video'][$preset] = $preset;
  }
}

/**
 * Implementation of hook_features_export_render().
 */
function video_features_export_render($module_name, $data) {
  $items = array();
  foreach ($data as $key) {
    $preset = video_get_preset($key);
    unset($preset['pid']);
    $items[$key] = $preset;
  }
  $code = "  \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';
  return array('video_default_presets' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function video_features_revert($module) {
  if ($default_presets = features_get_default('video', $module)) {
    foreach (array_keys($default_presets) as $default_preset) {
      video_preset_delete($default_preset);
    }
  }
}
