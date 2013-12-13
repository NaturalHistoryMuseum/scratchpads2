<?php
/**
 * @file
 * character_editor.relation_type_default.inc
 */


/**
 * Implements hook_relation_default_relation_types().
 */
function character_editor_relation_default_relation_types(){
  $export = array();
  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'character';
  $relation_type->label = 'character';
  $relation_type->reverse_label = 'character';
  $relation_type->directional = 0;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 0;
  $relation_type->source_bundles = array(
    0 => 'character_editor_project:*',
    1 => 'character_editor_character:*'
  );
  $relation_type->target_bundles = array();
  $export['character'] = $relation_type;
  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'character_state';
  $relation_type->label = 'character_state';
  $relation_type->reverse_label = 'character_state';
  $relation_type->directional = 0;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'taxonomy_term:*',
    1 => 'character_editor_character:*',
    2 => 'node:specimen_observation'
  );
  $relation_type->target_bundles = array();
  $export['character_state'] = $relation_type;
  return $export;
}
