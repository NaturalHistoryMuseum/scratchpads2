<?php

/**
 * @file
 * Development related functions.
 */

/**
 * Exports relation types, display them in a form. Used by ctools export.
 */
function relation_export_relation_type($form, &$form_state, $type) {
  $relation_type = relation_relation_type_load($type->relation_type);
  drupal_set_title(check_plain($type->label));
  $code = relation_relation_type_export($relation_type);
  $lines = substr_count($code, "\n");

  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into another myobj using the import function.'),
  );
  return $form;
}

/**
 * Loads a single relation type for export.
 */
function relation_relation_type_load($type) {
  ctools_include('export');
  $result = ctools_export_load_object('relation_type', 'names', array($type));
  if (isset($result[$type])) {
    return $result[$type];
  }
}

/**
 * Exports a relation type.
 */
function relation_relation_type_export($relation_type, $indent = '') {
  ctools_include('export');
  $additional2 = array(
    'source_bundles' => $relation_type->source_bundles,
    'target_bundles' => $relation_type->target_bundles,
  );
  $output = ctools_export_object('relation_type', $relation_type, $indent, NULL, array(), $additional2);
  return $output;
}

/**
 * Generates pseudorandom relations. Appropriate entities must already exist.
 *
 * @param $number
 *   Number of entities to generate of each entity_type.
 * @param $types
 *   Array of relation_type to generate relations for.
 * @param $kill
 *   Whether to delete all existing relations before creating new ones.
 *
 * @return
 *   Array of rids of the generated relations.
 */
function relation_generate_relations($number = 10, $types = array(), $kill = FALSE) {
  if ($kill) {
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'relation')
      ->execute();
    if ($results) {
      $rids = array_keys($results['relation']);
      relation_multiple_delete($rids);
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
    for ($i = $number; $i > 0; $i--) { // start new relation
      $entity_keys = array();
      for ($r_index = 0; $r_index < $arity; $r_index++) {
        if ($relation_type->directional && $r_index > 0) {
          $direction = 'target';
        }
        else { //use source bundles
          $direction = 'source';
        }
        $entity_type = array_rand($available_types[$direction]);
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', $entity_type, '=')
          // Would be nice to ->entityOrderBy('RAND()'); here, and set
          // range(0, 1). See http://drupal.org/node/1174806
          ->range(0, 2 * $number);
        if (!in_array('*', $available_types[$direction][$entity_type])) {
          $query->entityCondition('bundle', $available_types[$direction][$entity_type], 'IN');
        }
        $results = $query->execute();
        $entity_ids = array_keys(reset($results));
        $key = array_rand($entity_ids); //pseudorandomise until EFQ does random.
        $entity_keys[] = array(
          'entity_type' => $entity_type,
          'entity_id'   => $entity_ids[$key],
          'r_index'     => $r_index,
          );
      }
      $relation = relation_create($type, $entity_keys);
      $new_rids[] = relation_save($relation);
    }
  }
  return $new_rids;
}
