<?php

/**
 * @file
 * collections_profile.features.inc
 */
/**
 * Implements hook_ctools_plugin_api().
 */
function collections_profile_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "strongarm" && $api == "strongarm"){
    return array("version" => "1");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function collections_profile_views_api(){
  list($module, $api) = func_get_args();
  if($module == "views" && $api == "views_default"){return array(
      "version" => "3.0"
    );}
}

/**
 * Implements hook_node_info().
 */
function collections_profile_node_info(){
  $items = array(
    'collections_profile' => array(
      'name' => t('Collections profile'),
      'base' => 'node_content',
      'description' => t('A standardised profile for institutions which hold collections'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => ''
    )
  );
  return $items;
}
