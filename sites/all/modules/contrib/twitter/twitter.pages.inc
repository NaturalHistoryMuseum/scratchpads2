<?php
/**
 * @file
 * Page callbacks for Twitter module.
 */

/**
 * Form builder; Twitter settings form.
 */
function twitter_admin_form($form, &$form_state) {
  $form['twitter_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import and display the Twitter statuses of site users who have entered their Twitter account information.'),
    '#default_value' => variable_get('twitter_import', 1),
  );
  $form['twitter_expire'] = array(
    '#type' => 'select',
    '#title' => t('Delete old statuses'),
    '#default_value' => variable_get('twitter_expire', 0),
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(604800, 2592000, 7776000, 31536000), 'format_interval'),
    '#states' => array(
      'visible' => array(
        ':input[name=twitter_import]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#access' => module_exists('oauth_common'),
    '#description' => t('To enable OAuth based access for twitter, you must <a href="@url">register your application</a> with Twitter and add the provided keys here.', array('@url' => 'https://dev.twitter.com/apps/new')),
  );
  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#markup' => url('twitter/oauth', array('absolute' => TRUE)),
  );
  $form['oauth']['twitter_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer key'),
    '#default_value' => variable_get('twitter_consumer_key', NULL),
  );
  $form['oauth']['twitter_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer secret'),
    '#default_value' => variable_get('twitter_consumer_secret', NULL),
  );

  // Twitter external APIs settings.
  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter Settings'),
    '#description' => t('The following settings connect Twitter module with external APIs. ' .
      'Change them if, for example, you want to use Identi.ca.'),
  );
  $form['twitter']['twitter_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter host'),
    '#default_value' => variable_get('twitter_host', TWITTER_HOST),
  );
  $form['twitter']['twitter_api'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter API'),
    '#default_value' => variable_get('twitter_api', TWITTER_API),
  );
  $form['twitter']['twitter_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter search'),
    '#default_value' => variable_get('twitter_search', TWITTER_SEARCH),
  );
  $form['twitter']['twitter_tinyurl'] = array(
    '#type' => 'textfield',
    '#title' => t('TinyURL'),
    '#default_value' => variable_get('twitter_tinyurl', TWITTER_TINYURL),
  );

  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function twitter_user_settings($account) {
  module_load_include('inc', 'twitter');

  $output = array();
  if (!empty($account->twitter_accounts)) {
    $output['list_form'] = drupal_get_form('twitter_account_list_form', $account->twitter_accounts);
  }
  $output['form'] = drupal_get_form('twitter_account_form', $account);

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function twitter_account_list_form($form, $form_state, $twitter_accounts = array()) {
  $form['#tree'] = TRUE;
  $form['accounts'] = array();

  foreach ($twitter_accounts as $twitter_account) {
    $form['accounts'][] = _twitter_account_list_row($twitter_account);
  }

  if (!empty($twitter_accounts)) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }

  return $form;
}

function _twitter_account_list_row($account) {
  $form['#account'] = $account;

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $account->id,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['screen_name'] = array(
    '#type' => 'value',
    '#value' => $account->screen_name,
  );

  $form['image'] = array(
    '#markup' => theme('image', array('path' => $account->profile_image_url)),
  );

  $form['visible_name'] = array(
    '#markup' => l($account->screen_name, 'http://www.twitter.com/' . $account->screen_name),
  );

  $form['description'] = array(
    '#markup' => filter_xss($account->description),
  );

  $form['protected'] = array(
    '#markup' => empty($account->protected) ? t('No') : t('Yes'),
  );

  // Here we use user_access('import own tweets') to check permission
  // instead of user_access('import own tweets', $account->uid)
  // because we allow roles with sufficient permission to overwrite
  // the user's import settings.
  if (variable_get('twitter_import', TRUE) && user_access('import own tweets')) {
    $form['import'] = array(
      '#type' => 'checkbox',
      '#default_value' => user_access('import own tweets') ? $account->import : '',
    );
  }

  $form['delete'] = array(
    '#type' => 'checkbox',
  );

  return $form;
}

