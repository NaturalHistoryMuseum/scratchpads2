<?php

function spambot_user_spam_admin_form($form, &$form_state, $account) {
  $node_count = db_select('node')->condition('uid', $account->uid, '=')
    ->countQuery()->execute()->fetchField();
  
  if (module_exists('comment')) {
    $comment_count = db_select('comment')->condition('uid', $account->uid, '=')
      ->countQuery()->execute()->fetchField();
    $status = t('This account has @n nodes and @c comments.', array('@n' => $node_count, '@c' => $comment_count));
  }
  else {
    $status = t('This account has @n nodes.', array('@n' => $node_count));
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
    '#description' => t('An API key from <a href="http://www.stopforumspam.com">www.stopforumspam.com</a> is required to report spammers.<br />Select one or more posts below to report them to www.stopforumspam.com.'),
  );

  // Fetch a list of reportable nodes
  $form['action']['report']['nids'] = array();
  $result = db_select('node_spambot')->fields('node_spambot', array('nid', 'hostname'))->condition('uid', $account->uid)->orderBy('nid', 'DESC')->range(0, 20)->execute();
  $nid_hostnames = array();
  foreach ($result as $record) {
    $nid_hostnames[$record->nid] = $record->hostname;
  }

  foreach ($nid_hostnames as $nid => $hostname) {
    $node = node_load($nid);
    if (!empty($node->nid)) {
      $form['action']['report']['nids'][$nid] = array(
        '#type' => 'checkbox',
        '#title' => l(mb_strimwidth($node->title, 0, 128, '...'), 'node/' . $nid, array('attributes' => array('title' => mb_strimwidth($node->body['und'][0]['summary'] . "\n\n" . $node->body['und'][0]['value'], 0, 256, '...')))) . ' ' . t('(node, ip=@ip)', array('@ip' => $hostname)),
      );
    }
  }

  // Fetch a list of reportable comments
  if (module_exists('comment')) {
    $form['action']['report']['cids'] = array();
    $result = db_select('comment')->fields('comment', array('cid'))->condition('uid', $account->uid)->orderBy('cid', 'DESC')->range(0, 20)->execute();
    $cids = array();
    foreach ($result as $record) {
      $cids[$record->cid] = $record->cid;
    }

    foreach ($cids as $cid) {
      $comment = comment_load($cid);
      if (!empty($comment->cid)) {
        $form['action']['report']['cids'][$cid] = array(
          '#type' => 'checkbox',
          '#title' => l(mb_strimwidth($comment->subject, 0, 128, '...'), 'node/' . $comment->nid, array('fragment' => 'comment-'. $comment->cid, 'attributes' => array('title' => mb_strimwidth($comment->comment_body['und'][0]['value'], 0, 256, '...')))) . ' ' . t('(comment, ip=@ip)', array('@ip' => $comment->hostname)),
        );
      }
    }
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
  $form['uid'] = array('#type' => 'value', '#value' => $account->uid);
  return $form;
}

function spambot_user_spam_admin_form_validate($form, &$form_state) {
  $key_required = (!empty($form_state['values']['report']['nids']) && count(array_filter($form_state['values']['report']['nids']))) ? TRUE : FALSE;  
  if (module_exists('comment')) {
    $key_required = (!empty($form_state['values']['report']['cids']) && count(array_filter($form_state['values']['report']['cids']))) || $key_required;
  }

  if ($key_required && (!variable_get('spambot_sfs_api_key', FALSE))) {
    form_set_error('', t('To report spammers to www.stopforumspam.com, you need to register for an API key at <a href="http://www.stopforumspam.com">www.stopforumspam.com</a> and enter it into the !page.', array('!page' => l('spambot settings', 'admin/config/system/spambot'))));
  }
}

function spambot_user_spam_admin_form_submit($form, &$form_state) {
  $account = user_load($form_state['values']['uid']);
  if ($form_state['values']['op'] == $form_state['values']['check']) {
    // This is a more comprehensive check than the automated criteria checks.
    // This tests everything.    
    $messages = array();
    $service_down = FALSE;

    // Check email and username
    $request = array('email' => $account->mail, 'username' => $account->name);
    $data = array();
    if (spambot_sfs_request($request, $data)) {
      if (!empty($data['email']['appears'])) {
        $messages[] = t('This account\'s email address matches @num times: !link', array('!link' => l($request['email'], 'http://www.stopforumspam.com/search?q=' . $request['email']), '@num' => $data['email']['frequency']));
      }

      if (!empty($data['username']['appears'])) {
        $messages[] = t('This account\'s username matches @num times: !link', array('!link' => l($request['username'], 'http://www.stopforumspam.com/search?q=' . $request['username']), '@num' => $data['username']['frequency']));
      }
    }
    else {
      drupal_set_message(t('Error contacting service.'), 'warning');
      $service_down = TRUE;
    }

    // Check IP addresses
    if (!$service_down) {
      $ips = spambot_account_ip_addresses($account);
      foreach ($ips as $ip) {
        // Skip the loopback interface
        if ($ip == '127.0.0.1') {
          continue;
        }
        
        $request = array('ip' => $ip);
        $data = array();
        if (spambot_sfs_request($request, $data)) {
          if (!empty($data['ip']['appears'])) {
            $messages[] = t('An IP address !ip used by this account matches @num times.', array('!ip' => l($ip, 'http://www.stopforumspam.com/search?q=' . $ip), '@num' => $data['ip']['frequency']));
          }
        }
        else {
          drupal_set_message(t('Error contacting service.'), 'warning');
          $service_down = TRUE;
          break;
        }
      }
    }

    if (count($messages)) {
      foreach ($messages as $message) {
        drupal_set_message($message);
      }
    }
    else {
      drupal_set_message(t('No matches against known spammers found.'));
    }
  }
  else if ($form_state['values']['op'] == $form_state['values']['action']) {
    if ($account->uid == 1) {
      drupal_set_message(t('Sorry, taking action against uid 1 is not allowed.'));
      return;
    }

    // Block account
    if (!empty($form_state['values']['block_user'])) {
      if ($account->status) {
        user_save($account, array('status' => 0));
        drupal_set_message(t('Account blocked.'));
      }
      else {
        drupal_set_message(t('This account is already blocked.'));
      }
    }

    // Prepare some data
    $nodes = db_select('node')->fields('node', array('nid'))
      ->condition('uid', $account->uid, '=')->orderBy('nid')->execute()->fetchCol();

    $node_hostnames = array();
    $result = db_select('node_spambot')->fields('node_spambot', array('nid', 'hostname'))->condition('uid', $account->uid)->orderBy('nid', 'DESC')->execute();
    foreach ($result as $record) {
      $node_hostnames[$record->nid] = $record->hostname;
    }

    $comments = array();
    if (module_exists('comment')) {
      $comments = db_select('comment')->fields('comment', array('cid'))
        ->condition('uid', $account->uid, '=')->orderBy('cid')->execute()->fetchCol();
    }

    // Report posts to www.stopforumspam.com
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

    if (module_exists('comment') && !empty($form_state['values']['report']['cids'])) {
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

    // Delete nodes and content
    if (!empty($form_state['values']['delete_content'])) {
      node_delete_multiple($nodes);

      if (count($comments)) {
        comment_delete_multiple($comments);
      }

      drupal_set_message(t('Nodes and comments have been deleted.'));
    }
    else if (!empty($form_state['values']['unpublish_content'])) {
      // Unpublish nodes and content
      if (count($nodes)) {
        module_load_include('inc', 'node', 'node.admin');
        node_mass_update($nodes, array('status' => 0));
      }

      if (count($comments)) {
        db_update('comment')->fields(array('status' => COMMENT_NOT_PUBLISHED))
          ->condition('uid', $account->uid)->execute();
        cache_clear_all();
      }
      drupal_set_message(t('Nodes and comments have been unpublished.'));
    }

    // Delete user
    if (!empty($form_state['values']['delete_user'])) {
      // Redirect to user delete form
      $form_state['redirect'] = 'user/' . $account->uid . '/cancel';
    }
  }
}
