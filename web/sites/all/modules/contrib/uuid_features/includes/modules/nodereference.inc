<?php
/**
 * @file
 * uuid_node hooks on behalf of the nodereference module.
 */

/**
 * Implements hook_uuid_node_features_export_render().
 */
function nodereference_uuid_node_features_export_alter(&$export, &$pipe, $node) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK nodereference fields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'nodereference') {
        $field_name = $field['field_name'];

        // If the content type has changed since the last export, this field
        // may not exist.
        if (isset($node->$field_name)) {
          // Loop through all values of the field.
          foreach ($node->$field_name as $delta => $data) {
            if (!empty($data['nid'])) {
              $uuid = uuid_get_uuid('node', 'nid', $data['nid']);
              // If the referenced node doesn't have a uuid, take this opportunity to
              // create one.
              if (empty($uuid)) {
                $uuid = uuid_set_uuid('node', 'nid', $data['nid']);
              }
              $pipe['uuid_node'][$uuid] = $uuid;
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
function nodereference_uuid_node_features_export_render_alter(&$export, $node, $module) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK nodereference fields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'nodereference') {
        $field_name = $field['field_name'];
        $export->$field_name = array();

        // Loop through all values of the field.
        foreach ($node->$field_name as $delta => $data) {
          if (!empty($data['nid'])) {
            $uuid = uuid_get_uuid('node', 'nid', $data['nid']);
            $export->{$field_name}[$delta] = $uuid;
          }
        }
      }
    }
  }
}

/**
 * Implements hook_uuid_node_features_rebuild_alter().
 * Replace noderef uuid's with a field array suitable for node_save().
 */
function nodereference_uuid_node_features_rebuild_alter(&$node, $module) {
  $types = content_types();
  if (!empty($types[$node->type])) {
    // Find CCK nodereference fields.
    foreach ($types[$node->type]['fields'] as $field) {
      if ($field['module'] == 'nodereference') {
        $field_name = $field['field_name'];

        // Loop through all values of the field.
        foreach ($node->$field_name as $delta => $uuid) {
          $nid = uuid_get_serial_id('node', 'nid', $uuid);
          $node->{$field_name}[$delta] = array('nid' => $nid);
        }
      }
    }
  }
}
