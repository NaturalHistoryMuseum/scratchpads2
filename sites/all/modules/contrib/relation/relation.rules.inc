<?php

/**
 * @file
 * Implements the Rules module API for Relation.
 */

/**
 * Callback for creating a relation, in the form Rules wants it.
 */
function relation_rules_create($values = array()) {
  return relation_create($values['relation_type'], array());
}

/**
 * Access callback for creating a relation. For now, everyone has permission to
 * trigger the creation of a relation.
 */
function relation_rules_access($op, $entity = NULL, $account = NULL) {
  return TRUE;
}

/**
 * Endpoint property getter callback.
 */
function relation_rules_get_endpoints($relation, array $options, $property_name, $entity_type) {
  $array = array();
  foreach ($relation->endpoints[LANGUAGE_NONE] as $endpoint) {
    $entity = reset(entity_load($endpoint['entity_type'], array($endpoint['entity_id'])));
    $array[] = entity_metadata_wrapper($endpoint['entity_type'], $entity);
  }
  return $array;
}

/**
 * Endpoint property setter callback.
 *
 * @param $data
 *   The relation object that we are going to modify.
 * @param $name
 *   Name of the provided Rules variable.
 * @param $endpoint_wrappers
 *   Array of entity wrappers that we are going to add to the relation object.
 */
function relation_rules_set_endpoints(&$relation = NULL, $name = NULL, $entity_wrappers = NULL) {
  // Check that we are creating a new relation. Updating existing relations
  // aren't supported.
  if (isset($relation->rid) || empty($entity_wrappers)) {
    return;
  }

  foreach ($entity_wrappers as $i => $entity_wrapper) {
    $entity = $entity_wrapper->value();
    $entity_type = $entity_wrapper->type();
    $id_key = $entity_wrapper->entityKey('id');
    $bundle_key = $entity_wrapper->entityKey('bundle');

    if (isset($entity->{$id_key})) {
      $relation->endpoints[LANGUAGE_NONE][$i] = array(
        'entity_type' => $entity_wrapper->type(),
        'entity_id' => $entity->{$id_key},
        'entity_bundle' => isset($entity->{$bundle_key}) ? $entity->{$bundle_key} : $entity_type,
        'r_index' => $i,
      );
    }
  }
}
