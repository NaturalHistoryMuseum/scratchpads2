<?php
/**
 * @file
 * spm.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function spm_ctools_plugin_api() {
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
 * Implements hook_node_info().
 */
function spm_node_info() {
  $items = array(
    'spm' => array(
      'name' => t('Taxon Description'),
      'description' => t('A taxon description content type based on the TDWG/EoL Species Profile Model.'),
      'has_title' => '1',
      'title_label' => t('Taxon'),
      'help' => '',
      'base' => 'node_content'
    ),
  );
  return $items;
}