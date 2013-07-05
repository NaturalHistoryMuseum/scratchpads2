<?php

/**
 * @file
 * scratchpads_translate.strongarm.inc
 */
/**
 * Implements hook_strongarm().
 */
function scratchpads_translate_strongarm(){
  // Create base class
  $strongarm = new stdClass();
  $strongarm->disabled = FALSE;
  $strongarm->api_version = 1;
  $export = array();
  // entity_translation_comment_filter_page
  $strongarm->name = 'entity_translation_comment_filter_page';
  $strongarm->value = 0;
  $export['entity_translation_comment_filter_page'] = $strongarm;
  // entity_translation_entity_types
  $strongarm->name = 'entity_translation_entity_types';
  $strongarm->value = array(
    'node' => 'node',
    'taxonomy_term' => 'taxonomy_term',
    'comment' => 'comment',
    'publication' => 0,
    'file' => 'file',
    'user' => 'user'
  );
  $export['entity_translation_entity_types'] = $strongarm;
  // entity_translation_hide_translation_links_page
  $strongarm->name = 'entity_translation_hide_translation_links_page';
  $strongarm->value = 1;
  $export['entity_translation_hide_translation_links_page'] = $strongarm;
  // entity_translation_node_metadata_page
  $strongarm->name = 'entity_translation_node_metadata_page';
  $strongarm->value = '0';
  $export['entity_translation_node_metadata_page'] = $strongarm;
  // entity_translation_settings_node__page
  $strongarm->name = 'entity_translation_settings_node__page';
  $strongarm->value = array(
    'default_language' => 'en',
    'hide_language_selector' => 0,
    'exclude_language_none' => 0,
    'lock_language' => 0,
    'shared_fields_original_only' => 1
  );
  $export['entity_translation_settings_node__page'] = $strongarm;
  // entity_translation_shared_labels
  $strongarm->name = 'entity_translation_shared_labels';
  $strongarm->value = 1;
  $export['entity_translation_shared_labels'] = $strongarm;
  // entity_translation_workflow_enabled
  $strongarm->name = 'entity_translation_workflow_enabled';
  $strongarm->value = 0;
  $export['entity_translation_workflow_enabled'] = $strongarm;
  // translation_language_type
  $strongarm->name = 'translation_language_type';
  $strongarm->value = 'language_content';
  $export['translation_language_type'] = $strongarm;
  return $export;
}