<?php


/**
 * @file
 *
 * Plugin to provide an argument handler for a node id
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("OG: Group ID"),
  'keyword' => 'group',
  'description' => t('Creates a group context from a group ID argument.'),
  'context' => 'og_argument_gid_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the group  ID of a group for this argument'),
  ),
);

/**
 * Discover if this argument gives us the group we crave.
 */
function og_argument_gid_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('entity:group');
  }

  // We can accept either a group object or a pure gid.
  if (is_object($arg)) {
    return ctools_context_create('entity:group', $arg);
  }

  if (!is_numeric($arg)) {
    return FALSE;
  }

  $group = og_get_group('group', $arg);
  if (!$group) {
    return FALSE;
  }

  return ctools_context_create('entity:group', $group);
}