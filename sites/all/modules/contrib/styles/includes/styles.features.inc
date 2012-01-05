<?php

/**
 * Implements hook_features_export_options().
 */
function styles_style_features_export_options() {
  $options = array();
  foreach (styles_default_styles() as $name => $style) {
    foreach ($style['styles'] as $style_name => $style_instance) {
      // We need to know the field type and the actual machine name here,
      // so we add both to the name, and separate them with :
      $options[$name . ':' . $style_name] = $name . ': ' . (!empty($style_instance['label']) ? $style_instance['label'] : $style_instance['name']);
    }
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function styles_style_features_export($data, &$export, $module_name = '') {
  $styles = styles_default_styles();
  foreach ($data as $style_data) {
    list($type, $name) = explode(':', $style_data);
    $style = $styles[$type]['styles'][$name];
    if (isset($style)) {
      // Add a dependency if this style is provided by another module.
      if (isset($style['module']) && $style['module'] != $module_name) {
        $export['dependencies'][$module] = $style['module'];
      }
      else {
        $export['features']['styles_style'][$style_data] = $style_data;
      }
    }
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function styles_style_features_export_render($module_name, $data, $export = NULL) {
  $styles = styles_default_styles();
  $presets = styles_default_presets();
  // hook_styles_default_styles().
  $styles_code = array();
  $styles_code[] = '  $styles = array();';
  $styles_code[] = '';
  // hook_styles_default_presets_alter().
  $presets_code = array();
  // The presets alter needs some info about our styles, see below.
  $presets_code[] = "  \$styles = styles_default_styles();";
  foreach ($data as $style) {
    list($type, $name) = explode(':', $style);
    $style = $styles[$type]['styles'][$name];
    $style_export = array(
      'label' => !empty($style['label']) ? $style['label'] : $name,
      'description' => $style['description'],
    );
    $styles_code[] = "  // Exported style: {$name}";
    $styles_code[] = "";
    // In order for this export to be useful, we need to add the default preset
    // and the current preset to the export. We generate the code necessary here.
    $presets_code[] = "  if (\$styles['$type']['styles']['$name']['storage'] == STYLES_STORAGE_DEFAULT) {";
    foreach ($presets[$type]['containers'] as $container_name => $container) {
      if (isset($container['styles'][$name])) {
        // This code will only be added when exporting the feature.
        if (isset($export)) {
          $preset_export = features_var_export($container['styles'][$name], "    ");
          $presets_code[] = "    \$presets['$type']['containers']['$container_name']['styles']['$name'] = {$preset_export};";
          $presets_code[] = '';
        }
        // We need to add the preset info to the default hook,
        // so that Features can compare the data in a proper way.
        // This is needed so that features can compare the database and the
        // code versions.
        $style_export['preset_info'][$container_name][$name] = $container['styles'][$name];
      }
    }
    $presets_code[] = '  }';
    $style_export = features_var_export($style_export, "  ");
    $styles_code[] = "  \$styles['{$style['field_type']}']['styles']['{$name}'] = $style_export;";
  }
  $styles_code[] = '  return $styles;';
  // Features does not support adding function that has parameters,
  // so we need to do our own little function to get around that.
  // This can't be included when we compare the code though, the code
  // will be run through eval().
  if (isset($export)) {
    $styles_code[] = '}';
    $styles_code[] = '';
    $styles_code[] = "/**";
    $styles_code[] = " * Implements hook_styles_default_presets_alter().";
    $styles_code[] = " */";
    $styles_code[] = "function {$module_name}_styles_default_presets_alter(&\$presets) {";
    $styles_code[] = implode("\n", $presets_code);
  }
  $styles_code = implode("\n", $styles_code);
  return array('styles_default_styles' => $styles_code);
}

/**
 * Implements hook_features_revert().
 */
function styles_style_features_revert($module) {
  if ($default_styles = features_get_default('styles_style', $module)) {
    foreach ($default_styles as $field_type => $styles) {
      foreach ($styles['styles'] as $style_name => $style) {
        if ($style = styles_style_load($field_type, $style_name)) {
          styles_style_delete($field_type, $style_name);
        }
      }
    }
  }
}
