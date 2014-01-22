<?php

/**
 * @file
 * character_editor.features.inc
 */
/**
 * Implementation of hook_default_character_editor_character_type().
 */
function character_editor_default_character_editor_character_type(){
  $items = array();
  $items['controlled'] = entity_import('character_editor_character_type', '{
    "type" : "controlled",
    "label" : "Controlled character",
    "rdf_mapping" : []
  }');
  $items['dna'] = entity_import('character_editor_character_type', '{ "type" : "dna", "label" : "DNA character", "rdf_mapping" : [] }');
  $items['numeric'] = entity_import('character_editor_character_type', '{ "type" : "numeric", "label" : "Numeric character", "rdf_mapping" : [] }');
  $items['text'] = entity_import('character_editor_character_type', '{ "type" : "text", "label" : "Text character", "rdf_mapping" : [] }');
  $items['group'] = entity_import('character_editor_character_type', '{ "type" : "group", "label" : "Group", "rdf_mapping" : [] }');
  $items['computed'] = entity_import('character_editor_character_type', '{ "type" : "computed", "label" : "Computed character", "rdf_mapping": []}');
  return $items;
}

/**
 * Implementation of hook_default_character_editor_project_type().
 */
function character_editor_default_character_editor_project_type(){
  $items = array();
  $items['default_character_project'] = entity_import('character_editor_project_type', '{
    "type" : "default_character_project",
    "label" : "Character projects",
    "weight" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
