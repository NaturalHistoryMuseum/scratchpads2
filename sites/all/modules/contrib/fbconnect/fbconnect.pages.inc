<?php

/**
 * @file
 * User pages callbacks for the fbconnect module.
 */

/**
 * Menu callback.
 * Called when user perform Facebook registration
 */
function fbconnect_prompt_page() {
  $conf = fbconnect_get_config();
  $params = array(
    '!site_name' => $conf['invite_name'],
  );
  $reg_msg = t('Log in to !site_name with Facebook account', $params);
  $link_msg = t('Click here to connect your existing !site_name account with Facebook', $params);

  $reg_attr = array();
  if ($conf['fast_reg_mode']) {
    $reg_attr = array('attributes' => array(
      'onclick' => 'Drupal.fbconnect.DoFastRegistration(this); return false;'
    ));
  }
  $items = array('items' => array(l($reg_msg, 'fbconnect/register/create', $reg_attr), l($link_msg, 'user', array('query' => array('destination' => 'fbconnect/link')))));
  /*
  $links = array(
    array(
      'title' => $reg_msg,
      'href' => 'fbconnect/register/create',
      $reg_attr,
    ),
    array(
      'title' => $link_msg,
      'href' => 'user',
      array('attributes' => array('query' => 'destination=fbconnect/link')),
    ),
  );
  */

  //return /*theme('links', array('links' => fbconnect_prompt_page_links))*/
  return theme('item_list', $items);
}

/*
function fbconnect_prompt_page_links() {
  $conf = fbconnect_get_config();
  $params = array(
    '!site_name' => $conf['invite_name'],
  );
  $reg_msg = t('Click here to create a new !site_name account with Facebook', $params);
  $link_msg = t('Click here to connect your existing !site_name account with Facebook', $params);

  $reg_attr = array();
  if ($conf['fast_reg_mode']) {
    $reg_attr = array('attributes' => array(
      'onclick' => 'Drupal.fbconnect.DoFastRegistration(this); return false;'
    ));
  }

  $links = array(
    array(
      'title' => $reg_msg,
      'href' => 'fbconnect/register/create',
      $reg_attr,
    ),
    array(
      'title' => $link_msg,
      'href' => 'user',
      array('attributes' => array('query' => 'destination=fbconnect/link')),
    ),
  );

  return $links;
}
*/


/**
 * Menu callback.
 * Logs user into site or redirects.
 */
function fbconnect_login_page($form, $form_state = array()) {
  $conf = fbconnect_get_config();
  $data = fbconnect_get_user_info(array('name', 'email'));
  // User not logged into Facebook we dont have any information for them, let them log in.
  if (empty($data)) {
      return facebook_login_page($form, $form_state);
  }
  // Check if user has already registered with their Facebook account.
  $user_id = _is_fbconnect_user($data['id']);
  if ($user_id) {
    // User already registered Facebook account to site, log them in
    $form_state['uid'] = $user_id;
    user_login_submit($form, $form_state);
    drupal_set_message(t('You have been successfully logged in.'));
    drupal_goto('user/' . $user_id);
  }
  elseif ( _email_already_exist($data['email'])) {
    // The user was not found in the fbconnect_users table, but the emial from Facebook might already have an account on coracle.
    // Redirect user to the login page with a message.
    drupal_set_message(t('This email address is already registered to an account. Please log in to this account with the username and password provided during registration'));
    $options = array('query' => array(
      'fbconnect' => 'true'
    ));
    drupal_goto('user/login', $options);
  }
  else {
    drupal_set_message(t('You do not have an account with this site. Create an account on this site with through Facebook <a href="!here">here</a>'), array('!here' => url('fbconnect/register/create')));
  }

}

/**
 * Menu callback.
 * Called when user perform Facebook registration
 */
