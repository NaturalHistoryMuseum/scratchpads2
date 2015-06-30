<?php

/**
 * @file
 * This file is loaded when handling submissions, either submitting new,
 * editing, or viewing. It also contains all CRUD functions for submissions.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

/**
 * Given an array of submitted values, flatten it into data for a submission.
 *
 * @param $node
 *   The node object containing the current webform.
 * @param $submitted
 *   The submitted user values from the webform.
 * @return
 *   An array suitable for use in the 'data' property of a $submission object.
 */
function webform_submission_data($node, $submitted) {
  $data = array();

  foreach ($submitted as $cid => $values) {
    // Don't save pseudo-fields or pagebreaks as submitted data.
    if (!isset($node->webform['components'][$cid]) || $node->webform['components'][$cid]['type'] == 'pagebreak') {
      continue;
    }

    if (is_array($values)) {
      $data[$cid] = $values;
    }
    else {
      $data[$cid][0] = $values;
    }
  }

  return $data;
}

/**
 * Given set of $form_state values, prepare a psuedo-submission.
 *
 * @param object $node
 *   The webform node object.
 * @param object $account
 *   The user account that is creating this submission.
 * @param array $form_state
 *   The form_state containing the values for the submission
 * @param object $prototype
 *   An existing submission that is being previewed, if any.
 * @return object
 *   A new submission object, possibly for preview
 */
function webform_submission_create($node, $account, $form_state, $is_preview = FALSE, $prototype = NULL) {
  $data = webform_submission_data($node, $form_state['values']['submitted']);
  if ($prototype) {
    $submission = clone $prototype;
    $submission->preview = $is_preview;
    $submission->data = $data;
  }
  else {
    $submission = (object) array(
      'nid' => $node->nid,
      'uid' => $account->uid,
      'sid' => NULL,
      'submitted' => REQUEST_TIME,
      'completed' => 0,
      'modified' => REQUEST_TIME,
      'remote_addr' => webform_ip_address($node),
      'is_draft' => TRUE,
      'highest_valid_page' => 0,
      'preview' => $is_preview,
      'serial' => NULL,
      'data' => $data,
    );
  drupal_alter('webform_submission_create', $submission, $node, $account, $form_state);
  }
  return $submission;
}

/**
 * Update a webform submission entry in the database.
 *
 * @param $node
 *   The node object containing the current webform.
 * @param $submission
 *   The webform submission object to be saved into the database.
 * @return
 *   The existing submission SID.
 */
function webform_submission_update($node, $submission) {
  // Allow other modules to modify the submission before saving.
  foreach (module_implements('webform_submission_presave') as $module) {
    $function = $module . '_webform_submission_presave';
    $function($node, $submission);
  }

  $submission->completed = empty($submission->completed) && !$submission->is_draft ? REQUEST_TIME : $submission->completed;
  $submission->modified = REQUEST_TIME;

  // Update the main submission info.
  drupal_write_record('webform_submissions', $submission, 'sid');

  // If is draft, only delete data for components submitted, to
  // preserve any data from form pages not visited in this submission.
  if ($submission->is_draft) {
    $submitted_cids = array_keys($submission->data);
    if ($submitted_cids) {
      db_delete('webform_submitted_data')
        ->condition('sid', $submission->sid)
        ->condition('cid', $submitted_cids, 'IN')
        ->execute();
    }
  }
  else {
    db_delete('webform_submitted_data')
      ->condition('sid', $submission->sid)
      ->execute();
  }

  // Then re-add submission data to the database.
  $submission->is_new = FALSE;
  webform_submission_insert($node, $submission);

  module_invoke_all('webform_submission_update', $node, $submission);

  return $submission->sid;
}

/**
 * Insert a webform submission entry in the database.
 *
 * @param $node
 *   The node object containing the current webform.
 * @param $submission
 *   The webform submission object to be saved into the database.
 * @return
 *   The new submission SID.
 */
function webform_submission_insert($node, $submission) {
  // The submission ID may already be set if being called as an update.
  if (!isset($submission->sid) && (!isset($submission->is_new) || $submission->is_new == FALSE)) {
    // Allow other modules to modify the submission before saving.
    foreach (module_implements('webform_submission_presave') as $module) {
      $function = $module . '_webform_submission_presave';
      $function($node, $submission);
    }
    $submission->nid = $node->webform['nid'];
    if (empty($submission->serial)) {
      $submission->serial = _webform_submission_serial_next_value($node->nid);
    }
    $submission->completed = empty($submission->completed) && !$submission->is_draft ? REQUEST_TIME : $submission->completed;
    drupal_write_record('webform_submissions', $submission);
    $is_new = TRUE;
  }

  foreach ($submission->data as $cid => $values) {
    foreach ($values as $delta => $value) {
      $data = array(
        'nid' => $node->webform['nid'],
        'sid' => $submission->sid,
        'cid' => $cid,
        'no' => $delta,
        'data' => is_null($value) ? '' : $value,
      );
      drupal_write_record('webform_submitted_data', $data);
    }
  }

  // Invoke the insert hook after saving all the data.
  if (isset($is_new)) {
    module_invoke_all('webform_submission_insert', $node, $submission);
  }

  return $submission->sid;
}

