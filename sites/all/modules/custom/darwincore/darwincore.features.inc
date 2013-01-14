<?php

/**
 * @file
 * darwincore.features.inc
 */
/**
 * Implements hook_ctools_plugin_api().
 */
function darwincore_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "field_group" && $api == "field_group"){return array(
      "version" => "1"
    );}
  if($module == "strongarm" && $api == "strongarm"){return array(
      "version" => "1"
    );}
}

/**
 * Implements hook_node_info().
 */
function darwincore_node_info(){
  $items = array(
    'location' => array(
      'name' => t('Location'),
      'description' => t('A location which conforms to Darwincore, and can be associated with multiple specimens'),
      'has_title' => '1',
      'title_label' => t('Location identifier (unique ID or description of this location)'),
      'help' => '',
      'base' => 'node_content'
    ),
    'specimen_observation' => array(
      'name' => t('Specimen/Observation'),
      'description' => t('A Specimen content type based upon Darwincore'),
      'has_title' => '1',
      'title_label' => t('Specimen identifier'),
      'help' => '',
      'base' => 'node_content'
    )
  );
  return $items;
}
