<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for number.module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsProcessor::getMappingTargets()
 */
function number_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  $numeric_types = array(
    'list_integer',
    'list_float',
    'list_boolean',
    'number_integer',
    'number_decimal',
    'number_float',
  );
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (in_array($info['type'], $numeric_types)) {
      $targets[$name] = array(
        'name' => check_plain($instance['label']),
        'callback' => 'number_feeds_set_target',
        'description' => t('The @label field of the entity.', array('@label' => $instance['label'])),
        'summary_callback' => 'feeds_mapper_summary_language',
        'form_callback' => 'feeds_mapper_form_language',
      );
    }
  }
}

/**
 * Callback for mapping numerics.
 *
 * Ensure that $value is a numeric to avoid database errors.
 */
function number_feeds_set_target($source, $entity, $target, array $values, $mapping = array()) {
  // Set the language of the field depending on the mapping configuration.
  $langcode = isset($mapping['language']) ? $mapping['language'] : LANGUAGE_NONE;

  // Iterate over all values.
  $field = isset($entity->$target) ? $entity->$target : array($langcode => array());

  // If it's a new language we need to add it to the field
  if (!isset($field[$langcode])) {
    $field[$langcode] = array();
  }

  foreach ($values as $value) {

    if (is_object($value) && ($value instanceof FeedsElement)) {
      $value = $value->getValue();
    }

    if (is_numeric($value)) {
      $field[$langcode][] = array('value' => $value);
    }
  }

  $entity->$target = $field;
}
