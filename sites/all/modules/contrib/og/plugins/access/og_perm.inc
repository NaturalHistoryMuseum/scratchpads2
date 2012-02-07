<?php


/**
 * @file
 * Plugin to provide access control based on user permission strings in a group.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("OG: user permission in group"),
  'description' => t('Control access by group permission string.'),
  'callback' => 'og_perm_ctools_access_check',
  'default' => array('perm' => ''),
  'settings form' => 'og_perm_ctools_access_settings',
  'summary' => 'og_perm_ctools_access_summary',
  'required context' => array(
    new ctools_context_required(t('User'), 'user'),
    new ctools_context_required(t('OG Group'), 'entity:group'),
  ),
);

/**
 * Settings form for the 'by perm' access plugin
 */
function og_perm_ctools_access_settings($form, &$form_state, $conf) {
  $perms = array();
  // Get list of permissions
  foreach (og_get_permissions() as $perm => $value) {
    // By keeping them keyed by module we can use optgroups with the
    // 'select' type.
    $perms[$value['module']][$perm] = $value['title'];
  }

  $form['settings']['perm'] = array(
    '#type' => 'select',
    '#options' => $perms,
    '#title' => t('Group permission'),
    '#default_value' => $conf['perm'],
    '#description' => t('Only users with the selected permission flag, in the specified group, will be able to access this.'),
  );

  return $form;
}

/**
 * Check for access.
 */
function og_perm_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  list($user_context, $og_group_context) = $context;
  if (empty($user_context) || empty($user_context->data)) {
    return FALSE;
  }

  if (empty($og_group_context) || empty($og_group_context->data) || empty($og_group_context->data->gid)) {
    return FALSE;
  }

  return og_user_access($og_group_context->data->gid, $conf['perm'], $user_context->data);
}

/**
 * Provide a summary description based upon the checked roles.
 */
function og_perm_ctools_access_summary($conf, $context) {
  list($user_context, $og_group_context) = $context;
  if (!isset($conf['perm'])) {
    return t('Error, unset permission');
  }

  $permissions = og_get_permissions();
  return t('@identifier has "@perm in @group group"', array('@identifier' => $user_context->identifier, '@perm' => $permissions[$conf['perm']]['title'], '@group' => $og_group_context->identifier));
}

