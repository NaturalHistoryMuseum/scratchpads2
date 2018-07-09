<?php

/**
 * Presents a user with two forms on a tab on their account page.
 * 
 * The first form allows them to subscribe to all or selected forums on the site
 * The second form allows a user to add additional email addresses to associate
 * with their account.
 */
function femail_user_settings($account){
  // Title of the page is the user's name
  drupal_set_title(check_plain($account->name));
  if(!user_access('post by femail', $account)){
    drupal_set_message(t('This user account can not post messages to the forum by email'), 'error');
  }
  // ---------------- Subscribe to forums --------------------------------------
  // $output = rendered HTML
  $output = theme('femail_box', array(
    'title' => t('Subscriptions'),
    'content' => drupal_render(drupal_get_form('femail_user_subscribe', $account))
  ));
  // ---------------- Add additional emails ------------------------------------
  // Table for additional emails
  $header = array(
    t('Email'),
    t('Status'),
    t('Operations')
  );
  $rows = array();
  $results = db_select('femail_user_emails', 'f')->condition('uid', $account->uid)->fields('f', array(
    'status',
    'email'
  ))->execute();
  // List each of the email addresses associated with the account as a row.
  foreach($results as $mail_bits){
    $status = $mail_bits->status ? t('Verified') : t('Not verified');
    $rows[] = array(
      check_plain($mail_bits->email),
      $status,
      l(t('Remove'), 'femail/remove/' . $account->uid . '/' . $mail_bits->email)
    );
  }
  // Theme the table, along with the form
  $output .= theme('femail_box', array(
    'title' => t('Your additional email addresses'),
    'content' => theme('table', array(
      'header' => $header,
      'rows' => $rows
    )) . drupal_render(drupal_get_form('femail_user_add', $account))
  ));
  // Return both forms
  return $output;
}

/**
 * Simple replacement for Drupal 6 theme_box "theme('box', boxy shit)".
 * 
 * Interestingly, there is still an issue open on Drupal.org explaining that
 * they should document the fact this theme was removed.
 */
function theme_femail_box($variables){
  $output = '<h2 class="title">' . $variables['title'] . '</h2><div>' . $variables['content'] . '</div>';
  return $output;
}

/**
 * Simple callback that will verify an email.  The link is intended to be
 * clicked on from an email, so it's fine to redirect to the front page.
 * Verify an email
 */
function femail_email_verify($email, $hash){
  // Check the hash is correct.
  if(md5($email . variable_get('femail_install_time', 'INSTALLTIME')) == $hash){
    // Correct - we should mark this email as verified.
    db_update('femail_user_emails')->condition('email', $email)->fields(array('status' => 1))->execute();
    drupal_set_message(t('Email verified'));
  }else{
    // Hash failed - we'll add an error message and log it to the watchdog.
    watchdog('femail', 'Verification of an email address failed. Details: !ipaddress , !email , !hash', array(
      '!ipaddress' => ip_address(),
      '!email' => $email,
      '!hash' => $hash
    ), WATCHDOG_NOTICE);
    drupal_set_message(t('Email verification failed'));
  }
  drupal_goto();
}

/**
 * FIXME - This needs to be improved to ask the user "Are you sure?"
 * 
 * This currently removes an email address from the femail_user_emails table
 * without asking for any approval.  This could no doubt result in potential
 * issues.
 */
function femail_email_remove($account, $email){
  // If we're here, we just remove
  db_delete('femail_user_emails')->condition('email', $email)->condition('uid', $account->uid)->execute();
  drupal_set_message("Email removed");
  drupal_goto('user/' . $account->uid . '/femail');
}

/**
 * Form for selecting which forums to subscribe to
 */
