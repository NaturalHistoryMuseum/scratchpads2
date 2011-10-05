<?php

/**
 * Simple theme function
 */
function theme_gm3_map($variables){
  gm3_add($variables['map']);
  $variables['map'] = array_merge_recursive($variables['map'], array(
    'id' => 'gm3-map' // Could we get this dynamically from hook_theme?
  ));
  return '<div id="' . $variables['map']['id'] . '"></div>';
}

/**
 * Callback to add a Google map to a specific element ID on the page.
 */
function gm3_add($map = array()){
  $map['settings'] = array_merge(isset($map['settings']) ? $map['settings'] : array(), gm3_settings());
  foreach(module_implements('gm3_map_alter') as $module){
    $map = call_user_func($module . '_gm3_map_alter', $map);
  }
  if(isset($map['libraries']) && is_array($map['libraries'])){
    foreach($map['libraries'] as $library => $settings){
      $module = 'gm3';
      if(is_array($settings)){
        $module = isset($settings['module']) ? $settings['module'] : $module;
      }else{
        $map['libraries'][$settings] = $settings;
        unset($map['libraries'][$library]);
        $library = $settings;
      }
      $library = ($module == 'gm3') ? "gm3.$library" : $library;
      drupal_add_library($module, $library);
    }
  }
  drupal_add_library('gm3', 'gm3');
  drupal_add_js(array(
    'gm3' => array(
      'maps' => array(
        (isset($map['id']) ? $map['id'] : 'gm3-map') => $map
      )
    )
  ), 'setting');
}