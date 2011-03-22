<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function spm_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function spm_node_info() {
  $items = array(
    'spm' => array(
      'name' => t('Taxon Description'),
      'base' => 'node_content',
      'description' => t('A taxon description content type based on the TDWG/EoL Species Profile Model.'),
      'has_title' => '1',
      'title_label' => t('Taxon'),
      'help' => '',
    ),
  );
  return $items;
}
