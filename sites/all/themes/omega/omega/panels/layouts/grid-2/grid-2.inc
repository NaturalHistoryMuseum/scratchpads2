<?php

/**
 * @file
 * Defines a simple 2-column grid layout.
 */

// Plugin definition.
$plugin = array(
  'title' => t('Grid (2)'),
  'icon' => 'preview.png',
  'category' => t('Omega content layouts'),
  'theme' => 'grid_2',
  'css' => 'grid-2.css',
  'regions' => array(
    'top_left' => t('Top Left'),
    'top_right' => t('Top Right'),
    'middle_left' => t('Middle Left'),
    'middle_right' => t('Middle Right'),
    'bottom_left' => t('Bottom Left'),
    'bottom_right' => t('Bottom Right'),
  ),
);

/**
 * Implements hook_preprocess_grid_2().
 */
function template_preprocess_grid_2(&$variables) {
  $variables['attributes_array']['class'][] = 'panel-display';
  $variables['attributes_array']['class'][] = 'panel-display--grid-2';
}
