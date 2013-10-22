<?php

/**
 * @file
 * Test default relation types.
 */

/**
 * Implements hook_relation_default_relation_types().
 */
function relation_ctools_test_relation_default_relation_types() {
  $relation_types = array();

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'hates';
  $relation_type->label = 'hates';
  $relation_type->reverse_label = 'hates';
  $relation_type->directional = 0;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'user:*',
  );
  $relation_type->target_bundles = array();
  $relation_types['hates'] = $relation_type;

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'loves';
  $relation_type->label = 'loves';
  $relation_type->reverse_label = 'loves';
  $relation_type->directional = 0;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'user:*',
  );
  $relation_type->target_bundles = array();
  $relation_types['loves'] = $relation_type;

  return $relation_types;
}
