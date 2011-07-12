<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function feeds_fast_news_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "data" && $api == "data_default") {
    return array("version" => 1);
  }
  elseif ($module == "data" && $api == "data_table") {
    return array("version" => 1);
  }
  elseif ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function feeds_fast_news_node_info() {
  $items = array(
    'feed_fast' => array(
      'name' => t('Fast feed'),
      'module' => 'node',
      'description' => t('Subscribe to RSS or Atom feeds. Create light weight database records from feed content.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
