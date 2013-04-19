<?php

/**
 * @file
 * (Un)subscription and (un)subscription confirmation
 *
 * FAPI subscription form cases:
 * - ACCOUNT
 *   self/admin action: authenticated user
 *   via hook_user form: category=newsletter
 *
 * - BLOCK
 *   self action: anonymous / authenticated user
 *   via hook_block: block
 *
 * - PAGE
 *   self action: anonymous / authenticated user
 *   callback: newsletter/subscriptions
 *
 * - MULTI BLOCK
 *   self action: anonymous / authenticated user
 *   authenticated user
 *   via hook_block: multi_block
 *   using PAGE handlers
 *
 * - ADMIN
 *   admin action: authenticated user
 *   via hook_menu: admin
 *
 * FAPI additional form cases:
 * - CONFIRM ADD
 * - CONFIRM REMOVAL
 *
 * @ingroup simplenews
 */

/**
 * FAPI ACCOUNT subscription form.
 *
 * Finally _account_ cases inject into hook_user and won't work on its own.
 * Note that our basis is:
 * drupal_get_form('user_profile_form', ...);
 * and NOT:
 * drupal_get_form('simplenews_subscriptions_account', ...);
 *
 * see also user/user.module and user/user.pages.inc
 *
 * @see simplenews_subscriptions_account_form_validate()
 * @see simplenews_subscriptions_account_form_submit()
 */
function simplenews_subscriptions_account_form(&$form, &$form_state, $subscriber) {

  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_category_get_visible() as $newsletter) {
    $options[$newsletter->tid] = check_plain(_simplenews_newsletter_name($newsletter));
    $default_value[$newsletter->tid] = FALSE;
  }

  if ($subscriber) {
    $default_value = array_merge($default_value, $subscriber->tids);
  }

  $form['subscriptions'] = array(
    '#type' => 'fieldset',
    '#description' => t('Select your newsletter subscriptions.'),
  );
  $form['subscriptions']['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $default_value,
  );

  $form['subscriptions']['#title'] = t('Current newsletter subscriptions');

  // if we don't override #validate, see user_profile_form_validate
  // adding an own #submit leads to the situation where drupal omits execution of user_profile_form_submit completely
  $form['#submit'][] = 'simplenews_subscriptions_account_form_submit';
}

/**
 * FAPI ACCOUNT subscription form_submit.
 */
function simplenews_subscriptions_account_form_submit($form, &$form_state) {
  global $user;

  $account = $form['#user'];

  // We first subscribe, then unsubscribe. This prevents deletion of subscriptions
  // when unsubscribed from the
  arsort($form_state['values']['newsletters'], SORT_NUMERIC);
  foreach ($form_state['values']['newsletters'] as $tid => $checked) {
    if ($checked) {
      simplenews_subscribe_user($account->mail, $tid, FALSE, 'website');
    }
    else {
      simplenews_unsubscribe_user($account->mail, $tid, FALSE, 'website');
    }
  }
  if ($user->uid == $account->uid) {
    drupal_set_message(t('Your newsletter subscriptions have been updated.'));
  }
  else {
    drupal_set_message(t('The newsletter subscriptions for user %account have been updated.', array('%account' => $account->name)));
  }
}

/**
 * FAPI BLOCK subscription form.
 *
 * @param $tid term id of selected newsletter.
 *
 * @see simplenews_block_form_validate()
 * @see simplenews_block_form_submit()
 */
function simplenews_block_form($form, &$form_state, $tid) {
  global $user;
  $form = array();

  $submit_text = t('Subscribe');
  if ($user->uid) {
    if (simplenews_user_is_subscribed($user->mail, $tid)) {
      $submit_text = t('Unsubscribe');
      $form['action'] = array('#type' => 'value', '#value' => 'unsubscribe');
      $form['#attributes'] = array('class' => array('simplenews-unsubscribe'));
    }
    else {
      $form['action'] = array('#type' => 'value', '#value' => 'subscribe');
      $form['#attributes'] = array('class' => array('simplenews-subscribe'));
    }
    $form['mail'] = array('#type' => 'value', '#value' => $user->mail);
  }
  else {
    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 20,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    $form['action'] = array('#type' => 'value', '#value' => 'subscribe');
    $form['#attributes'] = array('class' => array('simplenews-subscribe'));
  }

  // All block forms use the same validate and submit function.
  // #tid carries the tid for processing of the right newsletter issue term.
  $form['#tid'] = $tid;
  $form['#validate'][] = 'simplenews_block_form_validate';
  $form['#submit'][] = 'simplenews_block_form_submit';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );
  return $form;
}

