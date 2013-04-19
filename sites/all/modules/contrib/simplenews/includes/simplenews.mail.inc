<?php

/**
 * @file
 * Simplenews email send and spool handling
 *
 * @ingroup mail
 */

/**
 * Add the newsletter node to the mail spool.
 *
 * @param $node
 *   The newsletter node to be sent.
 *
 * @ingroup issue
 */
function simplenews_add_node_to_spool($node) {
  // To send the newsletter, the node id and target email addresses
  // are stored in the spool.
  // Only subscribed recipients are stored in the spool (status = 1).
  $select = db_select('simplenews_subscriber', 's');
  $select->innerJoin('simplenews_subscription', 't', 's.snid = t.snid');
  $select->addField('s', 'mail');
  $select->addField('s', 'snid');
  $select->addField('t', 'tid');
  $select->addExpression($node->nid, 'nid');
  $select->addExpression(SIMPLENEWS_SUBSCRIPTION_STATUS_SUBSCRIBED, 'status');
  $select->addExpression(REQUEST_TIME, 'timestamp');
  $select->condition('t.tid', $node->simplenews->tid);
  $select->condition('t.status', SIMPLENEWS_SUBSCRIPTION_STATUS_SUBSCRIBED);
  $select->condition('s.activated', SIMPLENEWS_SUBSCRIPTION_ACTIVE);

  db_insert('simplenews_mail_spool')
    ->from($select)
    ->execute();

  // Update simplenews newsletter status to send pending.
  simplenews_newsletter_update_sent_status($node);

  // Notify other modules that a newsletter was just spooled.
  module_invoke_all('simplenews_spooled', $node);
}

/**
 * Send mail spool immediatly if cron should not be used.
 *
 * @param $conditions
 *   (Optional) Array of spool conditions which are applied to the query.
 */
function simplenews_mail_attempt_immediate_send(array $conditions = array(), $use_batch = TRUE) {
  if (variable_get('simplenews_use_cron', TRUE)) {
    return FALSE;
  }
  if ($use_batch) {
    // Set up as many send operations as necessary to send all mails with the
    // defined throttle amount.
    $throttle = variable_get('simplenews_throttle', 20);
    $spool_count = simplenews_count_spool($conditions);
    $num_operations = ceil($spool_count / $throttle);

    $operations = array();
    for ($i = 0; $i < $num_operations; $i++) {
      $operations[] = array('simplenews_mail_spool', array($throttle, $conditions));
    }

    // Add separate operations to clear the spool and updat the send status.
    $operations[] = array('simplenews_clear_spool', array());
    $operations[] = array('simplenews_send_status_update', array());

    $batch = array(
      'operations' => $operations,
      'title' => t('Sending mails'),
      'file' => drupal_get_path('module', 'simplenews') . '/includes/simplenews.mail.inc',
    );
    batch_set($batch);
  }
  else {
    // Send everything that matches the conditions immediatly.
    simplenews_mail_spool(SIMPLENEWS_UNLIMITED, $conditions);
    simplenews_clear_spool();
    simplenews_send_status_update();
  }
  return TRUE;
}

/**
 * Send test version of newsletter.
 *
 * @param mixed $node
 *   The newsletter node to be sent.
 *
 * @ingroup issue
 */
