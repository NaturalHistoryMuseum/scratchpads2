<?php

/**
 * @file
 * scratchpads_registry_stats.features.inc
 */
/**
 * Implements hook_ctools_plugin_api().
 */
function scratchpads_registry_stats_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "strongarm" && $api == "strongarm"){return array(
      "version" => "1"
    );}
}

/**
 * Implements hook_node_info().
 */
function scratchpads_registry_stats_node_info(){
  $items = array(
    'endpoint' => array(
      'name' => t('Endpoint'),
      'base' => 'node_content',
      'description' => t('Represents an endpoint associated with a Scratchpad.'),
      'has_title' => '1',
      'title_label' => t('Identifier'),
      'help' => ''
    ),
    'entity_data' => array(
      'name' => t('Scratchpad instance: Entity data'),
      'base' => 'node_content',
      'description' => t('Stores a snapshot of data about a particular Scratchpad and the number of entities it has.'),
      'has_title' => '1',
      'title_label' => t('ID'),
      'help' => ''
    ),
    'organization' => array(
      'name' => t('Organization'),
      'base' => 'node_content',
      'description' => t('Represents the organization registered with GBIF that is associated with a Scratchpad.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => ''
    ),
    'scratchpad' => array(
      'name' => t('Scratchpad'),
      'base' => 'node_content',
      'description' => t('Represents a single Scratchpad, with data pulled from the GBIF registry'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => ''
    ),
    'user_data' => array(
      'name' => t('Scratchpad instance: User data'),
      'base' => 'node_content',
      'description' => t('Stores a snapshot of data about a particular Scratchpad and the number of users it has, and their activity.'),
      'has_title' => '1',
      'title_label' => t('ID'),
      'help' => ''
    )
  );
  return $items;
}