/*
 * FAPI BLOCK subscription form_validate.
 */

function simplenews_block_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['mail'])) {
    form_set_error('mail', t("The e-mail address you supplied is not valid."));
  }
}

/*
 * FAPI BLOCK subscription form_submit.
 */

function simplenews_block_form_submit($form, &$form_state) {
  $tid = $form['#tid'];
  $account = simplenews_load_user_by_mail($form_state['values']['mail']);

  $confirm = simplenews_require_double_opt_in($tid, $account);

  switch ($form_state['values']['action']) {
    case 'subscribe':
      simplenews_subscribe_user($form_state['values']['mail'], $tid, $confirm, 'website');
      if ($confirm) {
        drupal_set_message(t('You will receive a confirmation e-mail shortly containing further instructions on how to complete your subscription.'));
      }
      else {
        drupal_set_message(t('You have been subscribed.'));
      }
      break;
    case 'unsubscribe':
      simplenews_unsubscribe_user($form_state['values']['mail'], $tid, $confirm, 'website');
      if ($confirm) {
        drupal_set_message(t('You will receive a confirmation e-mail shortly containing further instructions on how to cancel your subscription.'));
      }
      else {
        drupal_set_message(t('You have been unsubscribed.'));
      }
      break;
  }
}

/**
 * FAPI PAGE subscription form.
 *
 * @see simplenews_subscriptions_page_form_validate()
 * @see simplenews_subscriptions_page_form_submit()
 */
function simplenews_subscriptions_page_form($form, &$form_state, $code = NULL) {
  global $user;

  $subscriber = $mail = FALSE;
  if (!empty($user->mail)) {
    $subscriber = simplenews_subscriber_load_by_mail($user->mail);
    $mail = $user->mail;
  }
  // If a hash is provided, try to load the corresponding subscriber.
  elseif ($code) {
    if (!$subscriber = simplenews_subscriber_load_by_hash($code)) {
      drupal_not_found();
      return;
    }
    $mail = $subscriber->mail;
  }

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_category_get_visible() as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  if ($subscriber) {
    // If there is an existing subscriber object, use the existing settings.
    $default_value = array_merge($default_value, $subscriber->tids);
  }

  $form['subscriptions'] = array(
    '#type' => 'fieldset',
  );
  $form['subscriptions']['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $default_value
  );

  // If we have a mail address, which is either from a logged in user or a
  // subscriber identified through the hash code, display the mail address
  // instead of a textfield. Anonymous uses will still have to confirm any
  // changes.
  if ($mail) {
    $form['subscriptions']['#title'] = t('Subscriptions for %mail', array('%mail' => $mail));
    $form['subscriptions']['#description'] = t('Check the newsletters you want to subscribe to. Uncheck the ones you want to unsubscribe from.');
    $form['subscriptions']['mail'] = array('#type' => 'value', '#value' => $mail);
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
  }
  else {
    $form['subscriptions']['#title'] = t('Manage your newsletter subscriptions');
    $form['subscriptions']['#description'] = t('Select the newsletter(s) to which you want to subscribe or unsubscribe.');
    $form['subscriptions']['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 20,
      '#maxlength' => 128,
      '#weight' => 10,
      '#required' => TRUE,
    );
    $form['subscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Subscribe'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
    $form['unsubscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Unsubscribe'),
      '#weight' => 30,
      // @todo: add clean submit handler
    );
  }

  $form['#validate'][] = 'simplenews_subscriptions_page_form_validate';
  $form['#submit'][] = 'simplenews_subscriptions_page_form_submit';

  return $form;
}

/**
 * FAPI PAGE subscription form_validate.
 */
function simplenews_subscriptions_page_form_validate($form, &$form_state) {
  $valid_email = valid_email_address($form_state['values']['mail']);
  if (!$valid_email) {
    form_set_error('mail', t('The e-mail address you supplied is not valid.'));
  }

  $checked_newsletters = array_filter($form_state['values']['newsletters']);
  // Unless we're in update mode, at least one checkbox must be checked.
  if (!count($checked_newsletters) && $form_state['values']['op'] != t('Update')) {
    form_set_error('newsletters', t('You must select at least one newsletter.'));
  }
}

/**
 * FAPI PAGE subscription form_submit.
 */
