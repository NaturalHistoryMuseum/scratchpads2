<?php

/**
 * @file
 * Contains a process hook for 'views_view_grid'.
 */

/**
 * Implements hook_process_views_view_grid().
 *
 * @see hook_preprocess_views_view_grid()
 */
function omega_process_views_view_grid(&$variables) {
  $options = $variables['view']->style_plugin->options;
  $columns = $options['columns'];
  foreach ($variables['rows'] as $delta => $row) {
    // Convert the previously set row attributes arrays into strings.
    $variables['row_attributes'][$delta] = !empty($variables['row_attributes_array'][$delta]) ? drupal_attributes($variables['row_attributes_array'][$delta]) : '';

    for ($column = 0; $column < $columns; $column++) {
      // Convert the previously set column attributes arrays into strings.
      $variables['column_attributes'][$delta][$column] = !empty($variables['column_attributes_array'][$delta][$column]) ? drupal_attributes($variables['column_attributes_array'][$delta][$column]) : '';
    }
  }
}
