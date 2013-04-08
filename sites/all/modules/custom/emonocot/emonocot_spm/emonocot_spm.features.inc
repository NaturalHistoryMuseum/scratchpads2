<?php
/**
 * @file
 * emonocot_spm.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function emonocot_spm_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function emonocot_spm_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
