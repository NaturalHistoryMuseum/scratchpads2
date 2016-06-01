<?php

/**
 * @file.
 */

/**
 * Page callback for 'user/%user/spambot' path.
 */
function spambot_user_spam($account) {
  // Check if current user isn't anonymous user.
  if (!$account->uid) {
    drupal_set_message('The Anonymous user account can\'t be reported for spam. If you intended to block a user account verify that the URL is /user/XXXX/spambot where XXXX is a valid UID', 'warning');
    return MENU_NOT_FOUND;
  }

  return drupal_get_form('spambot_user_spam_admin_form', $account);
}

/**
 * Form builder for spambot_user_spam_admin_form form.
 */
function spambot_user_spam_admin_form($form, &$form_state, $account) {
  $key = variable_get('spambot_sfs_api_key', FALSE);
  $comments_enabled = module_exists('comment');

  $node_count = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('uid', $account->uid)
    ->execute()
    ->rowCount();

  $status = t('This account has @n nodes.', array('@n' => $node_count));
  if ($comments_enabled) {
    $comment_count = db_select('comment', 'c')
      ->fields('c', array('cid'))
      ->condition('uid', $account->uid)
      ->execute()
      ->rowCount();

    $status = t('This account has @n nodes and @c comments.', array('@n' => $node_count, '@c' => $comment_count));
  }

  $form['check'] = array(
    '#type' => 'submit',
    '#value' => t('Check if this account matches a known spammer'),
  );

  $form['action'] = array(
    '#type' => 'fieldset',
    '#title' => t('Take action against this account'),
    '#collapsible' => TRUE,
    '#description' => $status,
  );
  $form['action']['unpublish_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unpublish nodes and comments by this account'),
    '#default_value' => TRUE,
  );
  $form['action']['delete_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete nodes and comments by this account'),
    '#default_value' => FALSE,
  );

  $form['action']['report'] = array(
    '#type' => 'fieldset',
    '#title' => t('Report this account to www.stopforumspam.com'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
  );

  // Fetch a list of reportable nodes.
  $form['action']['report']['nids'] = array();
  $result = db_select('node_spambot', 'ns')
    ->fields('ns', array('nid', 'hostname'))
    ->condition('ns.uid', $account->uid)
    ->orderBy('ns.nid', 'DESC')
    ->range(0, 20)
    ->execute();

  $nid_hostnames = array();
  foreach ($result as $record) {
    $nid_hostnames[$record->nid] = $record->hostname;
  }

  foreach ($nid_hostnames as $nid => $hostname) {
    if ($node = node_load($nid)) {
      $title = truncate_utf8(check_plain($node->title), 128, TRUE, TRUE);

      $form['action']['report']['nids'][$nid] = array(
        '#type' => 'checkbox',
        '#title' => l(
          $title,
          "node/$nid",
          array(
            'attributes' => array(
              'title' => $title,
            )
          )
        ) . ' ' . t('(node, ip=@ip)', array('@ip' => $hostname)),
        '#disabled' => !$key,
      );
    }
  }

  // Fetch a list of reportable comments.
  if ($comments_enabled) {
    $form['action']['report']['cids'] = array();
    $result = db_select('comment')
      ->fields('comment', array('cid'))
      ->condition('uid', $account->uid)
      ->orderBy('cid', 'DESC')
      ->range(0, 20)
      ->execute();

    $cids = array();
    foreach ($result as $record) {
      $cids[$record->cid] = $record->cid;
    }

    foreach ($cids as $cid) {
      if ($comment = comment_load($cid)) {
        $subject = truncate_utf8(check_plain($comment->subject), 128, TRUE, TRUE);

        $form['action']['report']['cids'][$cid] = array(
          '#type' => 'checkbox',
          '#title' => l(
            $subject,
            "node/$comment->nid",
            array(
              'fragment' => "comment-$comment->cid",
              'attributes' => array(
                'title' => $subject,
              ),
            )
          ) . ' ' . t('(comment, ip=@ip)', array('@ip' => $comment->hostname)),
          '#disabled' => !$key,
        );
      }
    }
  }

  if ($key) {
    $evidence_count = count($form['action']['report']['nids']) + count($form['action']['report']['cids']);
    $form['action']['report']['#description'] = $evidence_count ? t('Select one or more posts below to report them to www.stopforumspam.com.') : t('This account cannot be reported because no evidence or IP address is available.');
  }
  else {
    $form['action']['report']['#description'] = t('An API key from <a href="http://www.stopforumspam.com">www.stopforumspam.com</a> must <a href="!admin-url">be configured</a> to report spammers.', array('!admin-url' => url('admin/config/system/spambot')));
  }

  $form['action']['block_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block this account'),
    '#default_value' => TRUE,
  );
  $form['action']['delete_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete this account'),
    '#default_value' => FALSE,
  );  
  $form['action']['action'] = array(
    '#type' => 'submit',
    '#value' => t('Take action'),
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['#validate'][] = 'spambot_user_spam_admin_form_validate';
  $form['#submit'][] = 'spambot_user_spam_admin_form_submit';

  return $form;
}