function simplenews_send_test($node, $test_addresses) {
  // Prevent session information from being saved while sending.
  if ($original_session = drupal_save_session()) {
    drupal_save_session(FALSE);
  }

  // Force the current user to anonymous to ensure consistent permissions.
  $original_user = $GLOBALS['user'];
  $GLOBALS['user'] = drupal_anonymous_user();

  // Send the test newsletter to the test address(es) specified in the node.
  // Build array of test email addresses

  // Send newsletter to test addresses.
  // Emails are send direct, not using the spool.
  $recipients = array('anonymous' => array(), 'user' => array());
  foreach ($test_addresses as $mail) {
    $mail = trim($mail);
    if (!empty($mail)) {
      $subscriber = simplenews_subscriber_load_by_mail($mail);
      if (!$subscriber) {
        // The source expects a subscriber object with mail and language set.
        // @todo: Find a cleaner way to do this.
        $subscriber = new stdClass();
        $subscriber->uid = 0;
        $subscriber->mail = $mail;
        $subscriber->language = $GLOBALS['language']->language;
      }

      if (!empty($account->uid)) {
        $recipients['user'][] = $account->name . ' <' . $mail . '>';
      }
      else {
        $recipients['anonymous'][] = $mail;
      }
      $source = new SimplenewsSourceNode($node, $subscriber);
      $source->setKey('test');
      $result = simplenews_send_source($source);
    }
  }
  if (count($recipients['user'])) {
    $recipients_txt = implode(', ', $recipients['user']);
    drupal_set_message(t('Test newsletter sent to user %recipient.', array('%recipient' => $recipients_txt)));
  }
  if (count($recipients['anonymous'])) {
    $recipients_txt = implode(', ', $recipients['anonymous']);
    drupal_set_message(t('Test newsletter sent to anonymous %recipient.', array('%recipient' => $recipients_txt)));
  }

  $GLOBALS['user'] = $original_user;
  if ($original_session) {
    drupal_save_session(TRUE);
  }
}

/**
 * Send a node to an email address.
 *
 * @param $source
 *   The source object.s
 *
 * @return boolean
 *   TRUE if the email was successfully delivered; otherwise FALSE.
 *
 * @ingroup source
 */
function simplenews_send_source(SimplenewsSourceInterface $source) {
  $params['simplenews_source'] = $source;

  // Send mail.
  $message = drupal_mail('simplenews', $source->getKey(), $source->getRecipient(), $source->getLanguage(), $params, $source->getFromFormatted());

  // Log sent result in watchdog.
  if (variable_get('simplenews_debug', FALSE)) {
    if ($message['result']) {
      watchdog('simplenews', 'Outgoing email. Message type: %type<br />Subject: %subject<br />Recipient: %to', array('%type' => $source->getKey(), '%to' => $message['to'], '%subject' => $message['subject']), WATCHDOG_DEBUG);
    }
    else {
      watchdog('simplenews', 'Outgoing email failed. Message type: %type<br />Subject: %subject<br />Recipient: %to', array('%type' => $source->getKey(), '%to' => $message['to'], '%subject' => $message['subject']), WATCHDOG_ERROR);
    }
  }

  // Build array of sent results for spool table and reporting.
  if ($message['result']) {
    $result = array(
      'status' => SIMPLENEWS_SPOOL_DONE,
      'error' => FALSE,
    );
  }
  else {
    // This error may be caused by faulty mailserver configuration or overload.
    // Mark "pending" to keep trying.
    $result = array(
      'status' => SIMPLENEWS_SPOOL_PENDING,
      'error' => TRUE,
    );
  }
  return $result;
}

/**
 * Send simplenews newsletters from the spool.
 *
 * Individual newsletter emails are stored in database spool.
 * Sending is triggered by cron or immediately when the node is saved.
 * Mail data is retrieved from the spool, rendered and send one by one
 * If sending is successful the message is marked as send in the spool.
 *
 * @todo: Redesign API to allow language counter in multilingual sends.
 *
 * @param $limit
 *   (Optional) The maximum number of mails to send. Defaults to
 *   unlimited.
 * @param $conditions
 *   (Optional) Array of spool conditions which are applied to the query.
 *
 * @return
 *   Returns the amount of sent mails.
 *
 * @ingroup spool
 */
