<?php

/**
 * @file
 * Contains a process hook for 'views_view_list'.
 */

/**
 * Implements hook_process_views_view_list().
 *
 * @see omega_preprocess_views_view_list()
 */
function omega_process_views_view_list(&$variables) {
  foreach ($variables['rows'] as $delta => $item) {
    // Convert the previously set row attributes arrays into strings.
    $variables['row_attributes'][$delta] = !empty($variables['row_attributes_array'][$delta]) ? drupal_attributes($variables['row_attributes_array'][$delta]) : '';
  }
}
