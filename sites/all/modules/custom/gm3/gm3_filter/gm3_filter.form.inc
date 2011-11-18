<?php

/**
 * Function to return the form!
 */
function gm3_filter_helper_form(){
  // Firstly we need to build a map array.
  $map = array(
    'libraries' => array(),
    'tools' => array()
  );
  $libraries_to_use = array(
    'point',
    'polygon',
    'polyline',
    'rectangle',
    'region'
  );
  foreach($libraries_to_use as $library){
    $map['libraries'][] = $library;
    $map['libraries'][] = "field_$library";
    $map['tools'][] = theme("gm3_{$library}_button");
  }
  return array(
    'map' => array(
      '#markup' => drupal_render(theme('gm3_map', array(
        'map' => $map
      )))
    )
  );
}