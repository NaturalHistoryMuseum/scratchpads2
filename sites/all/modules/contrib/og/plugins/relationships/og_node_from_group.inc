<?php


/**
 * @file
 * Plugin to provide an relationship handler for node from OG gorup.
 */

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Node from OG group'),
  'keyword' => 'node',
  'description' => t('Get the node from an OG group.'),
  'required context' => new ctools_context_required(t('Group'), 'entity:group'),
  'context' => 'og_node_from_group_context',
);

/**
 * Return a new context based on an existing context.
 */
function og_node_from_group_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data) || !isset($context->data->gid)) {
    return ctools_context_create_empty('node', NULL);
  }

  if (isset($context->data->gid) && $context->data->entity_type == 'node') {
    $group = $context->data;
    if ($node = $group->getEntity()) {

      // Send it to ctools.
      return ctools_context_create('node', $node);
    }
  }
}
