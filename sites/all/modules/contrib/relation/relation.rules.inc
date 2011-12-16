<?php

/**
 * @file
 * Implements the Rules module API for Relation.
 */

/**
 * Implements hook_rules_event_info().
 */
function relation_rules_event_info() {
  return array(
    'relation_insert' => array(
      'label' => t('After saving a new relation'),
      'group' => t('Relation'),
      'variables' => array(
        'relation' => array('type' => 'relation', 'label' => t('relation')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function relation_rules_action_info() {
  $items['relation_rules_load_related'] = array(
    'label' => t('Loads related entities'),
    'group' => t('Relation'),
    'parameter' => array(
      'left' => array(
        'type' => 'entity',
        'label' => t('Entity'),
      ),
      'relation_type' => array(
        'type' => 'text',
        'label' => t('Relation type'),
        'options list' => 'relation_rules_get_type_options',
      ),
    ),
    'provides' => array(
      'endpoints' => array(
        'type' => 'list<entity>',
        'label' => t('List of related entities'),
      ),
    ),
  );
  return $items;
}


/**
 * Options list callback for fetching relation types.
 */
function relation_rules_get_type_options() {
  $options = array();
  $types = relation_get_types();
  foreach ($types as $type) {
    $options[$type->relation_type] = $type->label;
  }
  return $options;
}

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
 * Entity-type specific property getter callback.
 */
function relation_rules_get_specific_endpoints($relation, array $options, $property_name, $entity_type, $info) {
  if ($info['endpoint_type'] == 'source') {
    if ($info['relation_directional']) {
      $endpoints = array($relation->endpoints[LANGUAGE_NONE][0]);
    }
    else {
      $endpoints = $relation->endpoints[LANGUAGE_NONE];
    }
  }
  else {
    $endpoints = array_slice($relation->endpoints[LANGUAGE_NONE], 1);
  }

  $array = array();
  foreach ($endpoints as $endpoint) {
    $entity = reset(entity_load($endpoint['entity_type'], array($endpoint['entity_id'])));
    $entity_id = entity_extract_ids($endpoint['entity_type'], $entity);
    $array[] = $entity_id[0];
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

/**
 * Related entities getter callback.
 */
function relation_rules_get_related_entities($entity, array $options, $name, $type, $info) {
  $source_entity = entity_metadata_wrapper($type, $entity);
  $source_entity_type = $source_entity->type();
  $source_entity_id = $source_entity->getIdentifier();
  $rids = array_keys(relation_query($source_entity_type, $source_entity_id)->entityCondition('bundle', $info['relation_type'])->execute());
  $entities_ids = array();
  if (!$rids) {
    return $entities_ids;
  }
  foreach (relation_load_multiple($rids) as $relation) {
    foreach ($relation->endpoints[LANGUAGE_NONE] as $endpoint) {
      if ($endpoint['entity_type'] == $info['target_type']) {
        $entities_ids[] = $endpoint['entity_id'];
      }
    }
  }
  return $entities_ids;
}

/**
* Action callback loading all related entities.
*/
function relation_rules_load_related($source_entity, $relation_type) {
  $endpoints = array();
  $source_entity_type = $source_entity->type();
  $source_entity_id = $source_entity->getIdentifier();
  foreach ($source_entity->getPropertyInfo() as $property_name => $property) {
    if (isset($property['relation_type']) && $property['relation_type'] == $relation_type && isset($property['target_type'])) {
      $related_entities = $source_entity->$property_name->value();
      if (!empty($related_entities)) {
        foreach ($related_entities as $related_entity) {
          $endpoint_wrapper = entity_metadata_wrapper($property['target_type'], $related_entity);
          if ($endpoint_wrapper->type() != $source_entity_type || $endpoint_wrapper->getIdentifier() != $source_entity_id) {
            $endpoints[] = $endpoint_wrapper;
          }
        }
      }
    }
  }
  return array('endpoints' => $endpoints);
}