function simplenews_subscriptions_page_form_submit($form, &$form_state) {
  $mail = $form_state['values']['mail'];
  $account = simplenews_load_user_by_mail($mail);

  // Group confirmation mails as necessary and configured.
  simplenews_confirmation_combine(TRUE);

  switch ($form_state['values']['op']) {
    case t('Update'):
      // We first subscribe, then unsubscribe. This prevents deletion of subscriptions
      // when unsubscribed from the
      arsort($form_state['values']['newsletters'], SORT_NUMERIC);
      foreach ($form_state['values']['newsletters'] as $tid => $checked) {
        $confirm = simplenews_require_double_opt_in($tid, $account);
        if ($checked) {
          simplenews_subscribe_user($mail, $tid, $confirm, 'website');
        }
        else {
          simplenews_unsubscribe_user($mail, $tid, $confirm, 'website');
        }
      }
      if (simplenews_confirmation_send_combined()) {
        drupal_set_message(t('You will receive a confirmation e-mail shortly containing further instructions on how to complete your subscription.'));
      }
      else {
        drupal_set_message(t('The newsletter subscriptions for %mail have been updated.', array('%mail' => $form_state['values']['mail'])));
      }
      break;
    case t('Subscribe'):
      foreach ($form_state['values']['newsletters'] as $tid => $checked) {
        if ($checked) {
          $confirm = simplenews_require_double_opt_in($tid, $account);
          simplenews_subscribe_user($mail, $tid, $confirm, 'website');
        }
      }
      if (simplenews_confirmation_send_combined()) {
        drupal_set_message(t('You will receive a confirmation e-mail shortly containing further instructions on how to complete your subscription.'));
      }
      else {
        drupal_set_message(t('The newsletter subscriptions for %mail have been updated.', array('%mail' => $form_state['values']['mail'])));
      }
      break;
    case t('Unsubscribe'):
      foreach ($form_state['values']['newsletters'] as $tid => $checked) {
        if ($checked) {
          $confirm = simplenews_require_double_opt_in($tid, $account);
          simplenews_unsubscribe_user($mail, $tid, $confirm, 'website');
        }
      }
      if (simplenews_confirmation_send_combined()) {
        drupal_set_message(t('You will receive a confirmation e-mail shortly containing further instructions on how to cancel your subscription.'));
      }
      else {
        drupal_set_message(t('The newsletter subscriptions for %mail have been updated.', array('%mail' => $form_state['values']['mail'])));
      }
      break;
  }
}

/**
 * FAPI MULTI BLOCK subscription form.
 *
 * Menu callback: Generates the subscription form for users for the multisignup block.
 *
 * @see simplenews_subscriptions_multi_block_form_validate()
 * @see simplenews_subscriptions_multi_block_form_submit()
 */
function simplenews_subscriptions_multi_block_form($form, &$form_state) {
  global $user;

  $subscriber = !empty($user->mail) ? simplenews_subscriber_load_by_mail($user->mail) : FALSE;
  // If someone not authorized to edit their subscription, return empty form.
  if (!user_access('subscribe to newsletters')) {
    return;
  }

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_category_get_visible() as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  if ($subscriber) {
    // If there is an existing subscriber object, use the existing settings.
    $default_value = array_merge($default_value, $subscriber->tids);
  }

  $form['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $default_value,
  );

  // If current user is logged in, just display email.
  // Anonymous users see an email box and will receive confirmations
  if (user_is_logged_in()) {
    // @todo why not simply Manage your subscriptions?
    $form['mail'] = array('#type' => 'value', '#value' => $user->mail);
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
  }
  else {
    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 20,
      '#maxlength' => 128,
      '#weight' => 10,
      '#required' => TRUE,
    );
    $form['subscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Subscribe'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
    $form['unsubscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Unsubscribe'),
      '#weight' => 30,
      // @todo: add clean submit handler
    );
  }

  $form['#validate'][] = 'simplenews_subscriptions_page_form_validate';
  $form['#submit'][] = 'simplenews_subscriptions_page_form_submit';

  return $form;
}

/**
 * Menu callback: confirm the user's (un)subscription request
 *
 * This function is called by clicking the confirm link in the confirmation
 * email or the unsubscribe link in the footer of the newsletter. It handles
 * both subscription addition and subscription removal.
 *
 * Calling URLs are:
 * newsletter/confirm/add
 * newsletter/confirm/add/$HASH
 * newsletter/confirm/remove
 * newsletter/confirm/remove/$HASH
 *
 * @see simplenews_confirm_add_form()
 * @see simplenews_confirm_removal_form()
 */

