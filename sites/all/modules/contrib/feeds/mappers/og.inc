<?php

/**
 * @file
 * On behalf implementation of mapping hooks for Organic Groups module.
 */

/**
 * Implementation of hook_feeds_parser_sources_alter().
 */
function og_feeds_parser_sources_alter(&$sources, $content_type) {
  if (!empty($content_type)) {
    $sources['parent:og_groups'] = array(
      'name' => t('Feed node: Organic group(s)'),
      'description' => t('One or more organic groups that the feed node is part of or the organic group that the feed node represents.'),
      'callback' => 'og_feeds_get_source',
    );
  }
}

/**
 * Callback, returns OG of feed node.
 */
function og_feeds_get_source(FeedsImportBatch $batch, $key) {
  if ($node = node_load($batch->feed_nid)) {
    if (in_array($node->type, og_get_types('group'))) {
      return array(
        $node->nid => $node->nid,
      );
    }
    else {
      return isset($node->og_groups) ? $node->og_groups : NULL;
    }
  }
}

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 */
function og_feeds_node_processor_targets_alter(&$targets, $content_type) {
  if (in_array($content_type, og_get_types('group_post'))) {
    $targets['og_groups'] = array(
      'name' => t('Organic group(s)'),
      'callback' => 'og_feeds_set_target',
      'description' => t('One or more organic groups that the node is part of.'),
    );
  }
}

/**
 * Callback for mapping.
 */
function og_feeds_set_target($node, $key, $groups) {
  $node->og_groups = $groups;
}
