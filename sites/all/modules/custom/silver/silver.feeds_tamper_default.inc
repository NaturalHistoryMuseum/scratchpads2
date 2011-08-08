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
    $feeds_tamper->id = 'taxonomy_importer_'.$vocabulary->vid.'-guid-calculate_hash';
    $feeds_tamper->importer = 'taxonomy_importer_'.$vocabulary->vid;
    $feeds_tamper->source = 'GUID';
    $feeds_tamper->plugin_id = 'hash';
    $feeds_tamper->settings = '';
    $feeds_tamper->weight = 0;
    $feeds_tamper->description = 'Calculate hash';
    $export['taxonomy_importer_'.$vocabulary->vid.'-guid-calculate_hash'] = $feeds_tamper;
  }
  return $export;
}