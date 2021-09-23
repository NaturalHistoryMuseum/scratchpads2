<?php


/**
 * @file
 * Plugin to provide a relationship handler for OG membership from group and
 * user.
 */

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('OG membership from OG group and user'),
  'keyword' => 'og_membership',
  'description' => t('Get the OG membership context from an OG group and user.'),
  'required context' => array(
    new ctools_context_required(t('OG group'), 'entity:group'),
    new ctools_context_required(t('User'), 'user'),
  ),
  'context' => 'og_membership_from_group_context',
);

/**
 * Return a new context based on an existing context.
 */
function og_membership_from_group_context($context, $conf) {
  list($group_context, $user_context) = $context;

  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($group_context->data)) {
    return ctools_context_create_empty('entity:og_membership', NULL);
  }

  if (empty($group_context) || empty($group_context->data)) {
    return;
  }

  if (empty($user_context) || empty($user_context->data)) {
    return;
  }

  $og_membership = og_get_group_membership($group_context->data->gid, 'user', $user_context->data->uid);
  if (!$og_membership) {
    return;
  }

  // Send it to ctools.
  return ctools_context_create('entity:og_membership', $og_membership);
}
