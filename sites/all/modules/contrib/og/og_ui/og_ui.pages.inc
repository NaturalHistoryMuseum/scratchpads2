<?php


/**
 * @file
 * Page callbacks for Group module.
 */

/**
 * Manage user subscription to a group.
 *
 * @param $node
 *   The group node entity.
 * @param $uid
 *   Optional; The user ID of the subscribing user. If empty the current user
 *   will be used.
 */
function og_ui_subscribe($entity_type, $etid, $uid = NULL) {
  global $user;
  if ($group = og_get_group($entity_type, $etid)) {
    if (empty($uid)) {
      $account = clone($user);

      if (!$account->uid) {
        $dest = drupal_get_destination();
        if (variable_get('user_register', 1)) {
          drupal_set_message(t('In order to join any group, you must <a href="!login">login</a>. After you have successfully done so, you will need to request membership again.', array('!login' => url("user/login", array('query' => $dest)))));
        }
        else {
          drupal_set_message(t('In order to join any group, you must <a href="!login">login</a> or <a href="!register">register</a> a new account. After you have successfully done so, you will need to request membership again.', array('!register' => url("user/register", array('query' => $dest)), '!login' => url("user/login", array('query' => $dest)))));
        }
        drupal_goto('user');
      }
    }
    else {
      $account = user_load($uid);
    }

    $entity = entity_load($entity_type, array($etid));
    $entity = reset($entity);

    // Check user isn't already subscribed.
    if (in_array($group->gid, og_get_entity_groups('user', $account))) {
      $params = array(
        '@user' => $account->name,
        // TODO: check user has permissions to see label (e.g. node title).
        '@group' => og_entity_label($entity_type, $entity),
      );

      $message = $account->uid == $user->uid ? t('You are already a member of the group @group.', $params) : t('@user is already a member of the group @group.', $params);
      drupal_set_message($message);

      _group_ui_redirect($entity_type, $entity);
    }
    elseif (og_user_access($group->gid, 'subscribe', $account) || og_user_access($group->gid, 'subscribe without approval', $account)) {
      // Show the user a subscription confirmation.
      return drupal_get_form('og_ui_confirm_subscribe', $group, $account);
    }
    drupal_access_denied();
  }
  // Not a valid group node.
  drupal_not_found();

}

/**
 * Confirm subscribe form.
 */
function og_ui_confirm_subscribe($form, &$form_state, $group, $account) {
  $form['group'] = array('#type' => 'value', '#value' => $group);
  $form['account'] = array('#type' => 'value', '#value' => $account);
  if (!og_user_access($group->gid, 'subscribe without approval')) {
    $form['request'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional details'),
      '#description' => t('Add any detail which will help an administrator decide whether to approve or deny your membership request.')
    );
  }
  $label = og_label($group->gid);
  return confirm_form($form, t('Are you sure you want to join the group %title?', array('%title' => $label)), "$group->entity_type/$group->etid", ' ', t('Join'), t('Cancel'));
}

/**
 * Submit handler; Confirm OG membership.
 */
function og_ui_confirm_subscribe_submit($form, &$form_state) {
  $group = $form_state['values']['group'];
  $account = $form_state['values']['account'];

  $state = og_user_access($group->gid, 'subscribe without approval') ? OG_STATE_ACTIVE : OG_STATE_PENDING;
  // Pass the user's request to the grouping action.
  $form_state['values'] += array('request' => '');
  $values = array('entity' => $account, 'state' => $state, 'user request' => $form_state['values']['request']);
  og_group($group->gid, $values);

  // Redirect.
  $form_state['redirect'] = "$group->entity_type/$group->etid";
}


/**
 * Confirm OG unsubscription form.
 *
 * The unsubscribing user is always the acting user - like this we make sure
 * no malicious user will unsubscribe another user. Administrators can reject or
 * ban another group member from the "people" page.
 */
function og_ui_unsubscribe($entity_type, $etid) {
  if ($group = og_get_group($entity_type, $etid)) {
    global $user;
    $label = og_label($group->gid);
    // Check the user isn't the manager of the group.
    $entity= entity_load($group->entity_type, array($group->etid));
    $entity = reset($entity);
    if (!empty($entity->uid) && $entity->uid != $user->uid) {
      $user_groups = og_get_entity_groups('user', $user);
      if (!empty($user_groups[$group->gid])) {
        // Show the user a subscription confirmation.
        return drupal_get_form('og_ui_confirm_unsubscribe', $group, $user);
      }
    }
    else {
      drupal_set_message(t('As the manager of %group, you can not leave the group.', array('%group' => $label)));
    }
    _group_ui_redirect($entity_type, $entity);
  }
  // Not a valid group.
  drupal_not_found();
}

/**
 * Helper function; Redirect back to entity, if access is allowed.
 *
 * @param $entity_type
 * @param $entity
 * @return unknown_type
 */
function _group_ui_redirect($entity_type, $entity, $account = NULL) {
  // Redirect back to entity, if access is allowed. Access is checked only
  // for nodes.
  $uri = array();

  if ($entity_type == 'node') {
    if (!node_access('view', $entity, $account)) {
      $uri = array(
        'path' => '<front>',
        'options' => array(),
      );
    }
  }
  if (!$uri) {
    $uri = entity_uri($entity_type, $entity);
  }
  drupal_goto($uri['path'], $uri['options']);
}

/**
 * Confirm unsubscribe form.
 */
function og_ui_confirm_unsubscribe($form, &$form_state, $group, $account) {
   $form['group'] = array('#type' => 'value', '#value' => $group);
   $form['account'] = array('#type' => 'value', '#value' => $account);

   $label = og_label($group->gid);
   return confirm_form($form, t('Are you sure you want to unsubscribe from the group %title?', array('%title' => $label)), "$group->entity_type/$group->etid", ' ', t('Remove'), t('Cancel'));
}

/**
 * Submit handler; Confirm OG unsubscription.
 */
function og_ui_confirm_unsubscribe_submit($form, &$form_state) {
  $group = $form_state['values']['group'];
  $account = $form_state['values']['account'];

  og_ungroup($group->gid, 'user', $account);

  // TODO: Determine where to go next - Group if accessible, or else site front
  // page.
  $form_state['redirect'] = "$group->entity_type/$group->etid";
}