/**
 * Validate handler for spambot_user_spam_admin_form() form.
 */
function spambot_user_spam_admin_form_validate(&$form, &$form_state) {
  $key_required = (!empty($form_state['values']['report']['nids']) && count(array_filter($form_state['values']['report']['nids']))) ? TRUE : FALSE;  
  if (module_exists('comment')) {
    $key_required = (!empty($form_state['values']['report']['cids']) && count(array_filter($form_state['values']['report']['cids']))) || $key_required;
  }

  if ($key_required && !variable_get('spambot_sfs_api_key', FALSE)) {
    form_set_error('', t('To report spammers to www.stopforumspam.com, you need to register for an API key at <a href="http://www.stopforumspam.com">www.stopforumspam.com</a> and enter it into the !page.', array('!page' => l('spambot settings', 'admin/config/system/spambot'))));
  }
}

/**
 * Submit handler for spambot_user_spam_admin_form() form.
 */
function spambot_user_spam_admin_form_submit(&$form, &$form_state) {
  $account = user_load($form_state['values']['uid']);

  if ($form_state['values']['op'] == $form_state['values']['check']) {
    _spambot_user_spam_admin_form_submit_check($form, $form_state, $account);
  }
  elseif ($form_state['values']['op'] == $form_state['values']['action']) {
    _spambot_user_spam_admin_form_submit_action($form, $form_state, $account);
  }
}

/**
 * Do complex checking at this user account.
 */
function _spambot_user_spam_admin_form_submit_check(&$form, &$form_state, $account) {
  $messages = array();
  $service_down = FALSE;

  // Check email and username.
  $data = array();
  $request = array(
    'email' => $account->mail,
    'username' => $account->name,
  );

  if (spambot_sfs_request($request, $data)) {
    if (!empty($data['email']['appears'])) {
      $messages[] = array(
        'text' => t('This account\'s email address matches @num times: !link', array('!link' => l($request['email'], 'http://www.stopforumspam.com/search?q=' . $request['email']), '@num' => $data['email']['frequency'])),
        'type' => 'warning',
      );
    }

    if (!empty($data['username']['appears'])) {
      $messages[] = array(
        'text' => t('This account\'s username matches @num times: !link', array('!link' => l($request['username'], 'http://www.stopforumspam.com/search?q=' . $request['username']), '@num' => $data['username']['frequency'])),
        'type' => 'warning',
      );
    }

    // Check data at whitelist.
    if (spambot_check_whitelist('email', $account->mail)) {
      $messages[] = array(
        'text' => t('This account\'s email address placed at your whitelist.'),
        'type' => 'status',
      );
    }
    if (spambot_check_whitelist('username', $account->name)) {
      $messages[] = array(
        'text' => t('This account\'s username placed at your whitelist.'),
        'type' => 'status',
      );
    }
  }
  else {
    drupal_set_message(t('Error contacting service.'), 'warning');
    $service_down = TRUE;
  }

  // Check IP addresses.
  if (!$service_down) {
    $ips = spambot_account_ip_addresses($account);
    foreach ($ips as $ip) {
      // Skip the loopback interface.
      if ($ip == '127.0.0.1') {
        continue;
      }
      elseif (spambot_check_whitelist('ip', $ip)) {
        $whitelist_ips[] = $ip;
        continue;
      }

      $request = array('ip' => $ip);
      $data = array();
      if (spambot_sfs_request($request, $data)) {
        if (!empty($data['ip']['appears'])) {
          $messages[] = array(
            'text' => t('An IP address !ip used by this account matches @num times.', array('!ip' => l($ip, 'http://www.stopforumspam.com/search?q=' . $ip), '@num' => $data['ip']['frequency'])),
            'type' => 'warning',
          );
        }
      }
      else {
        drupal_set_message(t('Error contacting service.'), 'warning');
        break;
      }
    }

    if (!empty($whitelist_ips)) {
      $messages[] = array(
        'text' => t('These IP addresses placed at your whitelist: %ips', array('%ips' => implode(', ', $whitelist_ips))),
        'type' => 'status',
      );
    }
  }

  if ($messages) {
    foreach ($messages as $message) {
      drupal_set_message($message['text'], $message['type']);
    }
  }
  else {
    drupal_set_message(t('No matches against known spammers found.'));
  }
}

