<?php
/**
 * @file
 * uuid_node hooks on behalf of the userreference module.
 */

/**
 * Implements hook_uuid_node_features_export_render().
 */
function userreference_uuid_node_features_export_render(&$export, &$pipe, $node) {
  // TODO: add user UUID's to pipe.
}

/**
 * Implements hook_uuid_node_features_export_render_alter().
 */
function userreference_uuid_node_features_export_render_alter(&$export, $node, $module) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK userreference fields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'userreference') {
        $field_name = $field['field_name'];
        $export->$field_name = array();

        // TODO: Use user UUID's instead.

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
