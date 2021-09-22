<?php


/**
 * @file
 * Plugin to provide an relationship handler for OG group from node.
 */

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('OG group from node'),
  'keyword' => 'group',
  'description' => t('Get the OG group context from a node.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'context' => 'og_group_from_node_context',
);

/**
 * Return a new context based on an existing context.
 */
function og_group_from_node_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data) || !isset($context->data->nid)) {
    return ctools_context_create_empty('entity:group', NULL);
  }

  $group = og_get_group('node', $context->data->nid);

  if (!$group) {
    // The node isn't a group, check if it's a group content.
    $node = clone $context->data;
    $gids = og_get_entity_groups('node', $node);
    if (!$gids) {
      return;
    }

    // Get the first group.
    $gid = reset($gids);
    $group = og_load($gid);
  }

  if (!$group) {
    return;
  }

  // Send it to ctools.
  return ctools_context_create('entity:group', $group);
}