/**
 * Menu callback: confirm the user's (un)subscription request
 *
 * This function is called by clicking the confirm link in the confirmation
 * email or the unsubscribe link in the footer of the newsletter. It handles
 * both subscription addition and subscription removal.
 *
 * @see simplenews_confirm_add_form()
 * @see simplenews_confirm_removal_form()
 *
 * @todo Add parameter description here.
 */
function simplenews_confirm_subscription() {
  $arguments = func_get_args();
  $op = array_shift($arguments);
  $code = array_shift($arguments);

  // Prevent search engines from indexing this page.
  $noindex = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex',
    ),
  );
  drupal_add_html_head($noindex, 'simplenews-noindex');

  if ($subscriber = simplenews_subscriber_load_by_hash($code)) {
    // Extract the category id.
    list($snid, $tid) = explode('t', drupal_substr($code, 10));
    if ($tid > 0) {
      $category = simplenews_category_load($tid);
    }
    // The confirmation page called with two arguments will display a confirmation question.
    // When called with three of more arguments the user will be directed to the
    // (un)subscribe confirmation page. The additional arguments will be passed on
    // to the confirmation page.
    if (empty($arguments)) {
      if ($op == 'remove') {
        return drupal_get_form('simplenews_confirm_removal_form', $subscriber->mail, $category);
      }
      elseif ($op == 'add') {
        return drupal_get_form('simplenews_confirm_add_form', $subscriber->mail, $category);
      }
      elseif ($op == 'combined') {
        // Redirect and display message if no changes are available.
        if (empty($subscriber->changes)) {
          drupal_set_message(t('All changes to your subscriptions where already applied. No changes made.'));
          drupal_goto(variable_get('site_frontpage', 'node'));
        }
        else {
          return drupal_get_form('simplenews_confirm_multi_form', $subscriber);
        }
      }
    }
    else {
      if ($op == 'remove') {
        simplenews_unsubscribe_user($subscriber->mail, $tid, FALSE, 'website');
        if ($path = variable_get('simplenews_confirm_unsubscribe_page', '')) {
          $path = $path . '/' . implode('/', $arguments);
          drupal_goto($path);
        }
        drupal_set_message(t('%user was unsubscribed from the %newsletter mailing list.', array('%user' => $subscriber->mail, '%newsletter' => _simplenews_newsletter_name($category))));
        drupal_goto(variable_get('site_frontpage', 'node'));
      }
      elseif ($op == 'add') {
        simplenews_subscribe_user($subscriber->mail, $tid, FALSE, 'website');
        if ($path = variable_get('simplenews_confirm_subscribe_page', '')) {
          $path = $path . '/' . implode('/', $arguments);
          drupal_goto($path);
        }
        drupal_set_message(t('%user was added to the %newsletter mailing list.', array('%user' => $subscriber->mail, '%newsletter' => _simplenews_newsletter_name($category))));
        drupal_goto(variable_get('site_frontpage', 'node'));
      }
      elseif ($op == 'combined') {
        // Redirect and display message if no changes are available.
        if (empty($subscriber->changes)) {
          drupal_set_message(t('All changes to your subscriptions where already applied. No changes made.'));
          drupal_goto(variable_get('site_frontpage', 'node'));
        }
        else {
          foreach ($subscriber->changes as $tid => $action) {
            if ($action == 'subscribe') {
              simplenews_subscribe_user($subscriber->mail, $tid, FALSE, 'website');
            }
            elseif ($action == 'unsubscribe') {
              simplenews_unsubscribe_user($subscriber->mail, $tid, FALSE, 'website');
            }
          }

          // Clear changes.
          $subscriber->changes = array();
          simplenews_subscriber_save($subscriber);

          drupal_set_message(t('Subscription changes confirmed for %user.', array('%user' => $subscriber->mail)));
          drupal_goto(variable_get('site_frontpage', 'node'));
        }
      }
    }
  }

  // If md5 didn't match, do a not found.
  drupal_not_found();
  return;
}

/**
 * Generate the confirm subscription form.
 *
 * @see simplenews_confirm_add_form_submit()
 */
function simplenews_confirm_add_form($form, &$form_state, $mail, $newsletter) {
  $form = array();
  $form['question'] = array(
    '#markup' => '<p>' . t('Are you sure you want to add %user to the %newsletter mailing list?', array('%user' => simplenews_mask_mail($mail), '%newsletter' => _simplenews_newsletter_name($newsletter))) . "<p>\n",
  );
  $form['mail'] = array(
    '#type' => 'value',
    '#value' => $mail,
  );
  $form['newsletter'] = array(
    '#type' => 'value',
    '#value' => $newsletter,
  );

  return confirm_form($form, t('Confirm subscription'), '', t('You can always unsubscribe later.'), t('Subscribe'), t('Cancel')
  );
}

