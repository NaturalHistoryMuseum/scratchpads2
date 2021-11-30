<?php

/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Implements hook_feeds_processor_targets().
 */
function feeds_test_field_feeds_processor_targets($entity_type, $bundle_name) {
  $targets = array();

  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if (in_array($info['type'], array('feeds_test_field'))) {
      $targets[$name] = array(
        'name' => $instance['label'],
        'callback' => 'feeds_test_field_feeds_set_target',
        'description' => t('The @label field of the node.', array('@label' => $instance['label'])),
      );
    }
  }

  return $targets;
}

/**
 * Implements callback_my_module_set_target().
 */
function feeds_test_field_feeds_set_target(FeedsSource $source, $entity, $target, array $values, array $mapping) {
  $language = $mapping['language'];

  // Iterate over all values.
  $field = isset($entity->$target) ? $entity->$target : array($language => array());

  foreach ($values as $value) {
    $field[$language][] = array('value' => $value);
  }

  $entity->$target = $field;
}
