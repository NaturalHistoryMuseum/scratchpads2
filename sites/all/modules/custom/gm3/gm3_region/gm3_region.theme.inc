<?php

/**
 * Region tools button
 */
function theme_gm3_region_button($variables){
  theme('gm3_beautytip', array(
    'selector' => '.gm3-tools-button[data-gm3-class="region"]',
    'text' => t('Click to enable selecting regions from the map.<br/>Add regions by clicking on the map, remove by right clicking on a highlighted region.<br/>The size of the region being selected is depended on your zoom level.  Zoom in to select smaller regions, zoom out to select larger ones, or click the "Selecting by..." text at the top of the map.')
  ));
  return '<button type="button" data-gm3-class="region" class="gm3-tools-button"><p>+ ' . t('Region') . '</p></button>';
}

/**
 * Themes for field types.
 */
function theme_gm3_region_text($variables){
  $parts = explode(':', $variables['data']['region_id']);
  return array(
    '#markup' => t('TDWG:@region_id', array(
      '@region_id' => end($parts)
    ))
  );
}

/**
 * Implements hook_gm3_field_theme
 */
function gm3_region_gm3_field_theme($name, $vars){
  switch ($name) {
    case 'region_text':
    case 'gm3_region_region_text':
      return theme('gm3_region_text', $vars);
  }
}