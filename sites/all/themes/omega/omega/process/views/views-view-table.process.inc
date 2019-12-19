<?php

/**
 * @file
 * Contains a process hook for 'views_view_table'.
 */

/**
 * Implements hook_process_views_view_table().
 *
 * @see hook_preprocess_views_view_table()
 */
function omega_process_views_view_table(&$variables) {
  foreach ($variables['rows'] as $delta => $row) {
    // Convert the previously set row attributes arrays into strings.
    $variables['row_attributes'][$delta] = !empty($variables['row_attributes_array'][$delta]) ? drupal_attributes($variables['row_attributes_array'][$delta]) : '';

    foreach ($row as $field => $content) {
      // Convert the previously set field attributes arrays into strings.
      $variables['field_attributes'][$field][$delta] = !empty($variables['field_attributes_array'][$field][$delta]) ? drupal_attributes($variables['field_attributes_array'][$field][$delta]) : '';
    }
  }

  foreach ($variables['header'] as $field => $label) {
    // Convert the previously set header attributes arrays into strings.
    $variables['header_attributes'][$field] = !empty($variables['header_attributes_array'][$field]) ? drupal_attributes($variables['header_attributes_array'][$field]) : '';
  }
}
