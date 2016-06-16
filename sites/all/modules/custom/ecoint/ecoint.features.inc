<?php

/**
 * @file
 * ecoint.features.inc
 */
/**
 * Implements hook_ctools_plugin_api().
 */
function ecoint_ctools_plugin_api(){
  list ( $module, $api ) = func_get_args();
  if($module == "field_group" && $api == "field_group"){
    return array(
      "version" => "1"
    );
  }
  list ( $module, $api ) = func_get_args();
  if($module == "strongarm" && $api == "strongarm"){
    return array(
      "version" => "1"
    );
  }
  if($module == "apachesolr_search" && $api == "apachesolr_search_defaults"){
    return array(
      "version" => "3"
    );
  }
  if($module == "context" && $api == "context"){
    return array(
      "version" => "3"
    );
  }
}

/**
 * Implements hook_node_info().
 */
function ecoint_node_info(){
  $items = array(
    'ecological_interactions' => array(
      'name' => t('Ecological Interactions'),
      'base' => 'node_content',
      'description' => t('Allows documenting of relationships between species.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => ''
    )
  );
  return $items;
}