function simplenews_mail_spool($limit = SIMPLENEWS_UNLIMITED, array $conditions = array()) {
  $check_counter = 0;

  // Send pending messages from database cache.
  $spool_list = simplenews_get_spool($limit, $conditions);
  if ($spool_list) {

    // Switch to the anonymous user.
    simplenews_impersonate_user(drupal_anonymous_user());

    $count_fail = $count_success = 0;
    $sent = array();

    _simplenews_measure_usec(TRUE);

    $spool = new SimplenewsSpool($spool_list);
    while ($source = $spool->nextSource()) {
      $source->setKey('node');
      $result = simplenews_send_source($source);

      // Update spool status.
      // This is not optimal for performance but prevents duplicate emails
      // in case of PHP execution time overrun.
      foreach ($spool->getProcessed() as $msid => $row) {
        $row_result = isset($row->result) ? $row->result : $result;
        simplenews_update_spool(array($msid), $row_result);
        if ($row_result['status'] == SIMPLENEWS_SPOOL_DONE) {
          $count_success++;
          if (!isset($sent[$row->actual_nid])) {
            $sent[$row->actual_nid] = 1;
          }
          else {
            $sent[$row->actual_nid]++;
          }
        }
        if ($row_result['error']) {
          $count_fail++;
        }
      }

      // Check every n emails if we exceed the limit.
      // When PHP maximum execution time is almost elapsed we interrupt
      // sending. The remainder will be sent during the next cron run.
      if (++$check_counter >= SIMPLENEWS_SEND_CHECK_INTERVAL && ini_get('max_execution_time') > 0) {
        $check_counter = 0;
        // Break the sending if a percentage of max execution time was exceeded.
        $elapsed = _simplenews_measure_usec();
        if ($elapsed > SIMPLENEWS_SEND_TIME_LIMIT * ini_get('max_execution_time')) {
          watchdog('simplenews', 'Sending interrupted: PHP maximum execution time almost exceeded. Remaining newsletters will be sent during the next cron run. If this warning occurs regularly you should reduce the !cron_throttle_setting.', array('!cron_throttle_setting' => l(t('Cron throttle setting'), 'admin/config/simplenews/mail')), WATCHDOG_WARNING);
          break;
        }
      }
    }

    // It is possible that all or at the end some results failed to get
    // prepared, report them separately.
    foreach ($spool->getProcessed() as $msid => $row) {
      $row_result = $row->result;
      simplenews_update_spool(array($msid), $row_result);
      if ($row_result['status'] == SIMPLENEWS_SPOOL_DONE) {
        $count_success++;
        if (isset($row->actual_nid)) {
          if (!isset($sent[$row->actual_nid])) {
            $sent[$row->actual_nid] = 1;
          }
          else {
            $sent[$row->actual_nid]++;
          }
        }
      }
      if ($row_result['error']) {
        $count_fail++;
      }
    }

    // Update subscriber count.
    foreach ($sent as $nid => $count) {
      db_update('simplenews_newsletter')
        ->condition('nid', $nid)
        ->expression('sent_subscriber_count', 'sent_subscriber_count + :count', array(':count' => $count))
        ->execute();
    }

    // Report sent result and elapsed time. On Windows systems getrusage() is
    // not implemented and hence no elapsed time is available.
    if (function_exists('getrusage')) {
      watchdog('simplenews', '%success emails sent in %sec seconds, %fail failed sending.', array('%success' => $count_success, '%sec' => round(_simplenews_measure_usec(), 1), '%fail' => $count_fail));
    }
    else {
      watchdog('simplenews', '%success emails sent, %fail failed.', array('%success' => $count_success, '%fail' => $count_fail));
    }

    variable_set('simplenews_last_cron', REQUEST_TIME);
    variable_set('simplenews_last_sent', $count_success);

    simplenews_revert_user();
    return $count_success;
  }
}

/**
 * Save mail message in mail cache table.
 *
 * @param array $spool
 *   The message to be stored in the spool table, as an array containing the
 *   following keys:
 *   - mail
 *   - nid
 *   - tid
 *   - status: (optional) Defaults to SIMPLENEWS_SPOOL_PENDING
 *   - time: (optional) Defaults to REQUEST_TIME.
 *
 * @ingroup spool
 */