function simplenews_confirm_add_form_submit($form, &$form_state) {
  simplenews_subscribe_user($form_state['values']['mail'], $form_state['values']['newsletter']->tid, FALSE, 'website');

  if (!$path = variable_get('simplenews_confirm_subscribe_page', '')) {
    $path = variable_get('site_frontpage', 'node');
    drupal_set_message(t('%user was added to the %newsletter mailing list.', array('%user' => $form_state['values']['mail'], '%newsletter' => _simplenews_newsletter_name($form_state['values']['newsletter']))));
  }

  $form_state['redirect'] = $path;
}

/**
 * Generate the confirm subscription form.
 *
 * @see simplenews_confirm_add_form_submit()
 */
function simplenews_confirm_multi_form($form, &$form_state, $subscriber) {
  $form = array();
  $form['question'] = array(
    '#markup' => '<p>' . t('Are you sure you want to confirm the following subscription changes for %user?', array('%user' => simplenews_mask_mail($subscriber->mail))) . "<p>\n",
  );

  $form['changes'] = array(
    '#theme' => 'item_list',
    '#items' => simplenews_confirmation_get_changes_list($subscriber),
  );

  $form['subscriber'] = array(
    '#type' => 'value',
    '#value' => $subscriber,
  );

  return confirm_form($form, t('Confirm subscription'), '', t('You can always change your subscriptions later.'), t('Confirm'), t('Cancel')
  );
}

function simplenews_confirm_multi_form_submit($form, &$form_state) {
  $subscriber = $form_state['values']['subscriber'];
  foreach ($subscriber->changes as $tid => $action) {
    if ($action == 'subscribe') {
      simplenews_subscribe_user($subscriber->mail, $tid, FALSE, 'website');
    }
    elseif ($action == 'unsubscribe') {
      simplenews_unsubscribe_user($subscriber->mail, $tid, FALSE, 'website');
    }
  }

  // Clear changes.
  $subscriber->changes = array();
  simplenews_subscriber_save($subscriber);

  drupal_set_message(t('Subscription changes confirmed for %user.', array('%user' => $subscriber->mail)));
  $form_state['redirect'] = variable_get('site_frontpage', 'node');
}

/**
 * Mask a mail address.
 *
 * For example, name@example.org will be masked as n*****@e*****.org.
 *
 * @param $mail
 *   A valid mail address to mask.
 *
 * @return
 *   The masked mail address.
 */
function simplenews_mask_mail($mail) {
  if (preg_match('/^(.).*@(.).*(\..+)$/', $mail)) {
    return preg_replace('/^(.).*@(.).*(\..+)$/', '$1*****@$2*****$3', $mail);
  }
  else {
    // Missing top-level domain.
    return preg_replace('/^(.).*@(.).*$/', '$1*****@$2*****', $mail);
  }
}

/**
 * Generate the confirm unsubscription form.
 *
 * @see simplenews_confirm_removal_form_submit()
 */
function simplenews_confirm_removal_form($form, &$form_state, $mail, $newsletter) {
  $form = array();
  $form['question'] = array(
    '#markup' => '<p>' . t('Are you sure you want to remove %user from the %newsletter mailing list?', array('%user' => simplenews_mask_mail($mail), '%newsletter' => _simplenews_newsletter_name($newsletter))) . "<p>\n",
  );
  $form['mail'] = array(
    '#type' => 'value',
    '#value' => $mail,
  );
  $form['newsletter'] = array(
    '#type' => 'value',
    '#value' => $newsletter,
  );

  return confirm_form($form, t('Confirm remove subscription'), '', t('This action will unsubscribe you from the newsletter mailing list.'), t('Unsubscribe'), t('Cancel')
  );
}

function simplenews_confirm_removal_form_submit($form, &$form_state) {
  simplenews_unsubscribe_user($form_state['values']['mail'], $form_state['values']['newsletter']->tid, FALSE, 'website');

  if (!$path = variable_get('simplenews_confirm_unsubscribe_page', '')) {
    $path = variable_get('site_frontpage', 'node');
    drupal_set_message(t('%user was unsubscribed from the %newsletter mailing list.', array('%user' => $form_state['values']['mail'], '%newsletter' => _simplenews_newsletter_name($form_state['values']['newsletter']))));
  }

  $form_state['redirect'] = $path;
}

