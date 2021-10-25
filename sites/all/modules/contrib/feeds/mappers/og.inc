<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for og.module.
 */

/**
 * Implements hook_feeds_prevalidate().
 *
 * Set the field mode of og reference fields to 'admin' when the 'authorize'
 * option is not checked.
 */
function og_feeds_prevalidate(FeedsSource $source, $entity, $item, $entity_id) {
  // Check if 'authorize' is enabled.
  $processor_config = $source->importer->processor->getConfig();
  if (!empty($processor_config['authorize'])) {
    // Authorize is enabled, so we will not set the 'field_mode'.
    return;
  }

  $entity_type = $entity->feeds_item->entity_type;

  // Not a real entity.
  if (!entity_get_info($entity_type)) {
    return;
  }

  // Gather the fields that Feeds is mapping to.
  $feeds_fields = array();
  foreach ($source->importer()->processor->getMappings() as $mapping) {
    list($field) = explode(':', $mapping['target']);
    $feeds_fields[$field] = TRUE;
  }

  list(, , $bundle) = entity_extract_ids($entity_type, $entity);

  foreach (field_info_instances($entity_type, $bundle) as $instance) {
    $field_name = $instance['field_name'];

    // Skip fields that Feeds isn't mapping to, and empty fields.
    if (!isset($feeds_fields[$field_name]) || empty($entity->$field_name) || !is_array($entity->$field_name)) {
      continue;
    }

    $info = field_info_field($field_name);

    // Check if the field is an entityreference field.
    if ($info['type'] != 'entityreference') {
      // Not an entityreference field. Continue to the next field.
      continue;
    }

    // Check if the field is controlled by og.
    if (empty($info['settings']['handler']) || $info['settings']['handler'] != 'og') {
      // Not an og reference field. Continue to the next field.
      continue;
    }

    // Set 'field_mode' of this og reference to 'admin'.
    foreach ($entity->$field_name as $language => &$values) {
      foreach ($values as &$value) {
        $value['field_mode'] = 'admin';
      }
    }

  }

}