function fbconnect_register_page($form, $form_state = array()) {
  $conf = fbconnect_get_config();
  $data = fbconnect_get_user_info(array('name', 'email'));
  // User not logged into Facebook we dont have any information for them, let them log in.
  if (empty($data)) {
    return facebook_login_page($form, $form_state);
  }
  // Check if user has already registered with thier Facebook account
  $uid = _is_fbconnect_user($data['id']);
  if ($uid) {
    // User already registered Facebook account to site, log them in
    $form_state['uid'] = $uid;
    user_login_submit($form, $form_state);
  }
  elseif (_email_already_exist($data['email'])) {
    // The user was not found in the fbconnect_users table, but the
    // email from Facebook might already have an account.
    // Redirect user to the login page with a message..
    drupal_set_message(t('This email address is already registered to an account. Please log in to this account with the username and password provided during registration'));
    $options = array('query' => array(
      'fbconnect' => 'true'
    ));
    drupal_goto('user/login', $options);
    return;
  }

  $form = drupal_retrieve_form('user_register_form', $form_state);
  $site = $conf['invite_name'];

  // #758918 : prevent users registration if admin has blocked free registration
  $user_reg_mode = variable_get('user_register', 1);
  if ($user_reg_mode == 0) {
    drupal_access_denied();
    return;
  }

  $form['account']['name']['#value'] = empty($form_state['input']['name']) ? $data['name'] : $form_state['input']['name'];
  $form['account']['mail']['#parents'] = array('name');
  $form['account']['mail']['#value'] = $data['email'];
  $form['account']['mail']['#parents'] = array('mail');

  $form['fbconnect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook Connect'),
  );

  $form['fbconnect']['fb_visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Let my Facebook friends see me on @sitename', array('@sitename' => $site)),
    '#default_value' => 1,
  );

  if (variable_get('user_pictures', 0) && $conf['user_pictures'] == 'allow') {
    $form['fbconnect']['fb_avatar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use my Facebook picture as user picture'),
      '#description' => t('Your picture will be loaded from Facebook.'),
      '#default_value' => 1,
    );
  }

  $form_state['redirect'] = isset($_GET['destination']) ? $_GET['destination'] : '';
  $form['#validate'][] = 'fbconnect_register_form_validate';
  $form['#submit'][] = 'fbconnect_register_form_submit';

  // Fast registration mode, we by pass std drupal reg form.
  if ($conf['fast_reg_mode']) {
    $newname = $conf['fast_reg_autoname']? _fbconnect_get_username($data['name']) : $data['name'];
    //drupal_set_message('new name : ' . $newname);
    $form_state['values']['name'] = $newname;
    $form_state['values']['mail'] = $data['email'];
    $form_state['values']['pass'] = user_password();
    $form_state['values']['status'] = 1;
    $form_state['values']['fb_visible'] = 1;
    $form_state['values']['fb_avatar'] = 1;

    //to make validation work have to unset some form fields and settings Taxonomy fields cause errors
    //TODO:: check this code sure it will need to check for required profile fields and fill them with data
    foreach ($form as $key => $val) {
      //unset all cusom fields
      if (strstr($key, 'field_')) {
        //unset this custom field
        unset($form[$key]);
      }
    }
    $form_state['complete form'] = $form;
    //taxonomy linked fields causes issues with this validate form function
    drupal_validate_form('user_register', $form, $form_state);

    $success = !form_get_errors();
    if (!$success) {
      $dest = array('query' => 'destination=fbconnect/link');
      drupal_set_message(t('Facebook connect registration failed for the reasons listed. You may register now, or if you already have an account you can <a href="@login">log in</a> now and link your account', array('@login' => url('user/login', $dest))), 'warning');
      // Append form validation errors below the above warning.
      $messages = drupal_get_messages('error');
      foreach ($messages['error'] as $message) {
        drupal_set_message($message, 'error');
      }

      drupal_goto('user/register', $dest['query']);
    }

    fbconnect_register_form_submit($form, $form_state);
  }

  return $form;
}

function facebook_login_page(&$form, $form_state = array()) {
    $facebook = facebook_client();
    $params['scope'] = 'email';
    $login_url = $facebook->getLoginUrl($params);
    $form['log_in_to_facebook'] = array(
      '#title' => t('You must login to Facebook AND give the site access to use this feature'),
      '#description' => t('Log in using your Facebook account'),
      '#type' => 'item',
      '#markup' => l(t('Login using your Facebook account'), $login_url),
    );
    return $form;
}

