<?php

/**
 * @file
 * Drush integration for the relation module.
 */

/**
 * Implements hook_drush_command().
 */
function relation_drush_command() {
  $items['relation-generate'] = array(
    'description' => dt('Generates relations.'),
    'arguments' => array(
      'number_relations' => dt('The number of relations to generate.'),
    ),
    'options' => array(
      'kill' => 'Delete all relations before generating new ones.',
      'types' => 'A comma delimited list of relation types to create.',
    ),
    'aliases' => array('genrel'),
  );
  return $items;
}

/**
 * Drush callback to generate relations.
 *
 * @param $number_relations
 *   Number of entities to generate of each entity_type.
 */
function drush_relation_generate($number_relations) {
  $types = drush_get_option('types');
  $types = $types ? explode(',', $types) : array();
  $kill = drush_get_option('kill');
  relation_generate_relations($number_relations, $types, $kill);
}

/**
 * Sends message to drush log, if enabled.
 *
 * @param $message
 *   Text of message.
 */
function relation_generate_message($message) {
  if (function_exists('drush_log')) {
    drush_log($message, 'ok');
  }
  else {
    drupal_set_message($message);
  }
}

/**
 * Generates pseudorandom relations. Appropriate entities must already exist.
 *
 * @param $number_relations
 *   Number of entities to generate of each entity_type.
 * @param $types
 *   Array of relation_type to generate relations for.
 * @param $kill
 *   Whether to delete all existing relations before creating new ones.
 *
 * @return
 *   Array of rids of the generated relations.
 */
function relation_generate_relations($number_relations = 10, $types = array(), $kill = FALSE) {
  if ($kill) {
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'relation')
      ->execute();
    if ($results) {
      $rids = array_keys($results['relation']);
      relation_delete_multiple($rids);
    }
  }
  $relation_types = relation_get_types($types);
  $new_rids = array();
  foreach ($relation_types as $type => $relation_type) {
    $available_types = array();
    foreach ($relation_type->source_bundles as $bundle_key) {
      list($entity_type, $bundle) = explode(':', $bundle_key, 2);
      $available_types['source'][$entity_type][] = $bundle;
    }
    foreach ($relation_type->target_bundles as $bundle_key) {
      list($entity_type, $bundle) = explode(':', $bundle_key, 2);
      $available_types['target'][$entity_type][] = $bundle;
    }
    $arity = rand($relation_type->min_arity, $relation_type->min_arity);
    for ($i = $number_relations; $i > 0; $i--) { // start new relation
      $entity_keys = array();
      for ($r_index = 0; $r_index < $arity; $r_index++) {
        if ($relation_type->directional && $r_index > 0) {
          $direction = 'target';
        }
        else { // use source bundles
          $direction = 'source';
        }
        $entity_type = array_rand($available_types[$direction]);
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', $entity_type, '=')
          // Would be nice to ->entityOrderBy('RAND()'); here, and set
          // range(0, 1). See http://drupal.org/node/1174806
          ->range(0, 2 * $number_relations);
        if (!in_array('*', $available_types[$direction][$entity_type])) {
          // Entities with a single bundle don't support EFQ bundle condition.
          $entity_info = entity_get_info($entity_type);
          if (count($entity_info['bundles']) > 1) {
            $query->entityCondition('bundle', $available_types[$direction][$entity_type], 'IN');
          }
        }
        $results = $query->execute();
        if ($results) {
          $entity_ids = array_keys(reset($results));
          $key = array_rand($entity_ids); // pseudorandomise until EFQ does random.
          $entity_keys[] = array(
            'entity_type' => $entity_type,
            'entity_id'   => $entity_ids[$key],
            'r_index'     => $r_index,
          );
        }
      }
      $relation = relation_create($type, $entity_keys);
      $new_rids[] = relation_save($relation);
    }
  }
  relation_generate_message(t('Generated @number_relations relations.', array('@number_relations' => $number_relations)), 'ok');
  return $new_rids;
}
