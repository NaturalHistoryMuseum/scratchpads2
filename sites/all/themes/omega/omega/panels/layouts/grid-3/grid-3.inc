<?php

/**
 * @file
 * Defines a simple 2-column grid layout.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Grid (3)'),
  'icon' => 'preview.png',
  'category' => t('Omega content layouts'),
  'theme' => 'grid_3',
  'css' => '../../../css/layouts/grid-3/grid-3.layout.css',
  'regions' => array(
    'first' => t('First'),
    'second' => t('Second'),
    'third' => t('Third'),
    'fourth' => t('Fourth'),
    'fifth' => t('Fifth'),
    'sixth' => t('Sixth'),
  ),
);

/**
 * Implements hook_preprocess_grid_3().
 */
function template_preprocess_grid_3(&$variables) {
  $variables['attributes_array']['class'][] = 'panel-display';
  $variables['attributes_array']['class'][] = 'panel-display--grid-3';

  foreach($variables['content'] as $name => $item) {
    $variables['region_attributes_array'][$name]['class'][] = 'grid-3-region';
    $variables['region_attributes_array'][$name]['class'][] = 'grid-3-region--' . drupal_clean_css_identifier($name);
  }
}
