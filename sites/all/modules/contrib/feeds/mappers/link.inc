<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for link.module (CCK).
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 */
function link_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);

  $fields = array();
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {

      if (in_array($field['type'], array('link'))) {
        $name = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
        $targets[$field_name .':url'] = array(
          'name' => t('@field_name (URL)', array('@field_name' => $name)),
          'callback' => 'link_feeds_set_target',
          'description' => t('The URL for the CCK @name field of the node.', array('@name' => $name)),
          'real_target' => $field_name,
        );

        //Provides a mapping target for the field title if used.
        if (in_array($field['title'], array('optional', 'required'))) {
          $targets[$field_name .':title'] = array(
            'name' => t('@name (title)', array('@name' => $name)),
            'callback' => 'link_feeds_set_target',
            'description' => t('The title for the CCK @name field of the node.', array('@name' => $name)),
            'real_target' => $field_name,
          );
        }
      }
    }
  }
}

/**
 * Callback for mapping to link field.
 *
 * @param $node
 *   Reference to the node object we are working on.
 * @param $target
 *   The selected link CCK field.
 * @param $value
 *   The value to assign to the CCK field.
 */
function link_feeds_set_target($node, $target, $value) {
  module_load_include('inc', 'link');
  if (!empty($value)) {
    static $defaults = array();
    list($field_name, $sub_field) = explode(':', $target);

    if (!isset($defaults[$node->type][$field_name])) {
      $field = content_fields($field_name, $node->type);
      $defaults[$node->type][$field_name]['attributes'] = $field['attributes'];
      if (!in_array($field['title'], array('optional', 'required', 'none'))) {
        $defaults[$node->type][$field_name]['title'] = $field['title_value'];
      }
    }
    $field_data = isset($node->$field_name) ? $node->$field_name : array();

    if (!is_array($value)) {
      $value = array($value);
    }

    $i = 0;
    foreach ($value as $v) {
      if ($v instanceof FeedsEnclosure) {
        $v = $v->getValue();
      }
      if (!isset($field_data[$i])) {
        $field_data[$i] = $defaults[$node->type][$field_name];
      }
      if ($sub_field != 'url' || (($v = link_cleanup_url($v)) && valid_url($v, TRUE))) {
        $field_data[$i][$sub_field] = $v;
      }
      $i++;
    }

    $node->$field_name = $field_data;
  }
}
