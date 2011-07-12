<?php

/**
 * @file
 * On behalf implementation of mapping hooks for locale module.
 */

/**
 * Implementation of hook_feeds_parser_sources_alter().
 *
 * Declare the language of the feed node as a mapping source. This will most
 * commonly be used for having feed item nodes inherit the language
 * configuration of their feed node.
 */
function locale_feeds_parser_sources_alter(&$sources, $content_type) {
  if (!empty($content_type)) {
    $sources['parent:language'] = array(
      'name' => t('Feed node: Language'),
      'description' => t('Language of the feed node.'),
      'callback' => 'locale_feeds_get_source',
    );
  }
}

/**
 * Callback, returns specific locale settings of the parent feed node.
 */
function locale_feeds_get_source(FeedsImportBatch $batch, $key) {
  if ($node = node_load($batch->feed_nid)) {
    return $node->language;
  }
}

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 */
function locale_feeds_node_processor_targets_alter(&$targets, $content_type) {
  if (variable_get('language_content_type_'. $content_type, FALSE)) {
    $targets['language'] = array(
      'name' => t('Language'),
      'callback' => 'locale_feeds_set_target',
      'description' => t('The language of the node.'),
    );
  }
}

/**
 * Callback for mapping.
 */
function locale_feeds_set_target($node, $key, $language) {
  $node->language = $language;
}
