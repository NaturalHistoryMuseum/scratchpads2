<?php

/**
 * Implementation of hook_feeds_importer_default().
 */
function feeds_fast_news_feeds_importer_default() {
  $export = array();
  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'feed_fast';
  $feeds_importer->config = array(
    'name' => 'Fast feed',
    'description' => 'Create light weight database records from feed items. Faster than aggregating nodes.',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsDataProcessor',
      'config' => array(
        'update_existing' => 0,
        'expire' => '7257600',
        'mappings' => array(
          '0' => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          '1' => array(
            'source' => 'description',
            'target' => 'description',
            'unique' => 0,
          ),
          '2' => array(
            'source' => 'url',
            'target' => 'url',
            'unique' => 1,
          ),
          '3' => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
      ),
    ),
    'content_type' => 'feed_fast',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );

  $export['feed_fast'] = $feeds_importer;
  return $export;
}
