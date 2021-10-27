<?php

function scratchpad_admin_openids()
{
  return array(
    'https://vbrant.scratchpads.org/user/57/identity' => 'Ben Scott (https://vbrant.scratchpads.org/user/57/identity)',
    'https://vbrant.scratchpads.org/users/laurence-livermore/openid' => 'Laurence Livermore (https://vbrant.scratchpads.org/users/laurence-livermore/openid)',
    'https://vbrant.scratchpads.org/users/vince-smith/openid' => 'Vince Smith (https://vbrant.scratchpads.org/users/vince-smith/openid)',
    'https://vbrant.scratchpads.org/users/paul-kiddle/openid' => 'Paul Kiddle (https://vbrant.scratchpads.org/users/paul-kiddle/openid)',
    'https://vbrant.scratchpads.org/users/josh-humphries/openid' => 'Josh Humphries (https://vbrant.scratchpads.org/users/josh-humphries/openid)',
    'https://vbrant.scratchpads.org/users/ginger/openid' => 'Ginger (https://vbrant.scratchpads.org/users/ginger/openid)',
  );
}

function scratchpad_logins()
{
  $query = db_select('users_roles', 'ur');
  $query->join('users', 'u', 'u.uid = ur.uid');
  $query->join('role', 'r', 'r.rid = ur.rid');
  $query->condition('u.uid', 1, '>')->fields('u', array(
    'uid',
    'name'
  ))->condition('status', 1)->fields('r', array(
    'rid'
  ))->orderBy('rid', 'DESC')->orderBy('name');
  $results = $query->execute();
  $roles = user_roles();
  unset($roles[1]);
  unset($roles[2]);
  if (variable_get('scratchpad_logins_uid_1', TRUE)) {
    $users = array(
      'Scratchpad Team' => array(
        1 => 'Scratchpad Team [1]'
      )
    );
  } else {
    $users = array();
  }
  foreach ($results as $row) {
    $users[$roles[$row->rid]][$row->uid] = $row->name . " [" . $row->uid . "]";
  }
  // db_query('SELECT uid FROM {users_roles} WHERE rid = 5 AND uid >
  // 1')->fetchAllAssoc();
  $lowest_maintainer_uid = db_select('users_roles', 'u')->fields('u', array(
    'uid'
  ))->condition('rid', 5)->condition('uid', 1, '>')->orderBy('uid', 'ASC')->range(0, 1)->execute()->fetchCol(0);
  // $lowest_maintainer_uid = array_pop(db_query('SELECT MIN(uid) FROM
  // {users_roles} WHERE rid = 5 AND uid > 1')->fetchAllAssoc());
  // $lowest_maintainer_uid = 2;
  $boost_warning = '';
  if (module_exists('boost')) {
    $boost_warning = '<p>' . t("Note, this page will be cached by Boost.  If you've recently added a new maintainer and they're not in the list below, press Ctrl-F5 (<a href=\"http://en.wikipedia.org/wiki/Wikipedia:Bypass_your_cache#Instructions_for_various_browsers\">or something else</a>)") . '</p>';
  }
  switch ($_SERVER['SERVER_ADDR']) {
    case '157.140.127.10':
      $default_value = 'http://vbrant.eu/user/57/identity';
      break;
    case '157.140.126.27':
      $default_value = 'http://vbrant.eu/users/vince-smith/openid';
      break;
    default:
      $default_value = 'https://id.ebaker.me.uk/ed';
  }
  return array(
    'boost_warning' => array(
      '#value' => $boost_warning
    ),
    'openid' => array(
      '#title' => 'OpenID',
      '#type' => 'select',
      '#options' => variable_get('scratchpad_administrator_openids', scratchpad_admin_openids()),
      '#default_value' => $default_value
    ),
    'uid' => array(
      '#title' => t('User to login as'),
      '#type' => 'select',
      '#options' => $users,
      '#default_value' => $lowest_maintainer_uid
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Login')
    )
  );
}

function scratchpad_logins_submit()
{
  global $user;
  if (variable_get('site_offline', 0) && !$user->uid) {
    openid_begin($_POST['openid'], url('user', array(
      'absolute' => TRUE,
      'query' => array(
        'uid' => $_POST['uid']
      )
    )));
  } else {
    openid_begin($_POST['openid'], url('logins/return/' . $_POST['uid'], array(
      'absolute' => TRUE
    )));
  }
}

function scratchpad_do_openid_login($uid)
{
  $result = openid_complete();
  // Ensure that the Openid returned, is one of the ones we're allowing
  $allowed_openids = variable_get('scratchpad_administrator_openids', scratchpad_admin_openids());
  if ($result['status'] == 'success' && isset($allowed_openids[$result['openid.claimed_id']])) {
    global $user;
    $user = user_load($uid);
    if (function_exists('boost_init')) {
      boost_init();
    }
    watchdog('logins', '"@openid" logged in as "@user"', array(
      '@openid' => $result['openid.claimed_id'],
      '@user' => $user->name
    ));
    drupal_goto();
  } else {
    watchdog('logins', print_r($result, 1));
    // db_query("DELETE FROM {openid_association} WHERE idp_endpoint_uri =
    // '%s'")
    drupal_set_message(t('Login failed, are you sure the OpenID "%openid" is permitted to "Logins"', array(
      '%openid' => $result['openid.claimed_id']
    )), 'error');
    drupal_goto('logins');
  }
}