function simplenews_save_spool($spool) {
  $status = isset($spool['status']) ? $spool['status'] : SIMPLENEWS_SPOOL_PENDING;
  $time = isset($spool['time']) ? $spool['time'] : REQUEST_TIME;

  db_insert('simplenews_mail_spool')
    ->fields(array(
      'mail' => $spool['mail'],
      'nid' => $spool['nid'],
      'tid' => $spool['tid'],
      'snid' => $spool['snid'],
      'status' => $status,
      'timestamp' => $time,
      'data' => serialize($spool['data']),
    ))
    ->execute();
}

/*
 * Returns the expiration time for IN_PROGRESS status.
 *
 * @return int
 *   A unix timestamp. Any IN_PROGRESS messages with a timestamp older than
 *   this will be re-allocated and re-sent.
 */
function simplenews_get_expiration_time() {
  $timeout = variable_get('simplenews_spool_progress_expiration', 3600);
  $expiration_time = REQUEST_TIME - $timeout;
  return $expiration_time;
}

/**
 * This function allocates messages to be sent in current run.
 *
 * Drupal acquire_lock guarantees that no concurrency issue happened.
 * If the message status is SIMPLENEWS_SPOOL_IN_PROGRESS but the maximum send
 * time has expired, the message id will be returned as a message which is not
 * allocated to another process.
 *
 * @param $limit
 *   (Optional) The maximum number of mails to load from the spool. Defaults to
 *   unlimited.
 * @param $conditions
 *   (Optional) Array of conditions which are applied to the query. If not set,
 *   status defaults to SIMPLENEWS_SPOOL_PENDING, SIMPLENEWS_SPOOL_IN_PROGRESS.
 *
 * @return
 *   An array of message ids to be sent in the current run.
 *
 * @ingroup spool
 */
function simplenews_get_spool($limit = SIMPLENEWS_UNLIMITED, $conditions = array()) {
  $messages = array();

  // Add default status condition if not set.
  if (!isset($conditions['status'])) {
    $conditions['status'] = array(SIMPLENEWS_SPOOL_PENDING, SIMPLENEWS_SPOOL_IN_PROGRESS);
  }

  // Special case for the status condition, the in progress actually only
  // includes spool items whose locking time has expired. So this need to build
  // an OR condition for them.
  $status_or = db_or();
  $statuses = is_array($conditions['status']) ? $conditions['status'] : array($conditions['status']);
  foreach ($statuses as $status) {
    if ($status == SIMPLENEWS_SPOOL_IN_PROGRESS) {
      $status_or->condition(db_and()
        ->condition('status', $status)
        ->condition('s.timestamp', simplenews_get_expiration_time(), '<')
      );
    }
    else {
      $status_or->condition('status', $status);
    }
  }
  unset($conditions['status']);

  $query = db_select('simplenews_mail_spool', 's')
    ->fields('s')
    ->condition($status_or)
    ->orderBy('s.timestamp', 'ASC');

  // Add conditions.
  foreach ($conditions as $field => $value) {
    $query->condition($field, $value);
  }

  /* BEGIN CRITICAL SECTION */
  // The semaphore ensures that multiple processes get different message ID's,
  // so that duplicate messages are not sent.

  if (lock_acquire('simplenews_acquire_mail')) {
    // Get message id's
    // Allocate messages
    if ($limit > 0) {
      $query->range(0, $limit);
    }
    foreach ($query->execute() as $message) {
      if (drupal_strlen($message->data)) {
        $message->data = unserialize($message->data);
      }
      else {
        $message->data = simplenews_subscriber_load_by_mail($message->mail);
      }
      $messages[$message->msid] = $message;
    }
    if (count($messages) > 0) {
      // Set the state and the timestamp of the messages
      simplenews_update_spool(
        array_keys($messages),
        array('status' => SIMPLENEWS_SPOOL_IN_PROGRESS)
      );
    }

    lock_release('simplenews_acquire_mail');
  }

  /* END CRITICAL SECTION */

  return $messages;
}