/**
 * Delete a single submission.
 *
 * @param $node
 *   The node object containing the current webform.
 * @param $submission
 *   The webform submission object to be deleted from the database.
 */
function webform_submission_delete($node, $submission) {
  // Iterate through all components and let each do cleanup if necessary.
  foreach ($node->webform['components'] as $cid => $component) {
    if (isset($submission->data[$cid])) {
      webform_component_invoke($component['type'], 'delete', $component, $submission->data[$cid]);
    }
  }

  // Delete any anonymous session information.
  if (isset($_SESSION['webform_submission'][$submission->sid])) {
    unset($_SESSION['webform_submission'][$submission->sid]);
  }

  db_delete('webform_submitted_data')
    ->condition('nid', $node->nid)
    ->condition('sid', $submission->sid)
    ->execute();
  db_delete('webform_submissions')
    ->condition('nid', $node->nid)
    ->condition('sid', $submission->sid)
    ->execute();

  module_invoke_all('webform_submission_delete', $node, $submission);
}

/**
 * Send related e-mails related to a submission.
 *
 * This function is usually invoked when a submission is completed, but may be
 * called any time e-mails should be redelivered.
 *
 * @param $node
 *   The node object containing the current webform.
 * @param $submission
 *   The webform submission object to be used in sending e-mails.
 * @param $emails
 *   (optional) An array of specific e-mail settings to be used. If omitted, all
 *   e-mails in $node->webform['emails'] will be sent.
 */
