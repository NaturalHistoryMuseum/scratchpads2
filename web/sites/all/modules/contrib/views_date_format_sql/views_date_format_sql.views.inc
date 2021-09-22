<?php
/**
 * @file
 * This file contains the hook to assign this handler as default to node 'created' and 'changed' date fields.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_date_format_sql_views_data_alter(&$data) {
  // loops through fields definitions looking for date fields
  // and change the standard date handler with our own.
  foreach($data as $module => &$table) {
    foreach($table as $id => &$field) {
      if (is_array($field) &&
          array_key_exists('field', $field) &&
          is_array($field['field']) &&
          array_key_exists('handler', $field['field']) &&
         ($field['field']['handler'] == 'views_handler_field_date')) {
        $field['field']['handler'] = 'views_date_format_sql_handler_date';
      }
    }
  }
}

/**
 * Implements hook_field_views_data_alter().
 */
function views_date_format_sql_field_views_data_alter(&$result, $field, $module) {
  if ($module == 'date') {
    foreach ($result as $table => $data) {
      foreach ($data as $column => $value) {
        // The old 'entity_id' and 'revision_id' values got rewritten in Views.
        // The old values are still there with a 'moved to' key, so ignore them.
        if (array_key_exists('field', $value) && !array_key_exists('moved to', $value['field'])) {
          $result[$table][$column]['field']['handler'] = 'views_date_format_sql_handler_date_field';
          $result[$table][$column]['field']['add fields to query'] = TRUE;
        }
      }
    }
  }
}