/**
 * Update status of mail data in spool table.
 *
 * Time stamp is set to current time.
 *
 * @param array $msids
 *   Array of Mail spool ids to be updated
 * @param array $data
 *   Array containing email sent results, with the following keys:
 *   - status: An integer indicating the updated status. Must be one of:
 *     - 0: hold
 *     - 1: pending
 *     - 2: send
 *     - 3: in progress
 *   - error: (optional) The error id.  Defaults to 0 (no error).
 *
 * @ingroup spool
 */
function simplenews_update_spool($msids, $data) {
  db_update('simplenews_mail_spool')
    ->condition('msid', $msids)
    ->fields(array(
      'status' => $data['status'],
      'error' => isset($result['error']) ? (int)$data['error'] : 0,
      'timestamp' => REQUEST_TIME,
    ))
    ->execute();
}

/**
 * Count data in mail spool table.
 *
 * @param $conditions
 *   (Optional) Array of conditions which are applied to the query. Defaults
 *
 * @return
 *   Count of mail spool elements of the passed in arguments.
 *
 * @ingroup spool
 */
function simplenews_count_spool(array $conditions = array()) {

  // Add default status condition if not set.
  if (!isset($conditions['status'])) {
    $conditions['status'] = array(SIMPLENEWS_SPOOL_PENDING, SIMPLENEWS_SPOOL_IN_PROGRESS);
  }

  $query = db_select('simplenews_mail_spool');
  // Add conditions.
  foreach ($conditions as $field => $value) {
    if ($field == 'status') {
      if (!is_array($value)) {
        $value = array($value);
      }
      $status_or = db_or();
      foreach ($value as $status) {
        // Do not count pending entries unless they are expired.
        if ($status == SIMPLENEWS_SPOOL_IN_PROGRESS) {
          $status_or->condition(db_and()
            ->condition('status', $status)
            ->condition('timestamp', simplenews_get_expiration_time(), '<')
          );
        }
        else {
          $status_or->condition('status', $status);
        }
      }
      $query->condition($status_or);
    }
    else {
      $query->condition($field, $value);
    }
  }

  $query->addExpression('COUNT(*)', 'count');

  return (int)$query
    ->execute()
    ->fetchField();
}

/**
 * Remove old records from mail spool table.
 *
 * All records with status 'send' and time stamp before the expiration date
 * are removed from the spool.
 *
 * @return
 *   Number of deleted spool rows.
 *
 * @ingroup spool
 */
function simplenews_clear_spool() {
  $expiration_time = REQUEST_TIME - variable_get('simplenews_spool_expire', 0) * 86400;
  return db_delete('simplenews_mail_spool')
    ->condition('status', SIMPLENEWS_SPOOL_DONE)
    ->condition('timestamp', $expiration_time, '<=')
    ->execute();
}

/**
 * Remove records from mail spool table according to the conditions.
 *
 * @return Count deleted
 *
 * @ingroup spool
 */
function simplenews_delete_spool(array $conditions) {
  $query = db_delete('simplenews_mail_spool');

  foreach ($conditions as $condition => $value) {
    $query->condition($condition, $value);
  }
  return $query->execute();
}


/**
 * Update newsletter sent status.
 *
 * Set newsletter sent status based on email sent status in spool table.
 * Translated and untranslated nodes get a different treatment.
 *
 * The spool table holds data for emails to be sent and (optionally)
 * already send emails. The simplenews_newsletter table contains the overall
 * sent status of each newsletter issue (node).
 * Newsletter issues get the status pending when sending is initiated. As
 * long as unsend emails exist in the spool, the status of the newsletter remains
 * unsend. When no pending emails are found the newsletter status is set 'send'.
 *
 * Translated newsletters are a group of nodes that share the same tnid ({node}.tnid).
 * Only one node of the group is found in the spool, but all nodes should share
 * the same state. Therefore they are checked for the combined number of emails
 * in the spool.
 *
 * @ingroup issue
 */
