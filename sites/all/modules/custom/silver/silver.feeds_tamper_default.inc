<?php

/**
 * Implementation of hook_feeds_tamper_default().
 */
function silver_feeds_tamper_default(){
  $export = array();
  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'taxonomy_importer-guid-calculate_hash';
  $feeds_tamper->importer = 'taxonomy_importer';
  $feeds_tamper->source = 'GUID';
  $feeds_tamper->plugin_id = 'hash';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Calculate hash';
  $export['taxonomy_importer-guid-calculate_hash'] = $feeds_tamper;
  return $export;
}
