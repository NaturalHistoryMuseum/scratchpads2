<?php

/**
 * Implementation of hook_feeds_tamper_default().
 */
function silver_feeds_tamper_default(){
  // Array of "tampers".
  $export = array();
  // Get a list of the vocabularies.
  $vocabularies = taxonomy_get_vocabularies();
  foreach($vocabularies as $vocabulary){
    $feeds_tamper = new stdClass();
    $feeds_tamper->disabled = FALSE;
    $feeds_tamper->api_version = 2;
    $feeds_tamper->id = 'taxonomy_importer_' . $vocabulary->machine_name . '-guid-calculate_hash';
    $feeds_tamper->importer = 'taxonomy_importer_' . $vocabulary->machine_name;
    $feeds_tamper->source = 'GUID';
    $feeds_tamper->plugin_id = 'hash';
    $feeds_tamper->settings = array(
      'overwrite' => 0
    );
    $feeds_tamper->weight = 0;
    $feeds_tamper->description = 'Calculate hash';
    $export['taxonomy_importer_' . $vocabulary->machine_name . '-guid-calculate_hash'] = $feeds_tamper;
  }
  // Get the node types
  $entity_info = entity_get_info('node');
  foreach($entity_info['bundles'] as $node_type => $node_type_info){
    $feeds_tamper = new stdClass();
    $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
    $feeds_tamper->api_version = 2;
    $feeds_tamper->id = 'node_importer_' . $node_type . '-guid-calculate_hash';
    $feeds_tamper->importer = 'node_importer_' . $node_type;
    $feeds_tamper->source = 'GUID';
    $feeds_tamper->plugin_id = 'hash';
    $feeds_tamper->settings = array(
      'overwrite' => 0
    );
    $feeds_tamper->weight = 0;
    $feeds_tamper->description = 'Calculate hash';
    $export['node_importer_' . $node_type . '-guid-calculate_hash'] = $feeds_tamper;
  }
  return $export;
}