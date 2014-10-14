<?php
/**
 * @file
 * agridrupal_content_types.arc2_store.inc
 */

/**
 * Implements hook_default_arc2_store_settings().
 */
function agridrupal_content_types_default_arc2_store_settings() {
  $export = array();

  $arc2_store_settings = new stdClass();
  $arc2_store_settings->disabled = FALSE; /* Edit this to true to make a default arc2_store_settings disabled initially */
  $arc2_store_settings->api_version = 1;
  $arc2_store_settings->store_id = 'agridrupal';
  $arc2_store_settings->label = 'agridrupal';
  $arc2_store_settings->settings = array(
    'endpoint_enabled' => 1,
    'endpoint_path' => 'sparql2',
    'endpoint_read_features' => array(
      'select' => 'select',
      'construct' => 'construct',
      'describe' => 'describe',
      'ask' => 0,
      'dump' => 0,
    ),
    'endpoint_max_limit' => '500',
    'endpoint_timeout' => '3000',
    'endpoint_read_key' => '',
    'endpoint_write_enabled' => 0,
    'endpoint_write_key' => '23fcc7d52d98a44a96d8761474cf99497e2a0d17',
    'endpoint_write_features' => array(
      'load' => 0,
      'insert' => 0,
      'delete' => 0,
    ),
  );
  $export['agridrupal'] = $arc2_store_settings;

  return $export;
}
