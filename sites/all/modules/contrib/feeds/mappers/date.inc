<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for date
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets().
 *
 * @todo Only provides "end date" target if field allows it.
 */
function date_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  if (isset($info['fields']) && count($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if (in_array($field['type'], array('date', 'datestamp', 'datetime'))) {
        $name = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
        $targets[$field_name .':start'] = array(
          'name' => t('@name: Start', array('@name' => $name)),
          'callback' => 'date_feeds_set_target',
          'description' => t('The start date for the @name field. Also use if mapping both start and end.', array('@name' => $name)),
          'real_target' => $field_name,
        );
        $targets[$field_name .':end'] = array(
          'name' => t('@name: End', array('@name' => $name)),
          'callback' => 'date_feeds_set_target',
          'description' => t('The end date for the @name field.', array('@name' => $name)),
          'real_target' => $field_name,
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
 * @param $feed_element
 *   The value to be mapped. Should be either a (flexible) date string
 *   or a FeedsDateTimeElement object.
 *
 * @todo Support array of values for dates.
 */
function date_feeds_set_target($node, $target, $feed_element) {
  list($field_name, $sub_field) = explode(':', $target);
  if (!($feed_element instanceof FeedsDateTimeElement)) {
    if (is_array($feed_element)) {
      $feed_element = $feed_element[0];
    }

    // Empty string means no time - so do not set any!
    if (!is_numeric($feed_element) && !strtotime($feed_element)) {
      return ;
    }

    if ($sub_field == 'end') {
      $feed_element = new FeedsDateTimeElement(NULL, $feed_element);
    }
    else {
      $feed_element = new FeedsDateTimeElement($feed_element, NULL);
    }
  }
  $feed_element->buildDateField($node, $field_name);
}
