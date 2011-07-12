<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for filefield.module (CCK).
 *
 * @todo Support <em>list</em> subfields (how to provide default value?)
 * @todo Support <em>data</em> subfields (or its own subfieds?)
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter()
 */
function filefield_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  $fields = array();
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if ($field['type'] == 'filefield') {
        $name = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
        $targets[$field_name] = array(
          'name' => check_plain($name),
          'callback' => 'filefield_feeds_set_target',
          'description' => t('The URL for the CCK @name field of the node.', array('@name' => $name)),
        );
      }
    }
  }
}

/**
 * Implementation of hook_feeds_set_target().
 *
 * @param $node
 *   The target node.
 * @param $field_name
 *   The name of field on the target node to map to.
 * @param $value
 *   The value to be mapped. Should contain a URL or an array of URLs; a
 *   FeedsEnclosure or an array of FeedsEnclosures.
 *
 * @todo: should we support $object->url again?
 */
function filefield_feeds_set_target($node, $field_name, $value) {
  // Normalize $value, create an array of FeedsEnclosures of it.
  $enclosures = array();
  if (!is_array($value)) {
    $value = array($value);
  }
  foreach ($value as $k => $v) {
    if ($v instanceof FeedsEnclosure) {
      $enclosures[] = $v;
    }
    elseif (valid_url($v)) {
      $enclosures[$k] = new FeedsEnclosure($v, 'application/octet-stream');
    }
  }

  // Map enclosures.
  $items = isset($node->$field_name) ? $node->$field_name : array();
  foreach ($enclosures as $enclosure) {
    if ($file = $enclosure->getFile()) {
      $field = content_fields($field_name, $node->type);
      $target_dir = filefield_widget_file_path($field, user_load($node->uid));
      $info = field_file_save_file($enclosure->getFile(), array(), $target_dir);
      if ($info) {
        $info['list'] = array();
        $info['data'] = array('description' => '');
        if ($field['list_field']) {
          $info['list'] = $field['list_default'];
        }
        $items[] = $info;
        $error = FALSE;
      }
    }
  }
  $node->$field_name = $items;
}
