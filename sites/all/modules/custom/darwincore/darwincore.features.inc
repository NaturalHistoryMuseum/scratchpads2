<?php
/**
 * @file
 * darwincore.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function darwincore_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implementation of hook_node_info().
 */
function darwincore_node_info() {
  $items = array(
    'location' => array(
      'name' => t('Location'),
      'base' => 'node_content',
      'description' => t('A location which conforms to Darwincore 1.2.1, and can be associated with multiple specimens'),
      'has_title' => '1',
      'title_label' => t('Location identifier'),
      'help' => '',
    ),
    'specimen_observation' => array(
      'name' => t('Specimen/Observation'),
      'base' => 'node_content',
      'description' => t('A Specimen content type based upon Darwincore 1.2.1'),
      'has_title' => '1',
      'title_label' => t('Specimen identifier'),
      'help' => '',
    ),
  );
  return $items;
}
