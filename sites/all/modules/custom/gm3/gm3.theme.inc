<?php

/**
 * Simple theme function
 */
function theme_gm3_map($variables){
  $variables['map'] = array_merge_recursive($variables['map'], array(
    'id' => 'gm3-map' // Could we get this dynamically from hook_theme?
  ));
  gm3_add($variables['map']);
  $tools = '';
  if(isset($variables['map']['tools'])){
    $tools = '<div class="gm3-tools"><!-- Apologies to Vizzuality for completely ripping off your ideas! <h1>'.t('Tools').'</h1> --><ul><li>' . implode("</li><li>", $variables['map']['tools']) . '</li></ul></div>';
  }
  return array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'gm3') . '/css/gm3.css'
      )
    ),
    '#markup' => '<div class="gm3-container">' . $tools . '<div class="gm3-map-wrapper"><div id="' . $variables['map']['id'] . '"></div></div></div>'
  );
}

/**
 * Polygon tools button
 */
function theme_gm3_polygon_button($variables){
  return '<div id="'.$variables['id'].'-polygon" class="gm3-tools-button"><p>+ Polygon</p></div>';
}

/**
 * Callback to add a Google map to a specific element ID on the page.
 */
function gm3_add(&$map = array()){
  $map['settings'] = array_merge(isset($map['settings']) ? $map['settings'] : array(), gm3_settings());
  drupal_alter('gm3_map', $map);
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
        $map['id'] => $map
      )
    )
  ), 'setting');
}