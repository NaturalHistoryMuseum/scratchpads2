<?php

/**
 * Theme function 
 */
function theme_simple_google_chart($variables){
  $id = "google_chart-" . md5(mt_rand());
  drupal_add_library('simple_google_charts', 'google_charts');
  drupal_add_js(drupal_get_path('module', 'simple_google_charts') . '/js/simple_google_charts.js');
  drupal_add_js(array(
    'scratchpads_statistics' => array(
      $id => array(
        'data' => $variables['data'],
        'type' => $variables['type'],
        'options' => $variables['options']
      )
    )
  ), 'setting');
  if($variables['title']){
    $variables['title'] = "<h2>{$variables['title']}</h2>";
  }
  if($variables['subtitle']){
    $variables['title'] .= "<h3>{$variables['subtitle']}</h3>";
  }
  $table_data = $variables['data'];
  $table_header = array_shift($table_data);
  return '<div class="google_chart_wrapper">' . $variables['title'] . '<div id="' . $id . '" class="google_chart ' . $variables['type'] . '"><noscript>' . theme('table', array(
    'header' => $table_header,
    'rows' => $table_data
  )) . '</noscript></div></div>';
}

/**
 * Similar theme function to the above, but this time creating a google.charts
 * chart, rather than a google.visualization
 */
function theme_simple_google_material_chart($variables){
  $id = "google_chart-" . md5(mt_rand());
  drupal_add_library('simple_google_charts', 'google_charts');
  drupal_add_js(drupal_get_path('module', 'simple_google_charts') . '/js/simple_google_material_charts.js');
  drupal_add_js(array(
    'scratchpads_statistics' => array(
      $id => array(
        'data' => $variables['data'],
        'type' => $variables['type'],
        'options' => $variables['options']
      )
    )
  ), 'setting');
  if($variables['title']){
    $variables['title'] = "<h2>{$variables['title']}</h2>";
  }
  if($variables['subtitle']){
    $variables['title'] .= "<h3>{$variables['subtitle']}</h3>";
  }
  $table_data = $variables['data'];
  $table_header = array_shift($table_data);
  return '<div class="google_material_chart_wrapper">' . $variables['title'] . '<div id="' . $id . '" class="google_material_chart ' . $variables['type'] . '"><noscript>' . theme('table', array(
    'header' => $table_header,
    'rows' => $table_data
  )) . '</noscript></div></div>';
}