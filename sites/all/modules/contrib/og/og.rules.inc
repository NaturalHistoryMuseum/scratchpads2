<?php


/**
 * @file
 * Rules integration for the Organic groups module.
 *
 * @addtogroup rules
 * @{
 */


/**
 * Implements hook_rules_event_info().
 */
function og_rules_event_info() {
  $items = array();
  $items['og_group'] = array(
    'label' => t('After grouping content'),
    'group' => t('Organic groups'),
    'variables' => array(
      'account' => array('type' => 'user', 'label' => t('Subscribing user')),
      'group' => array('type' => 'group', 'label' => t('Group')),
      'state' => array(
        'type' => 'text',
        'label' => t('The state of the subscription'),
        'options list' => 'og_group_states',
      ),
      'request' => array('type' => 'text', 'label' => t('User request')),
    ),
    // TODO:
    // 'access callback' => 'rules_user_integration_access',
  );

  $items['og_ungroup'] = array(
    'label' => t('After ungrouping content'),
    'group' => t('Organic groups'),
    'variables' => array(
      'account' => array('type' => 'user', 'label' => t('Unsubscribing user')),
      'group' => array('type' => 'group', 'label' => t('Group')),
    ),
    // TODO:
    // 'access callback' => 'rules_user_integration_access',
  );
  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function og_rules_action_info() {
  $items = array();
  $items['og_get_members'] = array(
    'label' => t('Get group members from group audience'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'group_audience' => array(
        'type' => 'list<field_item_group>',
        'label' => t('Field item group'),
      ),
    ),
    'provides' => array(
      'group_members' => array('type' => 'list<user>', 'label' => t('List of group members')),
    ),
    'base' => 'og_rules_get_members',
    // 'access callback' => 'rules_php_integration_access',
  );

  $items['og_get_managers'] = array(
    'label' => t('Get group manager from group audience'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'group_audience' => array(
        'type' => 'list<field_item_group>',
        'label' => t('Field item group'),
      ),
    ),
    'provides' => array(
      'group_managers' => array('type' => 'list<user>', 'label' => t('List of group managers')),
    ),
    'base' => 'og_rules_get_managers',
    // 'access callback' => 'rules_php_integration_access',
  );

  $items['og_get_managers_from_group'] = array(
    'label' => t('Get group manager from group'),
    'group' => t('Organic groups'),
    'parameter' => array(
      'group' => array(
        'type' => 'group',
        'label' => t('Group'),
      ),
    ),
    'provides' => array(
      'group_manager' => array('type' => 'user', 'label' => t('Group manager')),
    ),
    'base' => 'og_rules_get_manager_from_group',
    // 'access callback' => 'rules_php_integration_access',
  );

  return $items;
}

/**
 * Get group members from group audience callback.
 */
function og_rules_get_members($group_audience) {
  $gids = $return = array();
  foreach ($group_audience as $key => $value) {
    $gids[] = $value['gid'];
  }

  if ($gids) {
    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', 'user')
      ->fieldCondition(OG_AUDIENCE_FIELD, 'gid', $gids, 'IN');

    if ($result = $query->execute()) {
      $return = array_keys($result['user']);
    }
  }

  return array('group_members' => $return);
}

/**
 * Get group managers from group audience callback.
 */
function og_rules_get_managers($group_audience) {
  $gids = $return = array();
  foreach ($group_audience as $key => $value) {
    $gids[] = $value['gid'];
  }

  if ($gids) {
    $groups = og_load_multiple($gids);
    foreach ($groups as $group) {
      if ($account = $group->user()) {
        $return[] = $account;
      }
    }
  }

  return array('group_managers' => $return);
}


/**
 * Get the group manager from group callback.
 */
function og_rules_get_manager_from_group($group) {
  return array('group_manager' => $group->user());
}


/**
 * @}
 */
