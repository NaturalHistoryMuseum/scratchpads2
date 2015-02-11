<?php

/**
 * @file
 * scratchpads_ecological_relations_block.features.inc
 */
/**
 * Implements hook_ctools_plugin_api().
 */
function scratchpads_ecological_relations_block_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "context" && $api == "context"){return array(
      "version" => "3"
    );}
}

/**
 * Implements hook_views_api().
 */
function scratchpads_ecological_relations_block_views_api(){
  list($module, $api) = func_get_args();
  if($module == "views" && $api == "views_default"){return array(
      "version" => "3.0"
    );}
}
