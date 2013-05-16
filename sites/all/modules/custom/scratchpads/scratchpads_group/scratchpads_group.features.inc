<?php

/**
 * @file
 * group.features.inc
 */
/**
 * Implementation of hook_ctools_plugin_api().
 */
function scratchpads_group_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "strongarm" && $api == "strongarm"){return array(
      "version" => "1"
    );}
  if($module == "context" && $api == "context"){return array(
      "version" => "3"
    );}
  if($module == 'facetapi'){return array(
      'version' => 1
    );}
}

/**
 * Implementation of hook_node_info().
 */
function scratchpads_group_node_info(){
  $items = array(
    'group' => array(
      'name' => t('Group'),
      'description' => t('A content type to enable users to create and manage their own \'groups\'. Each group can have subscribers, and maintains a group home page where subscribers communicate amongst themselves.'),
      'has_title' => '1',
      'title_label' => t('Group name'),
      'help' => '',
      'base' => 'node_content'
    )
  );
  return $items;
}
