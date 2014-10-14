<?php
/**
 * @file
 * agidrupal_feeds_and_importers.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function agidrupal_feeds_and_importers_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function agidrupal_feeds_and_importers_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function agidrupal_feeds_and_importers_node_info() {
  $items = array(
    'agris_authors_feed' => array(
      'name' => t('Agris AP authors feed'),
      'base' => 'node_content',
      'description' => t('This content type is used for importing authors from an Agris AP feed prior to importing the DLIOs'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agris_authors_upload' => array(
      'name' => t('Agris AP authors importer via upload'),
      'base' => 'node_content',
      'description' => t('This content type is used for importing authors from an Agris AP file by uploading it, prior to importing the DLIOs'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agris_conferences_feed' => array(
      'name' => t('Agris AP conferences feed'),
      'base' => 'node_content',
      'description' => t('This content type is used for importing conferences from an Agris AP feed prior to importing the DLIOs'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agris_corporate_feed' => array(
      'name' => t('Agris AP corporate authors feed'),
      'base' => 'node_content',
      'description' => t('This content type is used for importing corporate authors from an Agris AP feed prior to importing the DLIOs'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agris_dlio_upload' => array(
      'name' => t('Agris AP DLIO importer via upload'),
      'base' => 'node_content',
      'description' => t('This content type is used as a feed for importing DLIO records by uploading an Agris AP file. It must be run after the other Agris AP importers (authors, journals etc.) have been run on the same file.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agris_journals_feed' => array(
      'name' => t('Agris AP journals feed'),
      'base' => 'node_content',
      'description' => t('This content type is used for importing journals from an Agris AP feed prior to importing the DLIOs'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'agris_publishers_feed' => array(
      'name' => t('Agris AP publishers feed'),
      'base' => 'node_content',
      'description' => t('This content type is used for importing publishers from an Agris AP feed prior to importing the DLIOs'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'books_feed' => array(
      'name' => t('Agris AP Books feed'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'xmlimport_dlios' => array(
      'name' => t('Agris AP DLIO feed'),
      'base' => 'node_content',
      'description' => t('This content type is used as a feed for harvesting / importing DLIO records from an Agris AP. It must be run after the other Agris AP importers (authors, journals etc.) have been run.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
