<?php
/**
 * @file
 * scratchpads_forum.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function scratchpads_forum_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}