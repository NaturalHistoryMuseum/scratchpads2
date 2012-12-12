<?php

/**
 * Theme function 
 */
function theme_simple_google_chart($variables){
  $id = "google_chart-" . md5(mt_rand());
  // By adding the google_charts code as a library, we can ensure it appears
  // above our own JS on the page.
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
  return '<div class="google_chart_wrapper">' . $variables['title'] . '<div id="' . $id . '" class="google_chart ' . $variables['type'] . '"></div></div>';
}