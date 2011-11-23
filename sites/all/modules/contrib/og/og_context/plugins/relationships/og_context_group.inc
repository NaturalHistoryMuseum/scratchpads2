<?php


/**
 * @file
 * Plugin to provide an relationship handler for node from user.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Group from OG context module'),
  'keyword' => 'og_group',
  'description' => t('Get the group context, if exists, from Organic groups context module.'),
  'context' => 'og_group_context_from_og_context',
  // TODO: Remove this required context, as it is fake, however seems to be
  // needed by CTools.
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Return a new context based on an existing context.
 */
function og_group_context_from_og_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('entity:group', NULL);
  }

  if ($group = og_context()) {
    // Send it to ctools.
    return ctools_context_create('entity:group', $group);
  }
}
