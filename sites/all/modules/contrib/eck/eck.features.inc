<?php

/**
 * @{
 * ECK entities
 */

/**
 * Implementation of hook_features_export_options().
 */
function eck_features_export_options() {
  $entities = array();
  foreach (eck_get_entities() as $entity) {
    $entities[$entity->name] = $entity->label;
  }
  return $entities;
}



/**
 * Implementation of hook_features_export.
 */
function eck_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  foreach ($data as $entity) {
    // Export the entity
    $export['features']['eck'][$entity] = $entity;
    $export['dependencies']['eck'] = 'eck';
    $export['dependencies']['features'] = 'features';
  }

  return $pipe;
}



/**
 * Implementation of hook_features_export_render().
 */
function eck_features_export_render($module, $data, $export = NULL) {
  $elements = array(
    'name' => FALSE,
    'label' => TRUE,
    'properties' => FALSE,
    'custom_properties' => FALSE,
  );

  $output = array();
  $output[] = '  $items = array(';

  foreach ($data as $entity) {
    // Get the entity definition.
    $entity = eck__entity__load($entity);

    $output[] = "    '{$entity->name}' => array(";
    foreach ($elements as $key => $t) {
      if ($t) {
        $text = str_replace("'", "\'", $entity->$key);
        $text = !empty($text) ? "t('{$text}')" : "''";
        $output[] = "      '{$key}' => {$text},";
      }
      else if (is_array($entity->$key)) {
        $output[] = "      '{$key}' => '" . serialize($entity->$key) . "',";
      }
      else {
        $output[] = "      '{$key}' => '{$entity->$key}',";
      }
    }
    $output[] = "    ),";
  }

  $output[] = '  );';
  $output[] = '  return $items;';

  return array('eck_info' => implode("\n", $output));
}



/**
 * Implementation of hook_features_revert().
 */
function eck_features_revert($module) {
  eck_features_rebuild($module);
}



/**
 * Implements of hook_features_rebuild().
 *
 * Rebuilds eck entities from code defaults.
 */
function eck_features_rebuild($module) {
  if ($default_entities = features_get_default('eck', $module)) {
    foreach ($default_entities as $entity_name => $entity_info) {
      $entity = array(
        'name' => $entity_name,
        'label' => $entity_info['label'],
        'properties' => $entity_info['properties'],
        'custom_properties' => $entity_info['custom_properties'],
      );
      db_merge('eck')
        ->key(array('name' => $entity_name))
        ->fields($entity)
        ->execute();

      if (!db_table_exists("eck_{$entity_name}")) {
        $entity['properties'] = unserialize($entity['properties']);
        $entity['custom_properties'] = unserialize($entity['custom_properties']);
        db_create_table("eck_{$entity_name}", eck__entity__schema((object) $entity));
      }
    }

    drupal_get_schema(NULL, TRUE);
    entity_info_cache_clear();
    menu_rebuild();
  }
}

/**
 * @} End of ECK entities.
 */

/**
 * @{
 * ECK entity types
 */

/**
 * Implementation of hook_features_export_options().
 */
function eck_type_features_export_options() {
  $types = array();
  foreach (eck_get_types() as $type) {
    $types[$type->type] = $type->label;
  }
  return $types;
}



/**
 * Implementation of hook_features_export.
 */
function eck_type_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $map = features_get_default_map('eck');

  foreach ($data as $type) {
    // Get the entity name of type
    $instances = field_info_instances();
    foreach ($instances as $name => $bundles) {
      if (isset($bundles[$type])) {
        $entity_name = $name;
        break;
      }
    }

    // If this entity type is provided by a different module, add it as a dependency.
    if (isset($map[$entity_name]) && $map[$entity_name] != $module_name) {
      $export['dependencies'][$map[$entity_name]] = $map[$entity_name];
    }
    else {
      // Add a dependency on the ECK entity.
      $pipe['eck'][] = $entity_name;
    }

    // Export the entity type.
    $export['features']['eck_type'][$type] = $type;
    $export['dependencies']['eck'] = 'eck';
    $export['dependencies']['features'] = 'features';

    // Export fields.
    $fields = $instances[$entity_name][$type];
    foreach ($fields as $field) {
      $pipe['field'][] = "{$entity_name}-{$field['bundle']}-{$field['field_name']}";
    }
  }

  return $pipe;
}



/**
 * Implementation of hook_features_export_render().
 */
function eck_type_features_export_render($module, $data, $export = NULL) {
  $elements = array(
    'entity' => FALSE,
    'type' => FALSE,
    'label' => TRUE,
  );

  $output = array();
  $output[] = '  $items = array(';

  foreach ($data as $type) {
    // Get the entity definition.
    $type = eck__entity_type__load($type);

    if(!$type){
      continue;
    }

    $output[] = "    '{$type->type}' => array(";
    foreach ($elements as $key => $t) {
      if ($t) {
        $text = str_replace("'", "\'", $type->$key);
        $text = !empty($text) ? "t('{$text}')" : "''";
        $output[] = "      '{$key}' => {$text},";
      }
      else {
        $output[] = "      '{$key}' => '{$type->$key}',";
      }
    }
    $output[] = "    ),";
  }

  $output[] = '  );';
  $output[] = '  return $items;';

  return array('eck_type_info' => implode("\n", $output));
}



/**
 * Implementation of hook_features_revert().
 */
function eck_type_features_revert($module) {
  eck_type_features_rebuild($module);
}



/**
 * Implements of hook_features_rebuild().
 *
 * Rebuilds eck entities from code defaults.
 */
function eck_type_features_rebuild($module) {
  if ($default_types = features_get_default('eck_type', $module)) {
    foreach ($default_types as $type_name => $type_info) {
      $type = array(
        'entity' => $type_info['entity'],
        'type' => $type_name,
        'label' => $type_info['label'],
      );
      db_merge('eck_types')
        ->key(array('type' => $type_name))
        ->fields($type)
        ->execute();
    }

    drupal_get_schema(NULL, TRUE);
    entity_info_cache_clear();
    menu_rebuild();
  }
}

/**
 * @} End of ECK entity types
 */