/**
 * Returns a name suggestion if the wanted username is already used.
 */
function _fbconnect_get_username($name_suggestion) {
  $query = "SELECT COUNT(name) FROM {users} WHERE name = :user_name";
  $result = db_query($query, array(':user_name' => $name_suggestion))->fetchField();
  if ($result) {
    $counter = variable_get('fbconnect_namecounter', 1);
    $newname = 'fbuser' . $counter;
    $counter++;
    variable_set('fbconnect_namecounter', $counter);
  }
  else {
    $newname = $name_suggestion;
  }
  return $newname;
}

function fbconnect_register_form_validate($form, &$form_state) {
  $fbuid = fbconnect_get_fbuid();
  // Make sure we have a Facebook user object by the time we get here otherwise
  // drop form and exit with error message.
  if ($fbuid == 0) {
    drupal_set_message(t('You are not logged into Facebook. Log into Facebook and try again.'), 'error');
    drupal_goto();
  }
}

function fbconnect_register_form_submit($form, &$form_state) {
  $fbuid = fbconnect_get_fbuid();
  if (_is_fbconnect_user($fbuid)) {
    drupal_set_message(t('Retrying registration. Your Facebook account is already assigned to one account on our site.'), 'error');
    watchdog('fbconnect', 'Error creating account for @fbuid', array('@fbuid' => $fbuid), WATCHDOG_ALERT);
    drupal_goto();
  }

  if (variable_get('fbconnect_fast_reg', NULL)) {
    $user = user_save(NULL, $form_state['values']);
  }
  else {
    $user = $form_state['user'];
  }
  if (!$user) {
    drupal_set_message(t('Error saving user account.'), 'error');
    drupal_goto();
  }

  fbconnect_register($user->uid, $fbuid);
  $data['data']['fb_avatar'] = isset($form_state['values']['fb_avatar']) ? $form_state['values']['fb_avatar'] : 0;
  $data['data']['fb_visible'] = $form_state['values']['fb_visible'];
  $user = user_save($user, $data);

  $form_state['uid'] = $user->uid;
  user_login_submit(array(), $form_state);
  $_SESSION['fbconnect_feed']['type'] = 'register';

  $conf = fbconnect_get_config();
  if ($conf['fast_reg_mode']) {
    drupal_goto('user');
  }
}

/**
 * Menu callback fbconnect identities
 */
function fbconnect_user_identities($form_id, $account) {
  $conf = fbconnect_get_config();
  $current_fbuid = fbconnect_get_fbuid(); // fbuid of currently logged in FB user
  $fbuid = _get_user_fbuid($account->uid); // fbuid of account being edited
  drupal_set_title(check_plain($account->name));
  $uid = NULL;
  if ($fbuid) {
    $uid = _is_fbconnect_user($fbuid);
    $account->fbuid = $fbuid;
  }
  if (!$uid && !empty($current_fbuid)) {
    // We have Facebook access for this user but we dont have the user in the fbconnect_users table, add him.
    fbconnect_register($account->uid, $current_fbuid);
    $uid = _is_fbconnect_user($current_fbuid);
  }
  if (isset($account->fbuid) && $account->fbuid) {
    return drupal_get_form('fbconnect_user_settings_form', $account);
  }
  elseif ($uid && $account->uid != $uid) {
    return theme('fbconnect_user_profile_tab_connected', array('account' => $account, 'conf' => $conf, 'uid' => $uid));
  }
  else {
    return theme('fbconnect_user_profile_tab_unconnected', array('account' => $account, 'conf' => $conf, 'uid' => $uid));
  }
}

/**
 *  This form allows the user to manage their Facebook connect settings.
 *
 *  Some options are visible only if Facebook session is active.
 */
