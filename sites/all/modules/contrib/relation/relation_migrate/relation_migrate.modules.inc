<?php

/**
 * @files
 * Hook implementations in behalf of core and supported contrib modules.
 */

/**
 * Implements hook_relation_migrate_type_target_validate().
 */
function taxonomy_relation_migrate_type_target_validate($element, $form_state, $form) {
  $fields = array_filter($form_state['input']['relation_migrate_taxonomy_term_reference_fields']);
  $relation_type = relation_type_load($form_state['input']['relation_migrate_taxonomy_term_reference_relation_type']);
  $supported_bundles = $relation_type->directional ? $relation_type->target_bundles : $relation_type->source_bundles;

  $return = TRUE;
  foreach ($fields as $field) {
    $field_def = field_info_field($field);
    $return &= in_array('taxonomy_term:' . $field_def['settings']['allowed_values'][0]['vocabulary'], $supported_bundles);
  }

  return in_array('taxonomy_term:*', $supported_bundles) || $return;
}

/**
 * Implements hook_relation_migrate_type_target_validate().
 */
function user_reference_relation_migrate_type_target_validate($element, $form_state, $form) {
  $fields = array_filter($form_state['input']['relation_migrate_user_reference_fields']);
  if (empty($fields)) {
    return TRUE;
  }

  $relation_type = relation_type_load($form_state['input']['relation_migrate_user_reference_relation_type']);
  $supported_bundles = $relation_type->directional ? $relation_type->target_bundles : $relation_type->source_bundles;
  foreach ($supported_bundles as $bundle) {
    if (strpos($bundle, 'user:') !== FALSE) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Implements hook_relation_migrate_type_target_validate().
 */
function node_reference_relation_migrate_type_target_validate($element, $form_state, $form) {
  $fields = array_filter($form_state['input']['relation_migrate_node_reference_fields']);
  $relation_type = relation_type_load($form_state['input']['relation_migrate_node_reference_relation_type']);
  $supported_bundles = $relation_type->directional ? $relation_type->target_bundles : $relation_type->source_bundles;

  $return = TRUE;
  foreach ($fields as $field) {
    $field_def = field_info_field($field);
    $node_types = array_filter($field_def['settings']['referenceable_types']);
    foreach ($node_types as $node_type) {
      $return &= in_array('node:' . $node_type, $supported_bundles);
    }
  }

  return in_array('node:*', $supported_bundles) || $return;
}

/**
 * Implements hook_relation_migrate_type_target_validate().
 */
function entityreference_relation_migrate_type_target_validate($element, $form_state, $form) {
  $fields = array_filter($form_state['input']['relation_migrate_entityreference_fields']);
  $relation_type = relation_type_load($form_state['input']['relation_migrate_entityreference_relation_type']);
  $supported_bundles = $relation_type->directional ? $relation_type->target_bundles : $relation_type->source_bundles;

  $return = TRUE;
  foreach ($fields as $field) {
    $field_def = field_info_field($field);
    $entity_type = $field_def['settings']['target_type'];
    $bundles = array_filter($field_def['settings']['handler_settings']['target_bundles']);
    $bundles_allowed = TRUE;
    foreach ($bundles as $bundle) {
      $bundles_allowed &= in_array($entity_type . ':' . $bundle, $supported_bundles);
    }
    $return &= in_array($entity_type . ':*', $supported_bundles) || $bundles_allowed;
  }

  return $return;
}

