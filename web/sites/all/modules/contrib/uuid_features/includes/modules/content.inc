<?php

/**
 * @file
 * uuid_node hooks on behalf of the content module.
 */

/**
 * Implements hook_uuid_node_features_export_render_alter().
 *
 * For most fields, it is enough to simply add the values from each db
 * column into a field array.
 */
function content_uuid_node_features_export_render_alter(&$export, $node, $module) {
  if (!empty($types[$node->type])) {
    // Find CCK text fields.
    foreach ($types[$node->type]['fields'] as $field) {
      // Let field modules do their own thing if they want.
      if (!module_hook($field['module'], 'uuid_node_features_export_render_alter')) {
        $field_name = $field['field_name'];
        $export->$field_name = array();
        // If the content type has changed since the last export, this field
        // may not exist.
        if (isset($node->$field_name)) {
          // Loop through all values of the field.
          foreach ($node->$field_name as $delta => $data) {
            $export->{$field_name}[$delta] = array();

            // Save the value of each column.
            foreach ($field['columns'] as $column => $column_data) {
              $export->{$field_name}[$delta][$column] = $data[$column];
            }
          }
        }
      }
    }
  }
}
