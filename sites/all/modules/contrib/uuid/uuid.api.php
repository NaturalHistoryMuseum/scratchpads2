<?php

/**
 * @file
 * Hooks provided by the UUID module.
 */

/**
 * Defines one or more UUID generators exposed by a module.
 *
 * @return
 *   An associative array with the key being the machine name for the
 *   implementation and the values being an array with the following keys:
 *     - title: The human readable name for the generator.
 *     - callback: The function to be called for generating the UUID.
 *
 * @see uuid_get_info()
 */
function hook_uuid_info() {
  $generators = array();
  $generators['my_module'] = array(
    'title' => t('My module UUID generator'),
    'callback' => 'my_module_generate_uuid',
  );
  return $generators;
}

/**
 * Ensures all records have a UUID assigned to them.
 *
 * When called this hook should ensure all records it is responsible for
 * have a UUID and if not create one.
 *
 * @see entity_uuid_sync()
 */
function hook_uuid_sync() {
  // Do what you need to do to generate missing UUIDs for you implementation.
}

/**
 * Let modules transform their properties with local IDs to UUIDs when an
 * entity is loaded.
 */
function hook_entity_uuid_load(&$entities, $entity_type) {

}

/**
 * Let modules transform their fields with local IDs to UUIDs when an entity
 * is loaded.
 */
function hook_field_uuid_load($entity_type, $entity, $field, $instance, $langcode, &$items) {

}

/**
 * Let modules transform their properties with UUIDs to local IDs when an
 * entity is saved.
 */
function hook_entity_uuid_presave(&$entity, $entity_type) {

}

/**
 * Let modules transform their fields with UUIDs to local IDs when an entity
 * is saved.
 */
function hook_field_uuid_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

}
