<?php
/**
 * @file
 * ecoint.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ecoint_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
  if ($module == "apachesolr_search" && $api == "apachesolr_search_defaults") {
    return array("version" => "3");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == 'feeds' && $api == 'plugins') {
    return array('version' => 1);
  }
}

/**
 * Implements hook_ctools_plugin_type().
 */
function ecoint_ctools_plugin_type() {
  return array(
      'plugins' => array(
          'cache' => TRUE,
          'use hooks' => TRUE,
          'classes' => array('handler'),
      ),
  );
}

/**
 * Implements hook_node_info().
 */
function ecoint_node_info() {
  $items = array(
    'ecological_interactions' => array(
      'name' => t('Ecological Interactions'),
      'base' => 'node_content',
      'description' => t('Allows documenting of relationships between species.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_feeds_plugins().
 */
function ecoint_feeds_plugins() {
  $info['EcointFeedsProcessor'] = array(
      'name' => t('Ecoint Processor'),
      'description' => t('Add and update ecological relations.'),
      'help' => t('Add and update ecological relations.'),
      'handler' => array(
          'parent' => 'FeedsProcessor',
          'class' => 'EcointFeedsProcessor',
          'file' => 'EcointFeedsProcessor.inc',
          'path' => drupal_get_path('module', 'ecoint'),
      ),
  );
  return $info;
}