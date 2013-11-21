<?php
/**
 * @file
 * museum_profile.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function museum_profile_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function museum_profile_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function museum_profile_node_info() {
  $items = array(
    'museum_profile' => array(
      'name' => t('Museum profile'),
      'base' => 'node_content',
      'description' => t('A standardised museum profile for herbaria and museums which hold collections of seaweeds in the UK'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