function webform_submission_send_mail($node, $submission, $emails = NULL) {
  global $user;

  // Get the list of e-mails we'll be sending.
  $emails = isset($emails) ? $emails : $node->webform['emails'];

  // Create a themed message for mailing.
  $send_count = 0;
  foreach ($emails as $eid => $email) {
    // Set the HTML property based on availablity of MIME Mail.
    $email['html'] = ($email['html'] && webform_variable_get('webform_email_html_capable'));

    // Pass through the theme layer if using the default template.
    if ($email['template'] == 'default') {
      $email['message'] = theme(array('webform_mail_' . $node->nid, 'webform_mail', 'webform_mail_message'), array('node' => $node, 'submission' => $submission, 'email' => $email));
    }
    else {
      $email['message'] = $email['template'];
    }

    // Replace tokens in the message.
    $email['message'] = webform_replace_tokens($email['message'], $node, $submission, $email, (boolean)$email['html']);

    // Build the e-mail headers.
    $email['headers'] = theme(array('webform_mail_headers_' . $node->nid, 'webform_mail_headers'), array('node' => $node, 'submission' => $submission, 'email' => $email));

    // Assemble the From string.
    if (isset($email['headers']['From'])) {
      // If a header From is already set, don't override it.
      $email['from'] = $email['headers']['From'];
      unset($email['headers']['From']);
    }
    else {
      // Format the From address.
      $mapping = isset($email['extra']['from_address_mapping']) ?  $email['extra']['from_address_mapping'] : NULL;
      $email['from'] = webform_format_email_address($email['from_address'], $email['from_name'], $node, $submission, TRUE, TRUE, NULL, $mapping);
    }

    // If requested and not already set, set Reply-To to the From and re-format From address.
    if (webform_variable_get('webform_email_replyto') &&
        empty($email['headers']['Reply-To']) &&
        ($default_from_name = webform_variable_get('webform_default_from_name')) &&
        ($default_from_address = webform_variable_get('webform_default_from_address')) &&
        ($default_from_parts = explode('@', $default_from_address)) &&
        count($default_from_parts) == 2 &&
        $default_from_parts[1] &&
        stripos($email['from'], '@' . $default_from_parts[1]) === FALSE) {
      // Message is not already being sent from the domain of the default
      // webform from address.
      $email['headers']['Reply-To'] = $email['from'];
      $email['from'] = $default_from_address;
      if (webform_variable_get('webform_email_address_format') == 'long') {
        $email_parts = webform_parse_email_address($email['headers']['Reply-To']);
        $from_name = t('!name via !site_name',
                        array('!name' => strlen($email_parts['name']) ? $email_parts['name'] : $email_parts['address'],
                              '!site_name' => $default_from_name));
        $from_name = implode(' ', array_map('mime_header_encode', explode(' ', $from_name)));
        $email['from'] = '"' . $from_name . '" <' . $email['from'] . '>';
      }
    }

    // Update the subject if set in the themed headers.
    if (isset($email['headers']['Subject'])) {
      $email['subject'] = $email['headers']['Subject'];
      unset($email['headers']['Subject']);
    }
    else {
      $email['subject'] = webform_format_email_subject($email['subject'], $node, $submission);
    }

    // Update the to e-mail if set in the themed headers.
    if (isset($email['headers']['To'])) {
      $email['email'] = $email['headers']['To'];
      unset($email['headers']['To']);
      $addresses = array_filter(explode(',', $email['email']));
    }
    else {
      // Format the To address(es).
      $mapping = isset($email['extra']['email_mapping']) ?  $email['extra']['email_mapping'] : NULL;
      $addresses = webform_format_email_address($email['email'], NULL, $node, $submission, TRUE, FALSE, NULL, $mapping);
      $email['email'] = implode(',', $addresses);
    }

    // Generate the list of addresses that this e-mail will be sent to.
    $addresses_final = array_filter($addresses, 'webform_valid_email_address');

    if (!$addresses_final) {
      continue;
    }

    // Verify that this submission is not attempting to send any spam hacks.
    foreach ($addresses_final as $address) {
      if (_webform_submission_spam_check($address, $email['subject'], $email['from'], $email['headers'])) {
        watchdog('webform', 'Possible spam attempt from @remote !message',
                 array('@remote' => ip_address(), '!message'=> "<br />\n" . nl2br(htmlentities($email['message']))));
        drupal_set_message(t('Illegal information. Data not submitted.'), 'error');
        return FALSE;
      }
    }

    // Consolidate addressees into one message if permitted by configuration.
    $send_increment = 1;
    if (!webform_variable_get('webform_email_address_individual')) {
      $send_increment = count($addresses_final);
      $addresses_final = array(implode(', ', $addresses_final));
    }

    // Mail the webform results.
    foreach ($addresses_final as $address) {

      $language = $user->uid ? user_preferred_language($user) : language_default();
      $mail_params = array(
        'message' => $email['message'],
        'subject' => $email['subject'],
        'headers' => $email['headers'],
        'node' => $node,
        'submission' => $submission,
        'email' => $email,
      );

      if (webform_variable_get('webform_email_html_capable')) {
        // Load attachments for the e-mail.
        $attachments = array();
        if ($email['attachments']) {
          webform_component_include('file');
          foreach ($node->webform['components'] as $component) {
            if (webform_component_feature($component['type'], 'attachment') && !empty($submission->data[$component['cid']][0])) {
              if (webform_component_implements($component['type'], 'attachments')) {
                $files = webform_component_invoke($component['type'], 'attachments', $component, $submission->data[$component['cid']]);
                if ($files) {
                  $attachments = array_merge($attachments, $files);
                }
              }
            }
          }
        }

        // Add the attachments to the mail parameters.
        $mail_params['attachments'] = $attachments;

        // Set all other properties for HTML e-mail handling.
        $mail_params['plain'] = !$email['html'];
        $mail_params['plaintext'] = $email['html'] ? NULL : $email['message'];
        $mail_params['headers'] = $email['headers'];
        if ($email['html']) {
          // MIME Mail requires this header or it will filter all text.
          $mail_params['headers']['Content-Type'] = 'text/html; charset=UTF-8';
        }
      }

      // Mail the submission.
      $message = drupal_mail('webform', 'submission', $address, $language, $mail_params, $email['from']);
      if ($message['result']) {
        $send_count += $send_increment;
      }
    }
  }

  return $send_count;
}

/**
 * Confirm form to delete a single form submission.
 *
 * @param $form
 *   The new form array.
 * @param $form_state
 *   The current form state.
 * @param $node
 *   The node for which this webform was submitted.
 * @param $submission
 *   The submission to be deleted (from webform_submitted_data).
 */
function webform_submission_delete_form($form, $form_state, $node, $submission) {
  webform_set_breadcrumb($node, $submission);

  // Set the correct page title.
  drupal_set_title(webform_submission_title($node, $submission));

  // Keep the NID and SID in the same location as the webform_client_form().
  // This helps mollom identify the same fields when deleting a submission.
  $form['#tree'] = TRUE;
  $form['details']['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['details']['sid'] = array(
    '#type' => 'value',
    '#value' => $submission->sid,
  );

  $question = t('Are you sure you want to delete this submission?');

  return confirm_form($form, NULL, "node/{$node->nid}/submission/{$submission->sid}", $question, t('Delete'), t('Cancel'));
}

