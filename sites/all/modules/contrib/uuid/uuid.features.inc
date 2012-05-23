<?php

/**
 * @file
 * Features support to export entities from any Deploy <em>fetch-only</em> plan.
 */

/**
 * Implements [component]_features_export_options().
 */
function uuid_entities_features_export_options() {
  if (!module_exists('deploy')) {
    return array();
  }
  $options = array();
  $plans = deploy_plan_load_all(array('fetch_only' => 1));
  foreach ($plans as $plan) {
    $options[$plan->name] = $plan->title;
  }
  return $options;
}

/**
 * Implements [component]_features_export().
 */
function uuid_entities_features_export($components, &$export, $module_name) {
  foreach ($components as $plan_name) {
    $export['features']['uuid_entities'][$plan_name] = $plan_name;
  }
}

/**
 * Implements [component]_features_export_render().
 *
 * Exports entities that are formatted according to the OData protocol.
 *
 * @see DeployIterator
 * @see http://www.odata.org/developers/protocols
 */
function uuid_entities_features_export_render($module_name, $components, $export = NULL) {
  if (!module_exists('deploy')) {
    return array();
  }
  $code = array();
  $code[] = '  $entities = array();';
  $code[] = '';

  foreach ($components as $plan_name) {
    $plan = deploy_plan_load($plan_name);
    $iterator = $plan->getIterator();
    foreach ($iterator as $entity) {
      $entity_type = $entity->__metadata['type'];
      $entity_info = entity_get_info($entity_type);
      // We need to remove entity id and revision id keys.
      $id_key = $entity_info['entity keys']['id'];
      if (isset($entity->{$id_key})) {
        unset($entity->{$id_key});
      }
      if (!empty($entity_info['entity keys']['revision'])) {
        $vid_key = $entity_info['entity keys']['revision'];
        if (isset($entity->{$vid_key})) {
          unset($entity->{$vid_key});
        }
        // We also need to remove the VUUID key, since reverting can result in
        // new revision, and the feature will then always be overidden.
        if (!empty($entity_info['entity keys']['revision uuid'])) {
          $vuuid_key = $entity_info['entity keys']['revision uuid'];
          unset($entity->{$vuuid_key});
        }
      }
      // We unset some common timestamp properties, since those will change and
      // leave the feature constantly overidden.
      $keys = array('created', 'updated', 'changed', 'revision_timestamp', 'timestamp', 'stamp', 'current');
      foreach ($keys as $key) {
        if (isset($entity->{$key})) {
          unset($entity->{$key});
        }
      }
      // Let other modules alter exported entities.
      drupal_alter('uuid_entities_features_export_entity', $entity, $entity_type);

      list(,, $bundle_name) = entity_extract_ids($entity_type, $entity);
      $instances = field_info_instances($entity_type, $bundle_name);
      foreach ($instances as $field_name => $instance) {
        $field = field_info_field($field_name);
        if (!empty($entity->{$field_name})) {
          foreach ($entity->{$field_name} as $langcode => &$items) {
            // Let other modules alter fields on exported entities.
            $function = $field['module'] . '_uuid_entities_features_export_field_alter';
            if (function_exists($function)) {
              $function($entity_type, $entity, $field, $instance, $langcode, $items);
            }
            foreach ($items as &$item) {
              uuid_entities_features_clean($item);
            }
          }
        }
      }
      uuid_entities_features_clean($entity);
      $code[] = '  $entities[\'' . check_plain($plan->name) . '\'][] = (object) ' . features_var_export($entity, '  ') . ';';
    }
  }
  $code[] = '';
  $code[] = '  return $entities;';
  $code = implode("\n", $code);
  return array('uuid_default_entities' => $code);
}

/**
 * Implements [component]_features_export_rebuild().
 */
function uuid_entities_features_rebuild($module_name) {
  uuid_entities_rebuild($module_name, 'rebuild');
}

/**
 * Implements [component]_features_export_revert().
 */
function uuid_entities_features_revert($module_name) {
  uuid_entities_rebuild($module_name, 'revert');
}

/**
 * Helper function to rebuild entities from a plan.
 */
function uuid_entities_rebuild($module_name = '', $op = 'rebuild') {
  $entities = module_invoke($module_name, 'uuid_default_entities');
  if (!empty($entities)) {
    foreach ($entities as $plan_name => $entities) {
      // Let other modules do things before default entities are created.
      module_invoke_all("uuid_entities_pre_$op", $plan_name);
      foreach ($entities as $entity) {
        entity_uuid_save($entity->__metadata['type'], $entity);
      }
      // Let other modules do things after default entities are created.
      module_invoke_all("uuid_entities_post_$op", $plan_name);
    }
  }
}

/**
 * Helper function to sort properties of an object.
 *
 * This will maintain better consistency. Keys might get shifted order or type
 * due to alterations sometimes.
 */
function uuid_entities_features_clean(&$object) {
  $properties = array();
  foreach ($object as $key => $value) {
    $properties[$key] = $value;
    if (is_object($object)) {
      unset($object->{$key});
    }
    elseif (is_array($object)) {
      unset($object[$key]);
    }
  }
  ksort($properties);
  foreach ($properties as $key => $value) {
    // Make properties type consistent.
    if (is_string($value) || is_numeric($value)) {
      if (is_object($object)) {
        $object->{$key} = "$value";
      }
      elseif (is_array($object)) {
        $object[$key] = "$value";
      }
    }
    else {
      if (is_object($object)) {
        $object->{$key} = $value;
      }
      elseif (is_array($object)) {
        $object[$key] = $value;
      }
    }
  }
}
