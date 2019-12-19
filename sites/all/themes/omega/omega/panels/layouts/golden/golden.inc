<?php

/**
 * @file
 * Defines an asymetric, golden ratio based panels layout.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Golden'),
  'icon' => 'preview.png',
  'category' => t('Omega content layouts'),
  'theme' => 'golden',
  'css' => '../../../css/layouts/golden/golden.layout.css',
  'regions' => array(
    'first' => t('First'),
    'second' => t('Second'),
    'third' => t('Third'),
  ),
);

/**
 * Implements hook_preprocess_golden().
 */
function template_preprocess_golden(&$variables) {
  $variables['attributes_array']['class'][] = 'panel-golden';
  $variables['attributes_array']['class'][] = 'panel-display--golden';

  foreach($variables['content'] as $name => $item) {
    $variables['region_attributes_array'][$name]['class'][] = 'golden-region';
    $variables['region_attributes_array'][$name]['class'][] = 'golden-region--' . drupal_clean_css_identifier($name);
  }
}
