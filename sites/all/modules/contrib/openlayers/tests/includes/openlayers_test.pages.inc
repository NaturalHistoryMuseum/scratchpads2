<?php

/**
 * @file
 * Pages for test module
 */

/**
 * Callback for OpenLayers Test Show Maps Page
 */
function openlayers_test_show_maps() {
  $output = '';
  
  // Render all maps
  $maps = openlayers_maps();
  foreach ($maps as $name => $map) {
    $output .= '
      <h3>' . $map->title . '</h3>
      ' . openlayers_render_map($map) . '
    ';
  }

  /*
  // Create collapsed fieldset
  //$render_fieldset = openlayers_render_map();
  $element = array(
    '#value' => $render_fieldset['themed'],
    '#title' => t('Fieldset Example'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $fieldset = theme_fieldset($element);
  */

  return $output;
}

/**
 * Callback for OpenLayers Test JS Page
 */
function openlayers_test_js() {
  drupal_add_js(drupal_get_path('module', 'openlayers_test') . '/js/qunit/qunit.js');
  drupal_add_css(drupal_get_path('module', 'openlayers_test') . '/js/qunit/qunit.css');
  drupal_add_js(drupal_get_path('module', 'openlayers_test') . '/js/openlayers_test.js');
  $output = '';
  
  // Display QUnit stuff
  $output .= '
    <h1 id="qunit-header">OpenLayers QUnit Tests</h1>
    <h2 id="qunit-banner"></h2>
    <h2 id="qunit-userAgent"></h2>
    <ol id="qunit-tests"></ol>
  ';

  // Render only specific maps
  $maps = array('default', 'behaviors_test');
  foreach ($maps as $name => $map) {
    $output .= '
      ' . openlayers_render_map($map) . '
    ';
  }

  return $output;
}