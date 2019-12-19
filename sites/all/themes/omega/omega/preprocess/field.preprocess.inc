<?php

/**
 * @file
 * Contains a pre-process hook for 'field'.
 */

/**
 * Implements hook_preprocess_block().
 */
function omega_preprocess_field(&$variables) {
  // Change field classes to use BEM syntax.
  $variables['classes_array'] = preg_replace('/^field-(?!-)/', 'field--', $variables['classes_array']);
  $variables['label_display_css'] = drupal_clean_css_identifier($variables['element']['#label_display']);
}
