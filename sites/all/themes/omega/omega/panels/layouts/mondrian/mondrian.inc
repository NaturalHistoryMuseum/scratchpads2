<?php

/**
 * @file
 * Defines an asymetric Mondrian inspired panels layout.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Mondrian'),
  'icon' => 'preview.png',
  'category' => t('Omega content layouts'),
  'theme' => 'mondrian',
  'css' => '../../../css/layouts/mondrian/mondrian.layout.css',
  'regions' => array(
    'main' => t('Main'),
    'secondary' => t('Secondary'),
    'tertiary-first' => t('Tertiary First'),
    'tertiary-second' => t('Tertiary Second'),
    'tertiary-third' => t('Tertiary Third'),
  ),
);

/**
 * Implements hook_preprocess_mondrian().
 */
function template_preprocess_mondrian(&$variables) {
  $variables['attributes_array']['class'][] = 'panel-display';
  $variables['attributes_array']['class'][] = 'panel-display--mondrian';

  foreach($variables['content'] as $name => $item) {
    $variables['region_attributes_array'][$name]['class'][] = 'mondrian-region';
    $variables['region_attributes_array'][$name]['class'][] = 'mondrian-region--' . drupal_clean_css_identifier($name);
  }
}