function simplenews_send_status_update() {
  $counts = array(); // number pending of emails in the spool
  $sum = array(); // sum of emails in the spool per tnid (translation id)
  $send = array(); // nodes with the status 'send'

  // For each pending newsletter count the number of pending emails in the spool.
  $query = db_select('simplenews_newsletter', 's');
  $query->innerJoin('node', 'n', 's.nid = n.nid');
  $query->fields('s', array('nid', 'tid'))
    ->fields('n', array('tnid'))
    ->condition('s.status', SIMPLENEWS_STATUS_SEND_PENDING);
  foreach ($query->execute() as $newsletter) {
    $counts[$newsletter->tnid][$newsletter->nid] = simplenews_count_spool(array('nid' => $newsletter->nid));
  }

  // Determine which nodes are send per translation group and per individual node.
  foreach ($counts as $tnid => $node_count) {
    // The sum of emails per tnid is the combined status result for the group of translated nodes.
    // Untranslated nodes have tnid == 0 which will be ignored later.
    $sum[$tnid] = array_sum($node_count);
    foreach ($node_count as $nid => $count) {
      // Translated nodes (tnid != 0)
      if ($tnid != '0' && $sum[$tnid] == '0') {
        $send[] = $nid;
      }
      // Untranslated nodes (tnid == 0)
      elseif ($tnid == '0' && $count == '0') {
        $send[] = $nid;
      }
    }
  }

  // Update overall newsletter status
  if (!empty($send)) {
    foreach ($send as $nid) {
      db_update('simplenews_newsletter')
        ->condition('nid', $nid)
        ->fields(array('status' => SIMPLENEWS_STATUS_SEND_READY))
        ->execute();
    }
  }
}

/**
 * Build formatted from-name and email for a mail object.
 *
 * @return Associative array with (un)formatted from address
 *  'address'   => From address
 *  'formatted' => Formatted, mime encoded, from name and address
 */
function _simplenews_set_from() {
  $address_default = variable_get('site_mail', ini_get('sendmail_from'));
  $name_default = variable_get('site_name', 'Drupal');

  $address = variable_get('simplenews_from_address', $address_default);
  $name = variable_get('simplenews_from_name', $name_default);

  // Windows based PHP systems don't accept formatted email addresses.
  $formatted_address = (drupal_substr(PHP_OS, 0, 3) == 'WIN') ? $address : '"'. addslashes(mime_header_encode($name)) .'" <'. $address .'>';

  return array(
    'address' => $address,
    'formatted' => $formatted_address,
  );
}

/**
 * HTML to text conversion for HTML and special characters.
 *
 * Converts some special HTML characters in addition to drupal_html_to_text()
 *
 * @param string $text
 *   The source text with HTML and special characters.
 * @param boolean $inline_hyperlinks
 *   TRUE: URLs will be placed inline.
 *   FALSE: URLs will be converted to numbered reference list.
 * @return string
 *   The target text with HTML and special characters replaced.
 */
function simplenews_html_to_text($text, $inline_hyperlinks = TRUE) {
  // By replacing <a> tag by only its URL the URLs will be placed inline
  // in the email body and are not converted to a numbered reference list
  // by drupal_html_to_text().
  // URL are converted to absolute URL as drupal_html_to_text() would have.
  if ($inline_hyperlinks) {
    $pattern = '@<a[^>]+?href="([^"]*)"[^>]*?>(.+?)</a>@is';
    $text = preg_replace_callback($pattern, '_simplenews_absolute_mail_urls', $text);
  }

  // Replace some special characters before performing the drupal standard conversion.
  $preg = _simplenews_html_replace();
  $text = preg_replace(array_keys($preg), array_values($preg), $text);

  // Perform standard drupal html to text conversion.
  return drupal_html_to_text($text);
}

/**
 * Helper function for simplenews_html_to_text().
 *
 * Replaces URLs with absolute URLs.
 */
