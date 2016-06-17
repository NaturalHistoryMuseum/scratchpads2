<?php
/**
 * @file
 * uuid_node hooks on behalf of the filefield module.
 */

/**
 * Implements hook_uuid_node_features_export_alter().
 */
function filefield_uuid_node_features_export_alter(&$export, &$pipe, $node) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK filefields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'filefield') {
        $field_name = $field['field_name'];

        // If the content type has changed since the last export, this field
        // may not exist.
        if (isset($node->$field_name)) {
          // Loop through all values of the field.
          foreach ($node->$field_name as $delta => $data) {
            if (!empty($data['fid'])) {
              $uuid = uuid_get_uuid('files', 'fid', $data['fid']);
              // If the referenced file doesn't have a uuid, take this opportunity to
              // create one.
              if (empty($uuid)) {
                $uuid = uuid_set_uuid('files', 'fid', $data['fid']);
              }
              $pipe['uuid_file'][$uuid] = $uuid;
            }
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_node_features_export_render_alter().
 */
function filefield_uuid_node_features_export_render_alter(&$export, $node, $module) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK filefields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'filefield') {
        $field_name = $field['field_name'];
        $export->$field_name = array();

        // Loop through all values of the field.
        foreach ($node->$field_name as $delta => $data) {
          if (!empty($data['fid'])) {
            $export->{$field_name}[$delta] = array(
              'uuid' => uuid_get_uuid('files', 'fid', $data['fid']),
              'list' => $data['list'],
              'data' => $data['data'],
            );
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_node_features_rebuild_alter().
 * Replace filefield uuid's with a field array suitable for node_save().
 */
function filefield_uuid_node_features_rebuild_alter(&$node, $module) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK nodereference fields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'filefield') {
        $field_name = $field['field_name'];

        // Loop through all values of the field.
        foreach ($node->$field_name as $delta => $data) {
          $fid = uuid_get_serial_id('files', 'fid', $data['uuid']);
          $file = field_file_load($fid);

          $node->{$field_name}[$delta] = $file + $data;
        }
      }
    }
  }
}