function femail_user_subscribe($form, $form_state){
  // It's OK to use "taxonomy_get_tree", as NO FORUM IS HUGE!
  $forum_tree = taxonomy_get_tree(variable_get('forum_nav_vocabulary', 0));
  $forum_containers = variable_get('forum_containers', array());
  $form['femail_subscribe'] = array(
    '#tree' => TRUE,
    '#prefix' => '<table><tr><th>' . t('Forums') . '</th></tr>',
    '#suffix' => '</table>'
  );
  $results = db_select('femail_user_subscriptions', 'f')->condition('uid', $form_state['build_info']['args'][0]->uid)->fields('f', array(
    'tid'
  ))->execute();
  $subscriptions = array();
  foreach($results as $row){
    $subscriptions[$row->tid] = $row->tid;
  }
  $class = "odd";
  foreach($forum_tree as $branch){
    if($class == "odd"){
      $class = "even";
    }else{
      $class = "odd";
    }
    if(in_array($branch->tid, $forum_containers)){
      // Container
      $form['femail_subscribe'][$branch->tid] = array(
        '#markup' => '<tr class="' . $class . '"><td style="padding-left:' . (($branch->depth * 10) + 5) . 'px;">' .check_plain($branch->name) . '</td></tr>'
      );
    }else{
      // Forum
      $form['femail_subscribe'][$branch->tid] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($branch->name),
        '#default_value' => isset($subscriptions[$branch->tid]) ? 1 : isset($subscriptions[0]) ? 1 : 0,
        // Note, if the "Subscribe to all" option has previously been saved,
        // all forums will appear checked.  This will hopefully remove any
        // confusion from the "subscribe to all" option.
        '#prefix' => '<tr class="' . $class . '"><td style="padding-left:' . (($branch->depth * 10) + 5) . 'px">',
        '#suffix' => '</td></tr>'
      );
    }
  }
  // Hidden field for the UID - just makes things simpler later on.
  $form['femail_uid'] = array(
    '#type' => 'hidden',
    '#value' => $form_state['build_info']['args'][0]->uid
  );
  // Option to subscribe to all.  If this field is checked, all others will be
  // unchecked.
  $form['femail_subscribe_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Subscribe to all'),
    '#default_value' => isset($subscriptions[0]) ? 1 : 0,
    '#description' => t('Note, this is not the same as checking all the boxes above.  This option will automatically subscibe you to any new forums')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe')
  );
  return $form;
}

/**
 * Submit function for a user's Femail subscriptions.
 */
function femail_user_subscribe_submit($form, $form_state){
  // Lets delete everything from the femail_user_subscriptions for this user
  db_delete('femail_user_subscriptions')->condition('uid', $form_state['values']['femail_uid'])->execute();
  if($form_state['values']['femail_subscribe_all']){
    db_insert('femail_user_subscriptions')->fields(array(
      'uid' => $form_state['values']['femail_uid'],
      'tid' => 0
    ))->execute();
  }else{
    drupal_set_message(print_r($form_state['values']['femail_subscribe'], 1));
    foreach($form_state['values']['femail_subscribe'] as $tid => $value){
      if($value){
        db_insert('femail_user_subscriptions')->fields(array(
          'uid' => $form_state['values']['femail_uid'],
          'tid' => $tid
        ))->execute();
      }
    }
  }
  drupal_set_message("Subscription updated");
}

/**
 * Provides the form for adding an additional email address for a user.
 */
function femail_user_add($form, $form_state){
  $account = $form_state['build_info']['args'][0];
  $form['femail_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email')
  );
  $form['femail_uid'] = array(
    '#type' => 'hidden',
    '#value' => $account->uid
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add an additional Email')
  );
  return $form;
}

/**
 * Submit function that adds an email address for a user.  This will send out
 * the email to the user so that they can validate their email.
 */
function femail_user_add_submit($form, $form_state){
  $account = user_load($form_state['values']['femail_uid']);
  drupal_mail('femail', 'verify', $form_state['values']['femail_email'], user_preferred_language($account));
  db_insert('femail_user_emails')->fields(array(
    'uid' => $form_state['values']['femail_uid'],
    'email' => $form_state['values']['femail_email'],
    'status' => 0
  ))->execute();
  drupal_set_message($form_state['values']['femail_email'] . " added. Please now check your email to validate this address.");
}

/**
 * Ensure that the email supplied is not already in use on the site.  Also
 * ensure that the email address is a valid email.
 */
function femail_user_add_validate($form, &$form_state){
  // Check that it is a valid email address
  if(!valid_email_address($form_state['values']['femail_email'])){
    form_set_error('femail_email', t('Email address provided is not valid'));
  }
  // Check for existing entries.
  $mails = db_select('femail_user_emails', 'f')->fields('f', array(
    'email'
  ))->condition('email', $form_state['values']['femail_email'])->union(db_select('users', 'u')->fields('u', array(
    'mail'
  ))->condition('mail', $form_state['values']['femail_email']))->execute()->fetchField();
  if($mails){
    form_set_error('femail_email', t('Email is already in use on this site.'));
  }
}