function _simplenews_absolute_mail_urls($match) {
  global $base_url, $base_path;
  $regexp = &drupal_static(__FUNCTION__);
  $url = $label = '';

  if ($match) {
    if (empty($regexp)) {
      $regexp = '@^' . preg_quote($base_path, '@') . '@';
    }
    list(, $url, $label) = $match;
    $url = strpos($url, '://') ? $url : preg_replace($regexp, $base_url . '/', $url);

    // If the link is formed by Drupal's URL filter, we only return the URL.
    // The URL filter generates a label out of the original URL.
    if (strpos($label, '...') === drupal_strlen($label) - 3) {
      // Remove ellipsis from end of label.
      $label = drupal_substr($label, 0, drupal_strlen($label) - 3);
    }
    if (strpos($url, $label) !== FALSE) {
      return $url;
    }
    return $label . ' ' . $url;
  }
}

/**
 * Helper function for simplenews_html_to_text().
 *
 * List of preg* regular expression patterns to search for and replace with
 */
function _simplenews_html_replace() {
  return array(
    '/&quot;/i'  => '"',
    '/&gt;/i'    => '>',
    '/&lt;/i'    => '<',
    '/&amp;/i'   => '&',
    '/&copy;/i'  => '(c)',
    '/&trade;/i' => '(tm)',
    '/&#8220;/'  => '"',
    '/&#8221;/'  => '"',
    '/&#8211;/'  => '-',
    '/&#8217;/'  => "'",
    '/&#38;/'    => '&',
    '/&#169;/'   => '(c)',
    '/&#8482;/'  => '(tm)',
    '/&#151;/'   => '--',
    '/&#147;/'   => '"',
    '/&#148;/'   => '"',
    '/&#149;/'   => '*',
    '/&reg;/i'   => '(R)',
    '/&bull;/i'  => '*',
    '/&euro;/i'  => 'Euro ',
  );
}

/**
 * Helper function to measure PHP execution time in microseconds.
 *
 * @param bool $start
 *   If TRUE, reset the time and start counting.
 *
 * @return float
 *   The elapsed PHP execution time since the last start.
 */
function _simplenews_measure_usec($start = FALSE) {
  // Windows systems don't implement getrusage(). There is no alternative.
  if (!function_exists('getrusage')) {
    return;
  }

  $start_time = &drupal_static(__FUNCTION__);
  $usage = getrusage();
  $now = (float)($usage['ru_stime.tv_sec'] . '.' . $usage['ru_stime.tv_usec']) + (float)($usage['ru_utime.tv_sec'] . '.' . $usage['ru_utime.tv_usec']);

  if ($start) {
    $start_time = $now;
    return;
  }
  return $now - $start_time;
}


/**
 * Build subject and body of the test and normal newsletter email.
 *
 * @param array $message
 *   Message array as used by hook_mail().
 * @param array $source
 *   The SimplenewsSource instance.
 *
 * @ingroup source
 */
function simplenews_build_newsletter_mail(&$message, SimplenewsSourceInterface $source) {
  // Get message data from source.
  $message['headers'] = $source->getHeaders($message['headers']);
  $message['subject'] = $source->getSubject();
  $message['body']['body'] = $source->getBody();
  $message['body']['footer'] = $source->getFooter();

  // Optional params for HTML mails.
  if ($source->getFormat() == 'html') {
    $message['params']['plain'] = NULL;
    $message['params']['plaintext'] = $source->getPlainBody() . "\n" . $source->getPlainFooter();
    $message['params']['attachments'] = $source->getAttachments();
  }
  else {
    $message['params']['plain'] = TRUE;
  }
}

/**
 * Build subject and body of the subscribe confirmation email.
 *
 * @param array $message
 *   Message array as used by hook_mail().
 * @param array $params
 *   Parameter array as used by hook_mail().
 */
function simplenews_build_subscribe_mail(&$message, $params) {
  $context = $params['context'];
  $langcode = $message['language'];

  // Use formatted from address "name" <mail_address>
  $message['headers']['From'] = $params['from']['formatted'];

  $message['subject'] = simplenews_subscription_confirmation_text('subscribe_subject', $langcode);
  $message['subject'] = token_replace($message['subject'], $context, array('sanitize' => FALSE));

  if (simplenews_user_is_subscribed($context['simplenews_subscriber']->mail, $context['category']->tid)) {
    $body = simplenews_subscription_confirmation_text('subscribe_subscribed', $langcode);
  }
  else {
    $body = simplenews_subscription_confirmation_text('subscribe_unsubscribed', $langcode);
  }
    $message['body'][] = token_replace($body, $context, array('sanitize' => FALSE));
}