/**
 * Theme the list of Twitter accounts.
 */
function theme_twitter_account_list_form($variables) {
  $form = $variables['form'];

  if (variable_get('twitter_import', TRUE) && user_access('import own tweets')) {
    $header = array('', t('Name'), t('Description'), t('Private'), t('Import'), t('Delete'));
  }
  else {
    $header = array('', t('Name'), t('Description'), t('Private'), t('Delete'));
  }

  if (user_access('make twitter accounts global')) {
    $header[] = '';
  }

  $rows = array();

  foreach (element_children($form['accounts']) as $key) {
    $element = &$form['accounts'][$key];
    if (variable_get('twitter_import', TRUE) && user_access('import own tweets')) {
      $row = array(
        drupal_render($element['image']),
        drupal_render($element['id']) . drupal_render($element['screen_name']) . drupal_render($element['visible_name']),
        drupal_render($element['description']),
        drupal_render($element['protected']),
        drupal_render($element['import']),
        drupal_render($element['delete']),
      );
    }
    else {
      $row = array(
        drupal_render($element['image']),
        drupal_render($element['id']) . drupal_render($element['screen_name']) . drupal_render($element['visible_name']),
        drupal_render($element['description']),
        drupal_render($element['protected']),
        drupal_render($element['delete']),
      );
    }

    if (user_access('make twitter accounts global')) {
      $label = ($element['#account']->is_global) ? t('remove global') : t('make global');
      $row[] = l($label, 'user/' . $element['#account']->uid . '/edit/twitter/global/' . $element['#account']->id);
    }

    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form submit handler for altering the list of Twitter accounts.
 */
function twitter_account_list_form_submit($form, &$form_state) {
  $accounts = $form_state['values']['accounts'];
  foreach ($accounts as $account) {
    if (empty($account['delete'])) {
      twitter_account_save($account);
      drupal_set_message(t('The Twitter account settings were updated.'));
    }
    else {
      twitter_account_delete($account['id']);
      drupal_set_message(t('The Twitter account was deleted.'));
    }
  }
}

/**
 * Form builder for setting a Twitter account as global.
 */
function twitter_user_make_global($form, $form_state, $account, $twitter_uid) {
  module_load_include('inc', 'twitter');

  $twitter_account = twitter_account_load($twitter_uid);

  $form = array();

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['twitter_uid'] = array(
    '#type' => 'value',
    '#value' => $twitter_uid,
  );

  if ($twitter_account->is_global) {
    $text = t('Are you sure you want to remove %screen_name from the global accounts?', array('%screen_name' => $twitter_account->screen_name));
    $description = t('This means other users will no longer be allowed to post using this account.');
  }
  else {
    $text = t('Are you sure you want to allow other users to access the %screen_name account?', array('%screen_name' => $twitter_account->screen_name));
    $description = t('This will allow other users to post using this account.');
  }

  return confirm_form($form, $text, 'user/' . $account->uid . '/edit/twitter', $description);
}

/**
 * Form submit handler for setting a Twitter account as global.
 */
function twitter_user_make_global_submit($form, &$form_state) {
  db_update('twitter_account')
    ->expression('is_global', '(1 - is_global)')
    ->condition('twitter_uid', $form_state['values']['twitter_uid'])
    ->execute();

  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/edit/twitter';
}

/**
 * Form to add a Twitter account
 */
function twitter_account_form($form, $form_state, $account = NULL) {
  if (empty($account)) {
    global $user;
    $account = $user;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add account'),
  );

  return $form;
}

/**
 * Form validation for adding a new Twitter account.
 */
function twitter_account_form_validate($form, &$form_state) {
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');
  if ($key == '' || $secret == '') {
    form_set_error('', t('Please configure your consumer key and secret key at <a href="!url">Twitter settings</a>.', array(
      '!url' => url('admin/config/services/twitter'),
    )));
  }
}

/**
 * Form submit handler for adding a Twiter account.
 *
 * Loads Twitter account details and adds them to the user account
 */
function twitter_account_form_submit($form, &$form_state) {
  module_load_include('inc', 'twitter');

  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');
  $twitter = new Twitter($key, $secret);
  $token = $twitter->get_request_token();
  if ($token) {
    $_SESSION['twitter_oauth']['account'] = user_load($form['uid']['#value']);
    $_SESSION['twitter_oauth']['token'] = $token;
    $_SESSION['twitter_oauth']['destination'] = $_GET['q'];
    drupal_goto($twitter->get_authorize_url($token));
  }
  else {
  }
}

/**
 * Wrapper to call drupal_form_submit() which wasn't required in D6.
 */
function twitter_oauth_callback() {
  if (isset($_GET['denied']) || empty($_GET['oauth_token'])) {
    drupal_set_message(t('The connection to Twitter failed. Please try again.'), 'error');
    global $user;
    if ($user->uid) {
      // User is logged in, was attempting to OAuth a Twitter account.
      drupal_goto('user/' . $user->uid . '/edit/twitter');
    }
    else {
      // Anonymous user, redirect to front page.
      drupal_goto('<front>');
    }
  }
  $form_state['values']['oauth_token'] = $_GET['oauth_token'];
  drupal_form_submit('twitter_oauth_callback_form', $form_state);
}

/**
 * Form builder function. In D6 this form was built in response to the
 * oauth return request from Twitter, and the setting of
 * $form['#post'] seems to have caused the form to be validated and
 * processed.
 */
function twitter_oauth_callback_form($form, &$form_state) {
  $form['#post']['oauth_token'] = $_GET['oauth_token'];
  $form['oauth_token'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['oauth_token'],
  );
  return $form;
}

/**
 * Validate results from Twitter OAuth return request.
 */
function twitter_oauth_callback_form_validate($form, &$form_state) {
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');

  if ($key == '' || $secret == '') {
    form_set_error('', t('Please configure your Twitter consumer key and secret.'));
  }

  if (isset($_SESSION['twitter_oauth'])) {
    $form_state['twitter_oauth'] = $_SESSION['twitter_oauth'];
    unset($_SESSION['twitter_oauth']);
  }
  else {
    form_set_error('oauth_token', 'Invalid Twitter OAuth request');
  }

  if (isset($form_state['twitter_oauth']['token'])) {
    $token = $form_state['twitter_oauth']['token'];
    if (!is_array($token) || !$key || !$secret) {
      form_set_error('oauth_token', t('Invalid Twitter OAuth request'));
    }
    if ($token['oauth_token'] != $form_state['values']['oauth_token']) {
      form_set_error('oauth_token', t('Invalid OAuth token.'));
    }
  }
  else {
    form_set_error('oauth_token', t('Invalid Twitter OAuth request'));
  }

  module_load_include('inc', 'twitter');

  if ($twitter = new Twitter($key, $secret, $token['oauth_token'], $token['oauth_token_secret'])) {
    if ($response = $twitter->get_access_token()) {
      $form_state['twitter_oauth']['response'] = $response;
    }
    else {
      form_set_error('oauth_token', t('Invalid Twitter OAuth request'));
    }
  }
  else {
    form_set_error('oauth_token', t('Invalid Twitter OAuth request'));
  }
}

/**
 * Handle a Twitter OAuth return request and store the account creds
 * in the DB. Redirects to user/%/edit/twitter
 */
function twitter_oauth_callback_form_submit($form, &$form_state) {
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');
  $response = $form_state['twitter_oauth']['response'];

  $twitter = new Twitter($key, $secret, $response['oauth_token'], $response['oauth_token_secret']);
  try {
    $twitter_account = $twitter->users_show($response['screen_name']);
  } catch (TwitterException $e) {
    form_set_error('screen_name', t('Request failed: @message.', array('@message' => $e->getMessage())));
    return;
  }
  $twitter_account->set_auth($response);
  $account = $form_state['twitter_oauth']['account'];
  twitter_account_save($twitter_account, TRUE, $account);

  $form_state['programmed'] = FALSE;
  $form_state['redirect'] = $form_state['twitter_oauth']['destination'];
}