/**
 * Take action under this user account.
 */
function _spambot_user_spam_admin_form_submit_action(&$form, &$form_state, $account) {
  $comments_enabled = module_exists('comment');

  if ($account->uid == 1) {
    drupal_set_message(t('Sorry, taking action against uid 1 is not allowed.'), 'warning');
    return;
  }

  // Block account.
  if (!empty($form_state['values']['block_user'])) {
    if ($account->status) {
      user_save($account, array('status' => 0));
      drupal_set_message(t('Account blocked.'));
    }
    else {
      drupal_set_message(t('This account is already blocked.'));
    }
  }

  // Prepare some data.
  $nodes = db_select('node')
    ->fields('node', array('nid'))
    ->condition('uid', $account->uid, '=')
    ->orderBy('nid')
    ->execute()
    ->fetchCol();

  $node_hostnames = array();
  $result = db_select('node_spambot')
    ->fields('node_spambot', array('nid', 'hostname'))
    ->condition('uid', $account->uid)
    ->orderBy('nid', 'DESC')
    ->execute();
  foreach ($result as $record) {
    $node_hostnames[$record->nid] = $record->hostname;
  }

  $comments = array();
  if ($comments_enabled) {
    $comments = db_select('comment')
      ->fields('comment', array('cid'))
      ->condition('uid', $account->uid, '=')
      ->orderBy('cid')
      ->execute()
      ->fetchCol();
  }

  // Report posts to www.stopforumspam.com.
  if (!empty($form_state['values']['report']['nids'])) {
    foreach (array_filter($form_state['values']['report']['nids']) as $nid => $unused) {
      $node = node_load($nid);
      if (!empty($node->nid)) {
        if (spambot_report_account($account, $node_hostnames[$nid], $node->title . "\n\n" . $node->body['und'][0]['summary'] . "\n\n" . $node->body['und'][0]['value'])) {
          drupal_set_message(t('Node %title has been reported.', array('%title' => $node->title)));
        }
        else {
          drupal_set_message(t('There was a problem reporting node %title.', array('%title' => $node->title)));
        }
      }
    }
  }

  if ($comments_enabled && !empty($form_state['values']['report']['cids'])) {
    foreach (array_filter($form_state['values']['report']['cids']) as $cid => $unused) {
      $comment = comment_load($cid);
      if (!empty($comment->cid)) {
        if (spambot_report_account($account, $comment->hostname, $comment->subject . "\n\n" . $comment->comment_body['und'][0]['value'])) {
          drupal_set_message(t('Comment %title has been reported.', array('%title' => $comment->subject)));
        }
        else {
          drupal_set_message(t('There was a problem reporting comment %title.', array('%title' => $comment->subject)));
        }
      }
    }
  }

  // Delete nodes and content.
  if (!empty($form_state['values']['delete_content'])) {
    node_delete_multiple($nodes);

    if ($comments) {
      comment_delete_multiple($comments);
    }

    drupal_set_message(t('Nodes and comments have been deleted.'));
  }
  elseif (!empty($form_state['values']['unpublish_content'])) {
    // Unpublish nodes and content.
    if ($nodes) {
      module_load_include('inc', 'node', 'node.admin');
      node_mass_update($nodes, array('status' => 0));
    }

    if ($comments) {
      db_update('comment')
        ->fields(array('status' => COMMENT_NOT_PUBLISHED))
        ->condition('uid', $account->uid)
        ->execute();
    }
    drupal_set_message(t('Nodes and comments have been unpublished.'));
  }

  // Delete user.
  if (!empty($form_state['values']['delete_user'])) {
    // Redirect to user delete form.
    $form_state['redirect'] = "user/$account->uid/cancel";
  }
}