/**
 * Build subject and body of the subscribe confirmation email.
 *
 * @param array $message
 *   Message array as used by hook_mail().
 * @param array $params
 *   Parameter array as used by hook_mail().
 */
function simplenews_build_combined_mail(&$message, $params) {
  $context = $params['context'];
  $changes = $context['changes'];
  $langcode = $message['language'];

  // Use formatted from address "name" <mail_address>
  $message['headers']['From'] = $params['from']['formatted'];

  $message['subject'] = simplenews_subscription_confirmation_text('combined_subject', $langcode);
  $message['subject'] = token_replace($message['subject'], $context, array('sanitize' => FALSE));

  $changes_list = '';
  $actual_changes = 0;
  foreach (simplenews_confirmation_get_changes_list($context['simplenews_subscriber'], $changes, $langcode) as $tid => $change) {
    $changes_list .= ' - ' . $change . "\n";

    // Count the actual changes.
    $subscribed = simplenews_user_is_subscribed($context['simplenews_subscriber']->mail, $tid);
    if ($changes[$tid] == 'subscribe' && !$subscribed || $changes[$tid] == 'unsubscribe' && $subscribed) {
      $actual_changes++;
    }
  }

  // If there are actual changes, use the combined_body key otherwise use the
  // one without a confirmation link.
  $body_key = $actual_changes ? 'combined_body' : 'combined_body_unchanged';

  $body = simplenews_subscription_confirmation_text($body_key, $langcode);
  // The changes list is not an actual token.
  $body = str_replace('[changes-list]', $changes_list, $body);
  $message['body'][] = token_replace($body, $context, array('sanitize' => FALSE));
}

/**
 * Build subject and body of the unsubscribe confirmation email.
 *
 * @param array $message
 *   Message array as used by hook_mail().
 * @param array $params
 *   Parameter array as used by hook_mail().
 */
function simplenews_build_unsubscribe_mail(&$message, $params) {
  $context = $params['context'];
  $langcode = $message['language'];

  // Use formatted from address "name" <mail_address>
  $message['headers']['From'] = $params['from']['formatted'];

  $message['subject'] = simplenews_subscription_confirmation_text('subscribe_subject', $langcode);
  $message['subject'] = token_replace($message['subject'], $context, array('sanitize' => FALSE));

  if (simplenews_user_is_subscribed($context['simplenews_subscriber']->mail, $context['category']->tid)) {
    $body = simplenews_subscription_confirmation_text('unsubscribe_subscribed', $langcode);
    $message['body'][] = token_replace($body, $context, array('sanitize' => FALSE));
  }
  else {
    $body = simplenews_subscription_confirmation_text('unsubscribe_unsubscribed', $langcode);
    $message['body'][] = token_replace($body, $context, array('sanitize' => FALSE));
  }
}

/**
 * A mail sending implementation that captures sent messages to a variable.
 *
 * This class is for running tests or for development and does not convert HTML
 * to plaintext.
 */
class SimplenewsHTMLTestingMailSystem implements MailSystemInterface {

  /**
   * Implements MailSystemInterface::format().
   */
  public function format(array $message) {
    // Join the body array into one string.
    $message['body'] = implode("\n\n", $message['body']);
    // Wrap the mail body for sending.
    $message['body'] = drupal_wrap_mail($message['body']);
    return $message;
  }

  /**
   * Implements MailSystemInterface::mail().
   */
  public function mail(array $message) {
    $captured_emails = variable_get('drupal_test_email_collector', array());
    $captured_emails[] = $message;
    // @todo: This is rather slow when sending 100 and more mails during tests.
    // Investigate in other methods like APC shared memory.
    variable_set('drupal_test_email_collector', $captured_emails);
    return TRUE;
  }
}
