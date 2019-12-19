<?php

/**
 * @file
 * Contains a pre-process hook for 'views_view_grid'.
 */

/**
 * Implements hook_preprocess_views_view_grid().
 */
function omega_preprocess_views_view_grid(&$variables) {
  // For some reason views puts row classes into the classes array. Instead of
  // classes arrays we should always use proper attributes arrays and never
  // abuse the default versions of those for row classes. Instead, we should use
  // a custom variable for that, which is exactly what we convert it to here.
  $options = $variables['view']->style_plugin->options;
  $columns = $options['columns'];
  foreach ($variables['rows'] as $delta => $row) {
    $variables['row_attributes_array'][$delta] = isset($variables['row_attributes_array'][$delta]) ? $variables['row_attributes_array'][$delta] : array();

    if (!empty($variables['row_classes'][$delta])) {
      $variables['row_attributes_array'][$delta]['class'] = explode(' ', $variables['row_classes'][$delta]);
    }

    // Views tables have additional classes for each column.
    for ($column = 0; $column < $columns; $column++) {
      $variables['column_attributes_array'][$delta][$column] = isset($variables['column_attributes_array'][$delta][$column]) ? $variables['column_attributes_array'][$delta][$column] : array();

      if (!empty($variables['column_classes'][$delta][$column])) {
        $variables['column_attributes_array'][$delta][$column]['class'] = explode(' ', $variables['column_classes'][$delta][$column]);
      }
    }
  }
}