function webform_submission_delete_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['details']['nid']);
  $submission = webform_get_submission($form_state['values']['details']['nid'], $form_state['values']['details']['sid']);
  webform_submission_delete($node, $submission);
  drupal_set_message(t('Submission deleted.'));

  // If no destination query was supplied in the URL (e.g. Edit tab), redirect
  // to the most-privledged destination.
  $form_state['redirect'] = 'node/' . $node->nid .
                            (webform_results_access($node) ? '/webform-results' : '/submissions');
}

/**
 * Menu title callback; Return the submission number as a title.
 */
function webform_submission_title($node, $submission) {
  return t('Submission #@serial', array('@serial' => $submission->serial));
}

/**
 * Menu callback; Present a Webform submission page for display or editing.
 */
function webform_submission_page($node, $submission, $format) {
  global $user;

  // Render the admin UI breadcrumb.
  webform_set_breadcrumb($node, $submission);

  // Set the correct page title.
  drupal_set_title(webform_submission_title($node, $submission));

  if ($format == 'form') {
    $output = drupal_get_form('webform_client_form_' . $node->nid, $node, $submission);
  }
  else {
    $renderable = webform_submission_render($node, $submission, NULL, $format);
    $renderable['#attached']['css'][] = drupal_get_path('module', 'webform') . '/css/webform.css';
    $output = drupal_render($renderable);
  }

  // Determine the mode in which we're displaying this submission.
  $mode = ($format != 'form') ? 'display' : 'form';
  if (strpos(request_uri(), 'print/') !== FALSE) {
    $mode = 'print';
  }
  if (strpos(request_uri(), 'printpdf/') !== FALSE) {
    $mode = 'pdf';
  }

  // Add navigation for administrators.
  if (webform_results_access($node)) {
    $navigation = theme('webform_submission_navigation', array('node' => $node, 'submission' => $submission, 'mode' => $mode));
    $information = theme('webform_submission_information', array('node' => $node, 'submission' => $submission, 'mode' => $mode));
  }
  else {
    $navigation = NULL;
    $information = NULL;
  }

  // Actions may be shown to all users.
  $actions = theme('links', array('links' => module_invoke_all('webform_submission_actions', $node, $submission), 'attributes' => array('class' => array('links', 'inline', 'webform-submission-actions'))));

  // Disable the page cache for anonymous users viewing or editing submissions.
  if (!$user->uid) {
    webform_disable_page_cache();
  }

  $page = array(
    '#theme' => 'webform_submission_page',
    '#node' => $node,
    '#mode' => $mode,
    '#submission' => $submission,
    '#submission_content' => $output,
    '#submission_navigation' => $navigation,
    '#submission_information' => $information,
    '#submission_actions' => $actions,
  );
  $page['#attached']['library'][] = array('webform', 'admin');
  return $page;
}

/**
 * Form to resend specific e-mails associated with a submission.
 */
function webform_submission_resend($form, $form_state, $node, $submission) {
  // Render the admin UI breadcrumb.
  webform_set_breadcrumb($node, $submission);

  $form['#tree'] = TRUE;
  $form['#node'] = $node;
  $form['#submission'] = $submission;

  foreach ($node->webform['emails'] as $eid => $email) {

    $mapping = isset($email['extra']['email_mapping']) ?  $email['extra']['email_mapping'] : NULL;
    $addresses = webform_format_email_address($email['email'], NULL, $node, $submission, FALSE, FALSE, NULL, $mapping);
    $addresses_valid = array_map('webform_valid_email_address', $addresses);
    $valid_email = count($addresses) == array_sum($addresses_valid);
    
    $form['resend'][$eid] = array(
      '#type' => 'checkbox',
      '#default_value' => $valid_email ? TRUE : FALSE,
      '#disabled' => $valid_email ? FALSE : TRUE,
    );
    $form['emails'][$eid]['email'] = array(
      '#markup' => nl2br(check_plain(implode("\n", $addresses))),
    );
    if (!$valid_email) {
      $form['emails'][$eid]['email']['#markup'] .= ' (' . t('empty or invalid') . ')';
    }
    $form['emails'][$eid]['subject'] = array(
      '#markup' => check_plain(webform_format_email_subject($email['subject'], $node, $submission)),
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Resend e-mails'),
    );
    $form['actions']['cancel'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Cancel'), isset($_GET['destination']) ? $_GET['destination'] : 'node/' . $node->nid . '/submission/' . $submission->sid),
    );
  }
  return $form;
}

/**
 * Validate handler for webform_submission_resend().
 */
