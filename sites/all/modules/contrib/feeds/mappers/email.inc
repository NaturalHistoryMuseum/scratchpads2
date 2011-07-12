<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for email.module (CCK).
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 */
function email_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  $fields = array();
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if (in_array($field['type'], array('email'))) {
        $fields[$field_name] = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
      }
    }
  }
  foreach ($fields as $k => $name) {
    $targets[$k] = array(
      'name' => check_plain($name),
      'callback' => 'email_feeds_set_target',
      'description' => t('The CCK Email @name field of the node.', array('@name' => $name)),
    );
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * When the callback is invoked, $target contains the name of the field the
 * user has decided to map to and $value contains the value of the feed item
 * element the user has picked as a source.
 */
function email_feeds_set_target($node, $target, $value) {

  $field = isset($node->$target) ? $node->$target : array();

  // Handle multiple value fields.
  if (is_array($value)) {
    $i = 0;
    foreach ($value as $v) {
      if (!is_array($v) && !is_object($v)) {
        $field[$i]['email'] = $v;
      }
      $i++;
    }
  }
  else {
    $field[0]['email'] = $value;
  }

  $node->$target = $field;
}
