<?php

/**
 * @file
 * Plugin to provide access control based on user group membership.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("OG: user membership in group"),
  'description' => t('Control access by group membership.'),
  'callback' => 'og_member_ctools_access_check',
  'default' => array('state' => OG_STATE_ACTIVE),
  'settings form' => 'og_member_ctools_access_settings',
  'summary' => 'og_member_ctools_access_summary',
  'required context' => array(
    new ctools_context_required(t('User'), 'user'),
    new ctools_context_required(t('OG Group'), 'entity:group'),
  ),
);

/**
 * Settings form for the 'by perm' access plugin
 */
function og_member_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['state'] = array(
    '#type' => 'select',
    '#options' => og_group_content_states(),
    '#title' => t('State in group'),
    '#multiple' => TRUE,
    '#default_value' => $conf['state'],
    '#description' => t('Only users with the specified state in group will be able to access this.'),
  );

  return $form;
}

/**
 * Check for access.
 */
function og_member_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  list($user_context, $og_group_context) = $context;
  if (empty($user_context) || empty($user_context->data)) {
    return FALSE;
  }

  if (empty($og_group_context) || empty($og_group_context->data) || empty($og_group_context->data->gid)) {
    return FALSE;
  }

  return og_is_member($og_group_context->data->gid, 'user', $user_context->data, $conf['state']);
}

/**
 * Provide a summary description based upon the checked roles.
 */
function og_member_ctools_access_summary($conf, $context) {
  list($user_context, $og_group_context) = $context;
  if (!isset($conf['state'])) {
    return t('Error, unset state');
  }

  $states = og_group_content_states();
  $state = array();
  foreach ($conf['state'] as $value) {
    $state[] = $states[$value];
  }

  return t('@identifier has "@state in @group group"', array('@identifier' => $user_context->identifier, '@state' => implode(',', $state), '@group' => $og_group_context->identifier));
}