/**
 * FAPI ADMIN subscription form.
 *
 * Menu callback: handle the edit subscription page and a subscription
 * page for anonymous users.
 *
 * @see simplenews_subscriptions_admin_form_validate()
 * @see simplenews_subscriptions_admin_form_submit()
 */
function simplenews_subscriptions_admin_form($form, &$form_state, $snid) {
  $subscriber = simplenews_subscriber_load($snid);

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_category_get_visible() as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  $form['subscriptions'] = array(
    '#title' => t('Subscriptions for %mail', array('%mail' => $subscriber->mail)),
    '#type' => 'fieldset',
    '#description' => t('Select the newsletter(s) to add/remove from subscription.'),
  );
  $form['subscriptions']['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_merge($default_value, $subscriber->tids),
  );

  $form['activated'] = array(
    '#title' => t('Activation'),
    '#type' => 'fieldset',
    '#description' => t('Activate or inactivate account.'),
  );
  $form['activated']['activated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activated'),
    '#default_value' => $subscriber->activated,
  );

  if ((variable_get('language_count', 1) > 1)) {
    $languages = language_list('enabled');
    foreach ($languages[1] as $langcode => $item) {
      $name = t($item->name);
      $language_options[$langcode] = $name . ($item->native != $name ? ' (' . $item->native . ')' : '');
    }
    $form['language'] = array(
      '#type' => 'fieldset',
      '#title' => 'Preferred language',
      '#description' => t('The e-mails will be localized in language chosen. Real users have their preference in account settings.'),
      '#disabled' => FALSE,
    );
    if ($subscriber->uid) {
      // Fallback if user has not defined a language.
      $language_default = t('Site default language (@name)', array('@name' => $language_options[language_default()->language]));
      $form['language']['language'] = array(
        '#type' => 'item',
        '#title' => t('User language'),
        '#markup' => isset($language_options[$subscriber->language]) ? $language_options[$subscriber->language] : $language_default,
      );
    }
    else {
      $form['language']['language'] = array(
        '#type' => 'select',
        '#default_value' => $subscriber->language,
        '#options' => $language_options,
      );
    }
  }

  $form['subscriptions']['mail'] = array('#type' => 'value', '#value' => $subscriber->mail);
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 20,
  );

  $form['#validate'][] = 'simplenews_subscriptions_admin_form_validate';
  $form['#submit'][] = 'simplenews_subscriptions_admin_form_submit';
  $form['#redirect'] = 'admin/content/simplenews/users';

  return $form;
}

/**
 * FAPI ADMIN subscription form_validate.
 */
function simplenews_subscriptions_admin_form_validate($form, &$form_state) {
  $subscriber = simplenews_subscriber_load_by_mail($form_state['values']['mail']);

  $valid_email = valid_email_address($form_state['values']['mail']);
  if (!$valid_email) {
    form_set_error('mail', t('The e-mail address you supplied is not valid.'));
  }
  $checked_newsletters = array_filter($form_state['values']['newsletters']);
  if (!count($checked_newsletters) && !$subscriber) {
    form_set_error('newsletters', t('You must select at least one newsletter.'));
  }
  $languages = language_list('enabled');
  if (!empty($form_state['values']['language'])
    && !isset($languages[1][$form_state['values']['language']])) {
    form_set_error('language', t('Please choose a language from the list.'));
  }
}

/**
 * FAPI ADMIN subscription form_submit.
 */
function simplenews_subscriptions_admin_form_submit($form, &$form_state) {
  $subscriber = simplenews_subscriber_load_by_mail($form_state['values']['mail']);

  // update subscriptions
  arsort($form_state['values']['newsletters'], SORT_NUMERIC);
  foreach ($form_state['values']['newsletters'] as $tid => $checked) {
    if ($checked) {
      simplenews_subscribe_user($form_state['values']['mail'], $tid, FALSE, 'website');
    }
    else {
      simplenews_unsubscribe_user($form_state['values']['mail'], $tid, FALSE, 'website');
    }
  }

  // update subscriber
  $data = array();
  $subscriber->activated = $form_state['values']['activated'];
  if (!$subscriber->uid) {
    if (isset($form_state['values']['language'])) {
      $subscriber->language = $form_state['values']['language'];
    }
  }
  simplenews_subscriber_save($subscriber);

  drupal_set_message(t('The newsletter subscriptions for %mail have been updated.', array('%mail' => $form_state['values']['mail'])));
}

