<?php
/**
 * @file
 * scratchpad_relations2.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function scratchpad_relations_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}