function webform_submission_resend_validate($form, &$form_state) {
  if (count(array_filter($form_state['values']['resend'])) == 0) {
    form_set_error('emails', t('You must select at least one email address to resend submission.'));
  }
}

/**
 * Submit handler for webform_submission_resend().
 */
function webform_submission_resend_submit($form, &$form_state) {
  $node = $form['#node'];
  $submission = $form['#submission'];

  $emails = array();
  foreach ($form_state['values']['resend'] as $eid => $checked) {
    if ($checked) {
      $emails[] = $form['#node']->webform['emails'][$eid];
    }
  }
  $sent_count = webform_submission_send_mail($node, $submission, $emails);
  if ($sent_count) {
    drupal_set_message(format_plural($sent_count,
      'Successfully re-sent submission #@sid to 1 recipient.',
      'Successfully re-sent submission #@sid to @count recipients.',
      array('@sid' => $submission->sid)
    ));
  }
  else {
    drupal_set_message(t('No e-mails were able to be sent due to a server error.'), 'error');
  }
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @param $form
 *   The form array.
 * @return
 *   Formatted HTML form, ready for display.
 */
function theme_webform_submission_resend($variables) {
  $form = $variables['form'];

  $header = array('', t('E-mail to'), t('Subject'));
  $rows = array();
  if (!empty($form['emails'])) {
    foreach (element_children($form['emails']) as $eid) {
      // Add each component to a table row.
      $rows[] = array(
        drupal_render($form['resend'][$eid]),
        drupal_render($form['emails'][$eid]['email']),
        drupal_render($form['emails'][$eid]['subject']),
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('This webform is currently not setup to send emails.'), 'colspan' => 3));
  }
  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE, 'attributes' => array('id' => 'webform-emails')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Print a Webform submission for display on a page or in an e-mail.
 */
function webform_submission_render($node, $submission, $email, $format, $excluded_components = NULL) {
  $component_tree = array();
  $renderable = array();
  $page_count = 1;

  // Meta data that may be useful for modules implementing
  // hook_webform_submission_render_alter().
  $renderable['#node'] = $node;
  $renderable['#submission'] = $submission;
  $renderable['#email'] = $email;
  $renderable['#format'] = $format;

  // Set the theme function for submissions.
  $renderable['#theme'] = array('webform_submission_' . $node->nid, 'webform_submission');

  $components = $node->webform['components'];
  
  // Remove excluded components.
  if (is_array($excluded_components)) {
    foreach ($excluded_components as $cid) {
      unset($components[$cid]);
    }
    if ($email && $email['exclude_empty']) {
      foreach ($submission->data as $cid => $data) {
        if (!isset($data[0]) || $data[0] == '') {
          unset($components[$cid]);
        }
      }
    }
  }

  module_load_include('inc', 'webform', 'includes/webform.components');
  _webform_components_tree_build($components, $component_tree, 0, $page_count);

  // Make sure at least one field is available
  if (isset($component_tree['children'])) {
    // Recursively add components to the form.
    $sorter = webform_get_conditional_sorter($node);
    $input_values = $sorter->executeConditionals($submission->data);
    foreach ($component_tree['children'] as $cid => $component) {
      if ($sorter->componentVisibility($cid, $component['page_num']) == webformConditionals::componentShown) {
        _webform_client_form_add_component($node, $component, NULL, $renderable, $renderable, $input_values, $format);
      }
    }
  }

  drupal_alter('webform_submission_render', $renderable);
  return $renderable;
}

/**
 * Return all the submissions for a particular node.
 *
 * @param $filters
 *   An array of filters to apply to this query. Usually in the format
 *   array('nid' => $nid, 'uid' => $uid). A single integer may also be passed
 *   in, which will be equivalent to specifying a $nid filter.
 * @param $header
 *   If the results of this fetch will be used in a sortable
 *   table, pass the array header of the table.
 * @param $pager_count
 *   Optional. The number of submissions to include in the results.
 */
function webform_get_submissions($filters = array(), $header = NULL, $pager_count = 0) {
  return webform_get_submissions_load(webform_get_submissions_query($filters, $header, $pager_count));
}

/**
 * Returns an unexecuted webform_submissions query on for the arguments.
 *
 * This is an internal routine and not intended for use by other modules.
 *
 * @param $filters
 *   An array of filters to apply to this query. Usually in the format
 *   array('nid' => $nid, 'uid' => $uid). A single integer may also be passed
 *   in, which will be equivalent to specifying a $nid filter. 'sid' may also
 *   be included, either as a single sid or an array of sid's.
 * @param $header
 *   If the results of this fetch will be used in a sortable
 *   table, pass the array header of the table.
 * @param $pager_count
 *   Optional. The number of submissions to include in the results.
 */
function webform_get_submissions_query($filters = array(), $header = NULL, $pager_count = 0) {
  if (!is_array($filters)) {
    $filters = array('ws.nid' => $filters);
  }

  // Qualify all filters with a table alias. ws.* is assumed, except for u.uid.
  foreach ($filters as $column => $value) {
    if (strpos($column, '.') === FALSE) {
      $filters[($column == 'uid' ? 'u.' : 'ws.') . $column] = $value;
      unset($filters[$column]);
    }
  }

  // If the sid is specified, but there are none, force the query to fail
  // rather than query on an empty array
  if (isset($filters['ws.sid']) && empty($filters['ws.sid'])) {
    $filters['ws.sid'] = 0;
  }

  // Build the list of submissions and load their basic information.
  $pager_query = db_select('webform_submissions', 'ws')
    // Ensure only one row per submission is returned. Could be more than one if
    // sorting on a column that uses multiple rows for its data.
    ->distinct()
    ->addTag('webform_get_submissions_sids')
    ->fields('ws');

  // Add each filter.
  foreach ($filters as $column => $value) {
    $pager_query->condition($column, $value);
  }

  // Join to the users table to include user name in results.
  $pager_query->leftJoin('users', 'u', 'u.uid = ws.uid');
  $pager_query->fields('u', array('name'));
  if (isset($filters['u.uid']) && $filters['u.uid'] === 0) {
    if (!empty($_SESSION['webform_submission'])) {
      $anonymous_sids = array_keys($_SESSION['webform_submission']);
      $pager_query->condition('ws.sid', $anonymous_sids, 'IN');
    }
    else {
      $pager_query->condition('ws.sid', 0);
    }
  }

  if (is_array($header)) {
    $metadata_columns = array();
    foreach ($header as $header_item) {
      $metadata_columns[] = $header_item['data'];
    }
    $sort = drupal_get_query_parameters();
    // Sort by submitted data column if order is set but not in
    // $metadata_columns.
    if (isset($sort['order']) && !in_array($sort['order'], $metadata_columns, TRUE)) {
      // Default if sort is unset or invalid.
      if (!isset($sort['sort']) || !in_array($sort['sort'], array('asc', 'desc'), TRUE)) {
        $sort['sort'] = '';
      }
      $pager_query->leftJoin('webform_component', 'wc', 'ws.nid = wc.nid AND wc.name = :form_key', array('form_key' => $sort['order']));
      $pager_query->leftJoin('webform_submitted_data', 'wsd', 'wc.nid = wsd.nid AND ws.sid = wsd.sid AND wc.cid = wsd.cid');
      $pager_query->orderBy('wsd.data', $sort['sort']);
      $pager_query->orderBy('ws.sid', 'ASC');
    }
    // Sort by metadata column.
    else {
      // Extending the query instantiates a new query object.
      $pager_query = $pager_query->extend('TableSort');
      $pager_query->orderByHeader($header);
    }
  }
  else {
    $pager_query->orderBy('ws.sid', 'ASC');
  }

  if ($pager_count) {
    // Extending the query instantiates a new query object.
    $pager_query = $pager_query->extend('PagerDefault');
    $pager_query->limit($pager_count);
  }
  return $pager_query;
}

/**
 * Retrieve and load the submissions for the specified submissions query.
 *
 * This is an internal routine and not intended for use by other modules.
 *
 * @params object $pager_query
 *   A select or extended select query containing the needed fields:
 *     webform_submissions: all fields
 *     user: name
 * @return array
 *   An array of loaded webform submissions.
 */
function webform_get_submissions_load($pager_query) {
  // If the "$pager_query" is actually an unextended select query, then instead
  // of querying the webform_submissions_data table with a pontentially huge
  // array of sids in an IN clause, use the select query directy as this will
  // be much faster. Extended queries don't work in join clauses. The query
  // is assumed to include the sid.
  if ($pager_query instanceof SelectQuery) {
    $submissions_query = clone $pager_query;
  }

  // Extract any filter on node id to use in an optimization below
  foreach ($pager_query->conditions() as $index => $condition) {
    if ($index !== '#conjunction' && $condition['operator'] === '=' && ($condition['field'] === 'nid' || $condition['field'] === 'ws.nid')) {
      $nid = $condition['value'];
      break;
    }
  }

  $result = $pager_query->execute();
  $submissions = $result->fetchAllAssoc('sid');

  // If there are no submissions being retrieved, return an empty array.
  if (!$submissions) {
    return $submissions;
  }

  foreach ($submissions as $sid => $submission) {
    $submissions[$sid]->preview = FALSE;
    $submissions[$sid]->data = array();
  }

  // Query the required submission data.
  $query = db_select('webform_submitted_data', 'sd');
  $query
    ->addTag('webform_get_submissions_data')
    ->fields('sd', array('sid', 'cid', 'no', 'data'))
    ->orderBy('sd.sid', 'ASC')
    ->orderBy('sd.cid', 'ASC')
    ->orderBy('sd.no', 'ASC');

  if (isset($submissions_query)) {
    // If available, prefer joining on the subquery as it is much faster than an
    // IN clause on a large array. A subquery with the IN operator doesn't work
    // when the subquery has a LIMIT clause, requiring an inner join instead.
    $query->innerJoin($submissions_query, 'ws2', 'sd.sid = ws2.sid');
  }
  else {
    $query->condition('sd.sid',  array_keys($submissions), 'IN');
  }

  // By adding the NID to this query we allow MySQL to use the primary key on
  // in webform_submitted_data for sorting (nid_sid_cid_no).
  if (isset($nid)) {
    $query->condition('sd.nid', $nid);
  }

  $result = $query->execute();

  // Convert the queried rows into submission data.
  foreach ($result as $row) {
    $submissions[$row->sid]->data[$row->cid][$row->no] = $row->data;
  }

  foreach (module_implements('webform_submission_load') as $module) {
    $function = $module . '_webform_submission_load';
    $function($submissions);
  }

  return $submissions;
}

/**
 * Return a count of the total number of submissions for a node.
 *
 * @param $nid
 *   The node ID for which submissions are being fetched.
 * @param $uid
 *   Optional; the user ID to filter the submissions by.
 * @param $is_draft
 *   Optional; NULL for all, truthy for drafts only, falsy for completed only.
 *   The default is completed submissions only.
 * @return
 *   An integer value of the number of submissions.
 */
function webform_get_submission_count($nid, $uid = NULL, $is_draft = 0) {
  $counts = &drupal_static(__FUNCTION__);

  if (!isset($counts[$nid][$uid])) {
    $query = db_select('webform_submissions', 'ws')
      ->addTag('webform_get_submission_count')
      ->condition('ws.nid', $nid);
    if ($uid !== NULL) {
      $query->condition('ws.uid', $uid);
    }
    if ($uid === 0) {
      $submissions = isset($_SESSION['webform_submission']) ? $_SESSION['webform_submission'] : NULL;
      if ($submissions) {
        $query->condition('ws.sid', $submissions, 'IN');
      }
      else {
        // Intentionally never match anything if the anonymous user has no
        // submissions.
        $query->condition('ws.sid', 0);
      }
    }
    if (isset($is_draft)) {
      $query->condition('ws.is_draft', $is_draft);
    }

    $counts[$nid][$uid] = $query->countQuery()->execute()->fetchField();
  }
  return $counts[$nid][$uid];
}

/**
 * Fetch a specified submission for a webform node.
 */
function webform_get_submission($nid, $sid) {
  $submissions = &drupal_static(__FUNCTION__, array());

  // Load the submission if needed.
  if (!isset($submissions[$sid])) {
    $new_submissions = webform_get_submissions(array('nid' => $nid, 'sid' => $sid));
    $submissions[$sid] = isset($new_submissions[$sid]) ? $new_submissions[$sid] : FALSE;
  }

  return $submissions[$sid];
}

function _webform_submission_spam_check($to, $subject, $from, $headers = array()) {
  $headers = implode('\n', (array)$headers);
  // Check if they are attempting to spam using a bcc or content type hack.
  if (preg_match('/(b?cc\s?:)|(content\-type:)/i', $to . "\n" . $subject . "\n" . $from . "\n" . $headers)) {
    return TRUE; // Possible spam attempt.
  }
  return FALSE; // Not spam.
}

/**
 * Check if the current user has exceeded the limit on this form.
 *
 * @param $node
 *   The webform node to be checked.
 * @param $account
 *   Optional parameter. Specify the account you want to check the limit
 *   against.
 *
 * @return
 *   Boolean TRUE if the user has exceeded their limit. FALSE otherwise.
 */
function webform_submission_user_limit_check($node, $account = NULL) {
  global $user;
  $tracking_mode = webform_variable_get('webform_tracking_mode');

  if (!isset($account)) {
    $account = $user;
  }

  // We can only check access for anonymous users through their cookies.
  if ($user->uid !== 0 && $account->uid === 0) {
    watchdog('webform', 'Unable to check anonymous user submission limit when logged in as user @uid.', array('@uid' => $user->uid), WATCHDOG_WARNING);
    return FALSE;
  }

  // Check if submission limiting is enabled.
  if ($node->webform['submit_limit'] == '-1') {
    return FALSE; // No check enabled.
  }

  // Fetch all the entries from the database within the submit interval with
  // this username and IP.
  $num_submissions_database = 0;
  if (!$node->webform['confidential'] &&
      ($account->uid !== 0 || $tracking_mode === 'ip_address' || $tracking_mode === 'strict')) {
    $query = db_select('webform_submissions')
      ->addTag('webform_submission_user_limit_check')
      ->condition('nid', $node->nid)
      ->condition('is_draft', 0);

    if ($node->webform['submit_interval'] != -1) {
      $query->condition('submitted', REQUEST_TIME - $node->webform['submit_interval'], '>');
    }

    if ($account->uid) {
      $query->condition('uid', $account->uid);
    }
    else {
      $query->condition('remote_addr', ip_address());
    }
    $num_submissions_database = $query->countQuery()->execute()->fetchField();
  }

  // Double check the submission history from the users machine using cookies.
  $num_submissions_cookie = 0;
  if ($account->uid === 0 && ($tracking_mode === 'cookie' || $tracking_mode === 'strict')) {
    $cookie_name = 'webform-' . $node->nid;

    if (isset($_COOKIE[$cookie_name]) && is_array($_COOKIE[$cookie_name])) {
      foreach ($_COOKIE[$cookie_name] as $key => $timestamp) {
        if ($node->webform['submit_interval'] != -1 && $timestamp <= REQUEST_TIME - $node->webform['submit_interval']) {
          // Remove the cookie if past the required time interval.
          $params = session_get_cookie_params();
          setcookie($cookie_name . '[' . $key . ']', '', 0, $params['path'], $params['domain'], $params['secure'], $params['httponly']);
        }
      }
      // Count the number of submissions recorded in cookies.
      $num_submissions_cookie = count($_COOKIE[$cookie_name]);
    }
  }

  if ($num_submissions_database >= $node->webform['submit_limit'] || $num_submissions_cookie >= $node->webform['submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}

/**
 * Check if the total number of submissions has exceeded the limit on this form.
 *
 * @param $node
 *   The webform node to be checked.
 * @return
 *   Boolean TRUE if the form has exceeded it's limit. FALSE otherwise.
 */
function webform_submission_total_limit_check($node) {

  // Check if submission limiting is enabled.
  if ($node->webform['total_submit_limit'] == '-1') {
    return FALSE; // No check enabled.
  }

  // Retrieve submission data from the database.
  $query = db_select('webform_submissions')
    ->addTag('webform_submission_total_limit_check')
    ->condition('nid', $node->nid)
    ->condition('is_draft', 0);

  if ($node->webform['total_submit_interval'] != -1) {
    $query->condition('submitted', REQUEST_TIME - $node->webform['total_submit_interval'], '>');
  }

  // Fetch all the entries from the database within the submit interval.
  $num_submissions_database = $query->countQuery()->execute()->fetchField();

  if ($num_submissions_database >= $node->webform['total_submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}

/**
 * Preprocess function for webform-submission.tpl.php.
 */
function template_preprocess_webform_submission(&$vars) {
  $vars['node'] = $vars['renderable']['#node'];
  $vars['submission'] = $vars['renderable']['#submission'];
  $vars['email'] = $vars['renderable']['#email'];
  $vars['format'] = $vars['renderable']['#format'];
}

/**
 * Preprocess function for webform-submission-navigation.tpl.php.
 */
function template_preprocess_webform_submission_navigation(&$vars) {
  $start_path = ($vars['mode'] == 'print') ? 'print/' : 'node/';

  $previous_query = db_select('webform_submissions')
    ->condition('nid', $vars['node']->nid)
    ->condition('sid', $vars['submission']->sid, '<');
  $previous_query->addExpression('MAX(sid)');

  $next_query = db_select('webform_submissions')
    ->condition('nid', $vars['node']->nid)
    ->condition('sid', $vars['submission']->sid, '>');
  $next_query->addExpression('MIN(sid)');

  $vars['previous'] = $previous_query->execute()->fetchField();
  $vars['next'] = $next_query->execute()->fetchField();
  $vars['previous_url'] = $start_path . $vars['node']->nid . '/submission/' . $vars['previous'] . ($vars['mode'] == 'form' ? '/edit' : '');
  $vars['next_url'] = $start_path . $vars['node']->nid . '/submission/' . $vars['next'] . ($vars['mode'] == 'form' ? '/edit' : '');
}

/**
 * Preprocess function for webform-submission-navigation.tpl.php.
 */
function template_preprocess_webform_submission_information(&$vars) {
  $vars['account'] = user_load($vars['submission']->uid);
  $vars['actions'] = theme('links', module_invoke_all('webform_submission_actions', $vars['node'], $vars['submission']));
}
