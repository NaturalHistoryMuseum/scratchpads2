<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for text.module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsProcessor::getMappingTargets()
 */
function text_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  $text_types = array(
    'list_text',
    'text',
    'text_long',
    'text_with_summary',
  );
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (in_array($info['type'], $text_types)) {
      $targets[$name] = array(
        'name' => check_plain($instance['label']),
        'callback' => 'text_feeds_set_target',
        'description' => t('The @label field of the entity.', array('@label' => $instance['label'])),
      );
    }
  }
}

/**
 * Callback for mapping text fields.
 */
function text_feeds_set_target($source, $entity, $target, $value) {
  if (is_string($value) && !strlen(trim($value))) {
    return;
  }

  if (!is_array($value)) {
    $value = array($value);
  }

  if (isset($source->importer->processor->config['input_format'])) {
    $format = $source->importer->processor->config['input_format'];
  }

  $info = field_info_field($target);

  // List fields store machine values, while legacy imports commonly contain
  // either their labels or differently-cased values (for example Holotype
  // rather than holotype). Resolve those inputs before field validation.
  $allowed_values = array();
  if ($info['type'] == 'list_text') {
    $allowed_values = list_allowed_values($info);
  }

  // Iterate over all values.
  $field = isset($entity->$target) ? $entity->$target : array('und' => array());

  // Allow for multiple mappings to the same target.
  $delta = count($field['und']);

  foreach ($value as $v) {

    if ($info['cardinality'] == $delta) {
      break;
    }

    if (is_object($v) && ($v instanceof FeedsElement)) {
      $v = $v->getValue();
    }

    if (is_scalar($v)) {
      $v = is_string($v) ? trim($v) : $v;
      if ($allowed_values && !isset($allowed_values[$v])) {
        foreach ($allowed_values as $allowed_value => $allowed_label) {
          if (strcasecmp($v, $allowed_value) === 0 || strcasecmp($v, $allowed_label) === 0) {
            $v = $allowed_value;
            break;
          }
        }
      }
      $field['und'][$delta]['value'] = $v;

      if (isset($format)) {
        $field['und'][$delta]['format'] = $format;
      }

      $delta++;
    }
  }

  $entity->$target = $field;
}
