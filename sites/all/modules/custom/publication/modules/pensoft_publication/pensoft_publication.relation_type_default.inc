<?php
/**
 * @file
 * pensoft_publication.relation_type_default.inc
 */

/**
 * Implements hook_relation_default_relation_types().
 */
function pensoft_publication_relation_default_relation_types() {
  $export = array();

  /* Treatments */
  $relation_type = new stdClass;
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'publication_treatments';
  $relation_type->label = 'Publication treatments';
  $relation_type->reverse_label = 'Publication treatments';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 1;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'publication:*',
  );
  $relation_type->target_bundles = array(
    0 => 'node:spm',
  );
  $export['publication_treatments'] = $relation_type;

  /* Authors */
  $relation_type = new stdClass;
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'publication_users';
  $relation_type->label = 'Publication users';
  $relation_type->reverse_label = 'Publication users';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'publication:*',
  );
  $relation_type->target_bundles = array(
    0 => 'user:*',
  );
  $export['publication_users'] = $relation_type;
  
  /* Contributors */
  $relation_type = new stdClass;
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'publication_contributors';
  $relation_type->label = 'Publication contributors';
  $relation_type->reverse_label = 'Publication contributors';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'publication:*',
  );
  $relation_type->target_bundles = array(
    0 => 'user:*',
  );
  $export['publication_contributors'] = $relation_type;
  
  return $export;
}