function fbconnect_user_settings_form($form, $form_state, $account) {
  $conf = fbconnect_get_config();
  // These fields manage the visibility of the user,
  // if the option is enabled, user's Facebook friends
  // will be able to see his presence on this site
  $form['fb_visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Let my Facebook friends see me on @site', array('@site' => $conf['invite_name'])),
    '#description' => t('My Facebook friends will be able to see that I own an account on this website.'),
    '#default_value' => isset($account->data['fb_visible']) ? $account->data['fb_visible'] : NULL,
  );

  // Whether user picture is enabled,
  // users can choose to use his Facebook avatar as avatar
  if ($conf['user_pictures'] == 'allow') {
    $form['fb_avatar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use my Facebook picture as user picture'),
      '#description' => t('Your picture will be loaded from Facebook.'),
      '#default_value' => isset($account->data['fb_avatar']) ? $account->data['fb_avatar'] : NULL,
    );
  }

  if (!variable_get('user_pictures', 0)) {
    $form['fb_avatar']['#attributes'] = array(
      'disabled' => 'disabled',
    );
    $form['fb_avatar']['#description'] = t('Enable user picture support in !link', array(
      '!link' => l(t('User settings'), 'admin/config/people/accounts'),
    ));
  }

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['fbconnect'] = array(
    '#title' => t('Facebook account'),
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );

  $form['fbconnect']['profile'] = array(
    '#type' => 'item',
    '#title' => t('Linked to this Facebook account'),
    '#markup' => '<fb:name uid="' . $account->fbuid . '" useyou="false" linked="true"></fb:name>',
  );

  $form['fbconnect']['unlink'] = array(
    '#type' => 'submit',
    '#description' => t('Click here to unlink this Facebook account'),
    '#value' => t('Unlink'),
    '#access' => !!facebook_client(), // don't show unlink button if fbconnect is'nt properly configured
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Handle post-validation user_settingsForm submission.
 */
function fbconnect_user_settings_form_submit($form, &$form_state) {
  $account = $form_state['values']['account'];
  $data['data']['fb_visible'] = $form_state['values']['fb_visible'];
  $data['data']['fb_avatar'] = $form_state['values']['fb_avatar'];

  if ($form_state['values']['op'] == $form['fbconnect']['unlink']['#value']) {
    if (fbconnect_unregister($account->fbuid)) {
      unset($data);
    }
    else {
      form_set_error('fbconnect', t("Something went wrong. Can't unlink account"));
    }
  }

  if (isset($data)) {
    user_save($account, $data);
  }
  drupal_set_message(t('Your configuration options have been saved.'));
}

/**
 * Make rendering of Facebook user picture themeable
 */
function theme_fbconnect_user_picture_override($variables) {
  $fbuid = $variables['fbuid'];
  $account = $variables['account'];
  $user_url = $variables['user_url'];
  $size = $variables['size'];
  $protocol = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
  $path = $protocol . '://graph.facebook.com/' . $fbuid . '/picture?type=' . $size;
  $image = theme('image', array('path' => $path, 'alt' => $account->name));
  return '<div class="picture">' . $image . '</div>';
}


/**
 * Make rendering of Facebook user profile tab themable
 */
function theme_fbconnect_user_profile_tab_connected($variables) {
  $account = $variables['account'];
  $conf = $variables['conf'];
  $uid = $variables['uid'];
  $site_name = $conf['invite_name'] ? $conf['invite_name'] : '';
  return t("Your current Facebook session is associated to another account on our site.\nYou can logout of Facebook and attempt to associate another Facebook account to your !site_name account, or you need to disconnect the current association.", array('!site_name' => $site_name));
}

function theme_fbconnect_user_profile_tab_unconnected($variables) {
  $account = $variables['account'];
  $conf = $variables['conf'];
  $uid = $variables['uid'];
  return '<div>'
    . t('Click here to connect your Facebook account')
    . '</div><div class="fbconnect-button">'
    . fbconnect_render_button()
    . '</div>';
}

/**
 * Workaround to play nice with LoginDestination #786662.
 */
function fbconnect_link_callback() {
  global $user;
  drupal_get_messages('status');
  drupal_goto("user/{$user->uid}/edit/fbconnect");
}
