<?php
/**
 * @file
 * Feeds processor hooks for importing Countries fields using the Feeds module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @param array $targets
 *   Array containing the targets to be offered to the user.
 * @param string $entity_type
 *   The entity type of the target, for instance a 'node' entity.
 * @param string $bundle_name
 *   The bundle name for which to alter targets.
 */
function countries_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $field_name => $instance) {
    $info = field_info_field($field_name);
    if ($info['type'] == 'country') {
      $core_properties = countries_core_properties();
      unset($core_properties['enabled']);
      unset($core_properties['continent']);
      foreach ($core_properties as $key => $label) {
        $targets[$entity_type . ':' . $bundle_name . ':' . $field_name . ':' . $key] = array(
          'name' => check_plain($instance['label']) . ': ' . $label,
          'callback' => '_countries_feeds_set_target',
          'description' => t('The @label field of the node.', array('@label' => $instance['label'])),
        );
      }
    }
  }
}

/**
 * Callback defined via countries_feeds_processor_targets_alter().
 */
function _countries_feeds_set_target($source, $entity, $target, $values) {
  if (empty($values)) {
    return;
  }

  // Handle non-multiple values.
  if (!is_array($values)) {
    $values = array($values);
  }

  // Find the field name and country property keys.
  list($entity_type, $bundle_name, $field_name, $property) = explode(':', $target, 4);
  $field = field_read_field($field_name);
  $settings = $field['settings'];
  $continents = array_filter((array) $settings['continents']);

  // Iterate over all values.
  $i = 0;
  $entity->{$field_name} = isset($entity->$field_name) ? $entity->{$field_name} : array();
  foreach ($values as $value) {
    if ($country = countries_country_lookup($value, $property)) {
      // Check instance settings.
      if ($settings['enabled'] == COUNTRIES_ENABLED && !$country->enabled) {
        continue;
      }
      elseif ($settings['enabled'] == COUNTRIES_DISABLED && $country->enabled) {
        continue;
      }
      if (!empty($continents)) {
        if (!array_key_exists($country->continent, $continents)) {
          continue;
        }
      }

      $entity->{$field_name}[LANGUAGE_NONE][$i]['iso2'] = $country->iso2;
      $i++;
    }
    if ($field['cardinality'] != FIELD_CARDINALITY_UNLIMITED && $i >= $field['cardinality']) {
      break;
    }
  }
  return $entity;
}
