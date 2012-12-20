<?php

/**
 * @file
 * Provides functions used during Facebook login processes.
 */


/**
 * Menu callback; The main page for processing OAuth login transactions.
 *
 * @param $action
 *   The action being requested. Currently supports the following:
 *    - connect: Initiate Facebook connection and log a user in.
 *    - deauth: Remove the exisitng Facebook connection access.
 */
function fboauth_action_page($action) {
  global $user;

  // TODO: Support loading of more than one App ID and App Secret.
  $app_id = variable_get('fboauth_id', '');
  $app_secret = variable_get('fboauth_secret', '');

  $action_name = $action['name'];

  $error_message = t('The Facebook login could not be completed due to an error. Please create an account or contact us directly. Details about this error have already been recorded to the error log.');

  if (!($app_id && $app_secret)) {
    watchdog('fboauth', 'A Facebook login was attempted but could not be processed because the module is not yet configured. Vist the <a href="!url">Facebook OAuth configuration</a> to set up the module.', array('!url' => url('admin/config/people/fboauth')));
  }
  elseif (isset($_REQUEST['error'])) {
    $link = fboauth_action_link_properties($action_name, $app_id);
    watchdog('fboauth', 'A user refused to allow access to the necessary Facebook information (@permissions) to login to the site.', array('@permissions' => $link['query']['scope']));
    $error_message = t('This site requires access to information in order to log you into the site. If you like you can <a href="!facebook">sign in with Facebook again</a> or <a href="!register">register normally</a>.', array('!facebook' => url($link['href'], array('query' => $link['query'])), '!register' => url('user/register')));
  }
  elseif (!isset($_REQUEST['code'])) {
    watchdog('fboauth', 'A Facebook request code was expected but no authorization was received.');
  }
  // The primary action routine after access has been approved by the user.
  elseif ($access_token = fboauth_access_token($_REQUEST['code'], $action_name, $app_id, $app_secret)) {
    $destination = fboauth_action_invoke($action_name, $app_id, $access_token);
    if (empty($destination)) {
      $destination = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : '<front>';
    }
    drupal_goto($destination);
  }

  // In the event of an error, we stay on this page.
  return $error_message;
}

/**
 * Invoke an action specified through hook_fboauth_action_info().
 */
function fboauth_action_invoke($action_name, $app_id, $access_token) {
  $action = fboauth_action_load($action_name);

  // Call the specified action.
  if (isset($action['callback'])) {
    $callback = $action['callback'];

    if (function_exists($callback)) {
      return $callback($app_id, $access_token);
    }
  }
}

/**
 * Facebook OAuth callback for initiating a Facebook connection.
 */
function fboauth_action_connect($app_id, $access_token) {
  global $user;

  $fbuser = fboauth_graph_query('me', $access_token);
  $uid = fboauth_uid_load($fbuser->id);

  // If the user has logged in before, load their account and login.
  if (!$user->uid && $uid && ($account = user_load($uid))) {
    fboauth_login_user($account);
  }
  // If the Facebook e-mail address matches an existing account, bind them
  // together and log in as that account.
  elseif (!empty($fbuser->email) && ($account = user_load_by_mail($fbuser->email))) {
    fboauth_save($account->uid, $fbuser->id);

    // Logins will be denied if the user's account is blocked.
    if (fboauth_login_user($account)) {
      drupal_set_message(t('You\'ve connected your account with Facebook.'));
    }
  }
  else {
    // If the user is already logged in, associate the two accounts.
    if ($user->uid) {
      fboauth_save($user->uid, $fbuser->id);
      drupal_set_message(t('You\'ve connected your account with Facebook.'));
    }
    // Register a new user only if allowed.
    elseif (variable_get('user_register', 1)) {
      $account = fboauth_create_user($fbuser);
      // Load the account fresh just to have a fully-loaded object.
      $account = user_load($account->uid);

      // If the account requires administrator approval the new account will
      // have a status of '0' and not be activated yet.
      if ($account->status == 0) {
        _user_mail_notify('register_pending_approval', $account);
        drupal_set_message(t('An account has been created for you on @sitename but an administrator needs to approve your account. In the meantime, a welcome message with further instructions has been sent to your e-mail address.', array('@sitename' => variable_get('site_name', ''))));
      }
      // Log in the user if no approval is required.
      elseif (fboauth_login_user($account)) {
        drupal_set_message(t('Welcome to @sitename. Basic information has been imported from Facebook into your account. You may want to <a href="!edit">edit your account</a> to confirm the details and set a password.', array('@sitename' => variable_get('site_name', ''), '!edit' => url('user/' . $account->uid . '/edit'))));
      }
      // If the login fails, fboauth_login_user() throws its own error message.
    }
    // Since user's can't create new accounts on their own, show an error.
    else {
      drupal_set_message('Your Facebook e-mail address does not match any existing accounts. If you have an account, you must first log in before you can connect your account to Facebook. Creation of new accounts on this site is disabled.');
    }
  }
}

/**
 * Facebook OAuth callback for deauthorizing the site from Facebook.
 */
function fboauth_action_deauth($app_id, $access_token) {
  global $user;

  // Deauthorize our application from Facebook.
  $result = fboauth_method_invoke('auth.revokeAuthorization', $access_token);

  // If successful, also remove the uid-fbid pairing.
  if (!is_array($result) && $result) {
    fboauth_save($user->uid, NULL);
    drupal_set_message(t('Your account has been disconnected from Facebook.'));
  }
  else {
    drupal_set_message(t('There was an error disconnecting from Facebook. The server returned %message.', array('%message' => $result->error_msg)), 'error');
    watchdog('There was an error disconnecting the user %username from Facebook. The server returned %message.', array('%message' => $result->error_msg, '%username' => $user->name));
  }
}

/**
 * Given a Facebook user object, associate or save a Drupal user account.
 */
function fboauth_create_user($fbuser, $options = array()) {
  // Set default options.
  $defaults = array(
    'username' => variable_get('fboauth_user_username', 'username'),
    'picture' => (variable_get('user_pictures', 0) && variable_get('fboauth_user_picture', 'picture')) ? 'picture' : '',
    'status' => variable_get('user_register', 1) == 1 ? 1 : 0,
  );
  $options += $defaults;

  // Use their Facebook user name (if defined), otherwise their real name.
  // If an account already exists with that name, increment until the namespace
  // is available.
  if ($options['username'] === 'username' && !empty($fbuser->username)) {
    $username = $fbuser->username;
  }
  else {
    $username = $fbuser->name;
  }
  $query = "SELECT uid FROM {users} WHERE name = :name";
  $uid = db_query($query, array(':name' => $username))->fetchField();
  $i = 0;
  while ($uid) {
    $i++;
    $uid = db_query($query, array(':name' => ($username . $i)))->fetchField();
  }
  if ($i > 0) {
    $username = $username . $i;
  }

  // Initialize basic properties that are unlikely to need changing.
  $edit = array(
    'name' => $username,
    'mail' => !empty($fbuser->email) ? $fbuser->email : '',
    'init' => !empty($fbuser->email) ? $fbuser->email : '',
    // If user_register is "1", then no approval required.
    'status' => $options['status'],
    'timezone' => variable_get('date_default_timezone'), // TODO: is this appropriate in sites with no default?
    'fboauth' => TRUE, // Signify this is being imported by Facebook OAuth.
    'fboauth_fbid' => $fbuser->id, // So that other modules can load the account.
  );

  // Profile module support.
  if (module_exists('profile')) {
    module_load_include('inc', 'fboauth', 'includes/fboauth.profile');
    fboauth_profile_create_user($edit, $fbuser);
  }

  // Field module support.
  module_load_include('inc', 'fboauth', 'includes/fboauth.field');
  fboauth_field_create_user($edit, $fbuser);

  // Allow other modules to manipulate the user information before save.
  foreach (module_implements('fboauth_user_presave') as $module) {
    $function = $module . '_fboauth_user_presave';
    $function($edit, $fbuser);
  }

  $account = user_save(NULL, $edit);

  // Retrieve the user's picture from Facebook and save it locally.
  if ($account->uid && $options['picture'] === 'picture') {
    $picture_directory =  file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
    if(file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY)){
      $picture_result = drupal_http_request('https://graph.facebook.com/' . $fbuser->id . '/picture?type=large');
      $picture_path = file_stream_wrapper_uri_normalize($picture_directory . '/picture-' . $account->uid . '-' . REQUEST_TIME . '.jpg');
      $picture_file = file_save_data($picture_result->data, $picture_path, FILE_EXISTS_REPLACE);

      // Check to make sure the picture isn't too large for the site settings.
      $max_dimensions = variable_get('user_picture_dimensions', '85x85');
      file_validate_image_resolution($picture_file, $max_dimensions);

      // Update the user record.
      $picture_file->uid = $account->uid;
      $picture_file = file_save($picture_file);
      file_usage_add($picture_file, 'user', 'user', $account->uid);
      db_update('users')
        ->fields(array(
        'picture' => $picture_file->fid,
        ))
        ->condition('uid', $account->uid)
        ->execute();
    }
  }

  // Allow other modules to manipulate the user information after save.
  foreach (module_implements('fboauth_user_save') as $module) {
    $function = $module . '_fboauth_user_save';
    $function($account, $fbuser);
  }

  return $account;
}

/**
 * Given a Drupal user object, log the user in.
 *
 * This acts as a wrapper around user_external_login() in Drupal 6 and as a full
 * replacement function in Drupal 7, since no direct equivalent exists.
 *
 * @param $account
 *   A Drupal user account or UID.
 */
function fboauth_login_user($account) {
  global $user;

  if ($account->status) {
    $form_state['uid'] = $account->uid;
    user_login_submit(array(), $form_state);
  }
  else {
    drupal_set_message(t('The username %name has not been activated or is blocked.', array('%name' => $account->name)), 'error');
  }

  return !empty($user->uid);
}

/**
 * Given an approval code from Facebook, return an access token.
 *
 * The approval code is generated by Facebook when a user grants access to our
 * site application to use their data. We use this approval code to get an
 * access token from Facebook. The access token usually is valid for about
 * 15 minutes, allowing us to pull as much information as we want about the
 * user.
 *
 * @param $code
 *   An approval code from Facebook. Usually pulled from the ?code GET parameter
 *   after a user has approved our application's access to their information.
 * @param $action_name
 *   The action is the directory name underneath the "fboauth" path. This value
 *   must be the same between the page originally provided to Facebook as the
 *   "redirect" URL and when requesting an access token.
 * @return
 *   An access token that can be used in REST queries against Facebook's Graph
 *   API, which will provide us with info about the Facebook user.
 */
function fboauth_access_token($code, $action_name, $app_id = NULL, $app_secret = NULL) {
  // Use the default App ID and App Secret if not specified.
  $app_id = isset($app_id) ? $app_id : variable_get('fboauth_id', '');
  $app_secret = isset($app_secret) ? $app_secret : variable_get('fboauth_secret', '');

  // Note that the "code" provided by Facebook is a hash based on the client_id,
  // client_secret, and redirect_url. All of these things must be IDENTICAL to
  // the same values that were passed to Facebook in the approval request. See
  // the fboauth_link_properties function.
  $query = array(
    'client_id' => $app_id,
    'client_secret' => $app_secret,
    'redirect_uri' => fboauth_action_url('fboauth/' . $action_name, array('absolute' => TRUE, 'query' => !empty($_GET['destination']) ? array('destination' => $_GET['destination']) : array())),
    'code' => $code,
  );
  $token_url = url('https://graph.facebook.com/oauth/access_token', array('absolute' => TRUE, 'query' => $query));
  $authentication_result = drupal_http_request($token_url);

  if ($authentication_result->code != 200) {
    watchdog('fboauth', 'Facebook OAuth could not acquire an access token from Facebook. We queried the following URL: <code><pre>@url</pre></code>. Facebook\'s servers returned an error @error: <code><pre>@return</pre></code>', array('@url' => $token_url, '@error' => $authentication_result->code, '@return' => check_plain(print_r($authentication_result->data, TRUE))));
  }
  else {
    // The result from Facebook comes back in a query-string-like format,
    // key1=value1&key2=value2. Parse into an array.
    $authentication_strings = explode('&', $authentication_result->data);
    $authentication_values = array();
    foreach ($authentication_strings as $authentication_string) {
      list($authentication_key, $authentication_value) = explode('=', $authentication_string);
      $authentication_values[$authentication_key] = $authentication_value;
    }
  }

  return isset($authentication_values['access_token']) ? $authentication_values['access_token'] : NULL;
}

/**
 * Return a list of permissions based on a list of properties or connections.
 *
 * @param $access_requested
 *   Optional. A list of Facebook user properties or connections. If not
 *   specified, a list of all known permissions will be returned.
 * @return
 *   A list of Facebook permission names necessary to access those properties or
 *   connections.
 * @see http://developers.facebook.com/docs/reference/api/user/
 * @see http://developers.facebook.com/docs/authentication/permissions/
 */
function fboauth_user_permissions($access_requested = NULL) {
  $permissions = array();
  $permission_names = array(
    'user_about_me' => t('About yourself description'),
    'user_activities' => t('Your activities'),
    'user_birthday' => t('Your birthday'),
    'user_education_history' => t('Your education history'),
    'user_events' => t('Events you\'re attending'),
    'user_groups' => t('Your groups'),
    'user_hometown' => t('Your hometown'),
    'user_interests' => t('Your interests'),
    'user_likes' => t('Your likes'),
    'user_location' => t('Your location'),
    'user_notes' => t('Your notes'),
    'user_online_presence' => t('Your online/offline status'),
    'user_photo_video_tags' => t('Photos and videos you\'ve been tagged in'),
    'user_photos' => t('Photos you\'ve uploaded'),
    'user_relationships' => t('Access to your family and personal relationships and relationship status'),
    'user_relationship_details' => t('Your relationship preferences'),
    'user_religion_politics' => t('Your religious and political affiliations'),
    'user_status' => t('Your most recent status message'),
    'user_videos' => t('Videos you\'ve uploaded'),
    'user_website' => t('Your website URL'),
    'user_work_history' => t('Your work history'),
    'email' => t('Your e-mail'),
    'read_friendlists' => t('Access your lists of friends'),
    'read_insights' => t('Access your Facebook insights data'),
    'read_mailbox' => t('Access your Facebook inbox'),
    'read_requests' => t('Access your friend requests'),
    'read_stream' => t('Access your new feed'),
    'xmpp_login' => t('Log you into Facebook chat'),
    'ads_management' => t('Manage your ads'),
    'user_checkins' => t('Access check-in information'),
  );

  $properties = fboauth_user_properties();
  foreach ($properties as $property => $property_info) {
    if (isset($property_info['permission']) && (!isset($access_requested) || in_array($property, $access_requested))) {
      $permissions[$property_info['permission']] = isset($permission_names[$property_info['permission']]) ? $permission_names[$property_info['permission']] : $property_info['permission'];
    }
  }

  $connections = fboauth_user_connections();
  foreach ($connections as $connection => $connection_info) {
    if (isset($connection_info['permission']) && (!isset($access_requested) || in_array($connection, $access_requested))) {
      $permissions[$connection_info['permission']] = isset($permission_names[$connection_info['permission']]) ? $permission_names[$connection_info['permission']] : $connection_info['permission'];
    }
  }

  return $permissions;
}

/**
 * Return a list of Facebook user properties.
 *
 * This function provides a list of properties that may be attached directly to
 * a Facebook user account. This information is immediately available when a
 * user logs in with Facebook connect and may be stored locally.
 *
 * Each property requires extended permission granted by the end-user. The
 * returned array of properties provides the name of each required permission
 * and a human-readable name for the property.
 *
 * Note that access to a user's id, name, first_name, last_name, gender, locale,
 * link, username, third_party_id, timezone, updated_time, and verified
 * properties are always available if the user grants generic access to your
 * application.
 *
 * @param $include_common
 *   Optionally include all common properties in this list.
 *
 * @see fboauth_user_connections()
 * @see http://developers.facebook.com/docs/reference/api/user/
 * @see http://developers.facebook.com/docs/authentication/permissions/
 */
function fboauth_user_properties($include_common = FALSE) {
  $properties = array(
    'about' => array(
      'permission' => 'user_about_me',
      'label' => t('About me (a short bio)'),
      'field_types' => array('text', 'text_long'),
    ),
    'bio' => array(
      'permission' => 'user_about_me',
      'label' => t('Biography'),
      'field_types' => array('text_long'),
    ),
    'birthday' => array(
      'permission' => 'user_birthday',
      'label' => t('Birthday'),
      'field_types' => array('text', 'date', 'datetime', 'datestamp'),
    ),
    'education' => array(
      'permission' => 'user_education_history',
      'label' => t('Education history'),
    ),
    'email' => array(
      'permission' => 'email',
      'label' => t('E-mail'),
    ),
    'hometown' => array(
      'permission' => 'user_hometown',
      'label' => t('Hometown'),
      'field_types' => array('text'),
    ),
    'location' => array(
      'permission' => 'user_location',
      'label' => t('Location'),
      'field_types' => array('text'),
    ),
    'relationship_status' => array(
      'permission' => 'user_relationships',
      'label' => t('Relationship status (Single, Married, It\'s complicated, etc.)'),
      'field_types' => array('text', 'list_text'),
    ),
    'interested_in' => array(
      'permission' => 'user_relationship_details',
      'label' => t('Interested in (Men, Women)'),
    ),
    'significant_other' => array(
      'permission' => 'user_relationship_details',
      'label' => t('Signficant other'),
      'field_types' => array('text'),
    ),
    'political' => array(
      'permission' => 'user_religion_politics',
      'label' => t('Political view'),
      'field_types' => array('text'),
    ),
    'quotes' => array(
      'permission' => 'user_about_me',
      'label' => t('Favorite quotes'),
    ),
    'religion' => array(
      'permission' => 'user_religion_politics',
      'label' => t('Religion'),
      'field_types' => array('text'),
    ),
    'website' => array(
      'permission' => 'user_website',
      'label' => t('Website'),
      'field_types' => array('text'),
    ),
    'work' => array(
      'permission' => 'user_work_history',
      'label' => t('Work history'),
    ),
  );

  // Common properties. Always defined so that modules may alter if needed.
  $common = array(
    'id' => array(
      'label' => t('Facebook ID'),
    ),
    'name' => array(
      'label' => t('Full name'),
      'field_types' => array('text'),
    ),
    'first_name' => array(
      'label' => t('First name'),
      'field_types' => array('text'),
    ),
    'last_name' => array(
      'label' => t('Last name'),
      'field_types' => array('text'),
    ),
    'gender' => array(
      'label' => t('Gender'),
      'field_types' => array('text', 'list_text'),
    ),
    'locale' => array(
      'label' => t('Locale'),
    ),
    'link' => array(
      'label' => t('Facebook profile link'),
      'field_types' => array('text'),
    ),
    'username' => array(
      'label' => t('Username'),
    ),
    'third_party_id' => array(
      'label' => t('3rd-party ID'),
    ),
    'timezone' => array(
      'label' => t('Timezone'),
    ),
    'updated_time' => array(
      'label' => t('Updated time'),
    ),
    'verified' => array(
      'label' => t('Verified')
    ),
  );
  $properties += $common;

  drupal_alter('fboauth_user_properties', $properties);
  ksort($properties);

  // Filter out unneeded properties.
  if (!$include_common) {
    $properties = array_diff_key($properties, $common);
  }

  return $properties;
}

/**
 * Return a list of Facebook connection points.
 *
 * This function provides a list of all of the Facebook GraphAPI connection
 * points that can be access to learn extended information about a user. Usually
 * each of these connection points will allow querying against content the user
 * has created as opposed to information directly about the user.
 *
 * Each connection requires extended permission granted by the end-user. The
 * returned array of connections provides the name of each required permission
 * and a human-readable name for the connection.
 *
 * Note that access to the "picture" and "friends" connections are always
 * allowed if the user grants generic access to your application.
 *
 * @see fboauth_user_properties()
 * @see http://developers.facebook.com/docs/reference/api/user/
 * @see http://developers.facebook.com/docs/authentication/permissions/
 */
function fboauth_user_connections() {
  return array(
    'accounts' => array(
      'permission' => 'manage_pages',
      'label' => t('Account pages'),
    ),
    'activities' => array(
      'permission' => 'user_activities',
      'label' => t('Activities'),
    ),
    'apprequests' => array(
      'label' => t('App requests'),
      // No permission.
    ),
    'albums' => array(
      'permission' => 'user_photos',
      'label' => t('Photo albums'),
    ),
    'books' => array(
      'permission' => 'user_likes',
      'label' => t('Books liked'),
    ),
    'checkins' => array(
      'permission' => 'user_checkins',
      'label' => t('Check-ins'),
    ),
    'events' => array(
      'permission' => 'user_events',
      'label' => t('Events'),
    ),
    'feed' => array(
      'permission' => 'read_stream',
      'label' => t('User\'s wall'),
    ),
    'friendlists' => array(
      'permission' => 'read_friendlists',
      'label' => t('Lists of friends'),
    ),
    'home' => array(
      'permission' => 'read_stream',
      'label' => t('User\'s news feed'),
    ),
    'inbox' => array(
      'permission' => 'read_mailbox',
      'label' => t('Inbox threads'),
    ),
    'interests' => array(
      'permission' => 'user_interests',
      'label' => t('Interests'),
    ),
    'likes' => array(
      'permission' => 'user_likes',
      'label' => t('Pages liked'),
    ),
    'links' => array(
      'permission' => 'read_stream',
      'label' => t('Posted links'),
    ),
    'movies' => array(
      'permission' => 'user_likes',
      'label' => t('Movies liked'),
    ),
    'music' => array(
      'permission' => 'user_likes',
      'label' => t('Music liked'),
    ),
    'notes' => array(
      'permission' => 'read_stream',
      'label' => t('Notes'),
    ),
    'outbox' => array(
      'permission' => 'read_mailbox',
      'label' => t('Outbox'),
    ),
    'photos' => array(
      'permission' => 'user_photos',
      'label' => t('Photos'),
    ),
    'posts' => array(
      'permission' => 'read_stream',
      'label' => t('Posts'),
    ),
    'statuses' => array(
      'permission' => 'read_stream',
      'label' => t('Status updates'),
    ),
    'tagged' => array(
      'permission' => 'read_stream',
      'label' => t('Tagged in photos, videos, and posts'),
    ),
    'television' => array(
      'permission' => 'user_likes',
      'label' => t('Television liked'),
    ),
    'updates' => array(
      'permission' => 'read_mailbox',
      'label' => t('Inbox updates'),
    ),
    'videos' => array(
      'permission' => 'user_videos',
      'label' => t('Videos'),
    ),
  );
}

/**
 * Utility function to retrieve all permissions required for Facebook connect.
 */
function fboauth_user_connect_permissions() {
  $connect_permissions = array();
  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_email', TRUE) ? array('email') : array());
  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_properties', array()));
  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_connections', array()));

  if (module_exists('profile')) {
    $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_profile', array()));
  }

  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_fields', array()));

  return $connect_permissions;
}

/**
 * Execute a Graph API query through Facebook.
 *
 * @see http://developers.facebook.com/docs/reference/api/
 */
function fboauth_graph_query($id, $access_token = NULL, $parameters = array(), $method = 'GET') {
  if (isset($access_token)) {
    $parameters['access_token'] = $access_token;
  }

  if ($method == 'GET') {
    $graph_url = url('https://graph.facebook.com/' . $id, array('absolute' => TRUE, 'query' => $parameters));
    $graph_result = drupal_http_request($graph_url);
  }
  elseif ($method == 'POST') {
    $graph_url = 'https://graph.facebook.com/' . $id;
    $post_data = http_build_query($parameters, '', '&');
    $graph_result = drupal_http_request($graph_url, array('headers' => array(), 'method' => $method, 'data' => $post_data));
  }
  else {
    drupal_set_message(t('Invalid request type "@type" for Facebook graphy query. Must be either @get or @post.', array('@type' => $method, '@get' => 'GET', '@post' => 'POST')), 'error');
  }

  // If the response contains a redirect (such as to an image), return the
  // redirect as the data. i.e. https://graph.facebook.com/19292868552/picture.
  if (isset($graph_result->redirect_url)) {
    $data = array(
      'data' => $graph_result->data,
      'redirect_code' => $graph_result->redirect_code,
      'redirect_url' => $graph_result->redirect_url,
    );
  }
  else {
    $data = json_decode($graph_result->data);
  }

  return $data;
}

/**
 * Execute a legacy REST API method through Facebook.
 *
 * @see http://developers.facebook.com/docs/reference/rest/
 */
function fboauth_method_invoke($method, $access_token, $parameters = array()) {
  // Provide the default App ID for all requests if not specified.
  if (!isset($parameters['app_id'])) {
    $parameters['app_id'] = variable_get('fboauth_id', '');
  }

  $parameters['access_token'] = $access_token;

  // Keep all results in JSON for simplicity.
  $parameters['format'] = 'json';

  // Perform the REST query.
  $query_string = drupal_http_build_query($parameters);
  $result = drupal_http_request('https://api.facebook.com/method/' . $method . '?' . $query_string);
  return json_decode($result->data);
}

/**
 * Process a deauthorization request from Facebook.
 *
 * @see https://developers.facebook.com/docs/authentication/signed_request/
 */
function fboauth_deauthorize() {
  // A signed_request key is used when a deauth url is provided by the app.
  if (isset($_POST['signed_request'])) {
    $app_secret = variable_get('fboauth_secret', '');
    $signed_request = fboauth_parse_signed_request($_POST['signed_request'], $app_secret);

    if ($signed_request && $deauth_uid = fboauth_uid_load($signed_request['user_id'])) {
      fboauth_save($deauth_uid, NULL);
      watchdog('fboauth', 'The account for UID @uid has been disconnected from Facebook by the user via Facebook.', array('@uid' => $deauth_uid));

      // Allow other modules to hook into a deauth event.
      module_invoke_all('fboauth_deauthorize', $deauth_uid, $signed_request['user_id']);
    }
  }
}

/**
 * Parse a signed_request from Facebook.
 *
 * @see http://developers.facebook.com/docs/authentication/signed_request/
 */
function fboauth_parse_signed_request($signed_request, $secret) {
  list($encoded_signature, $payload) = explode('.', $signed_request, 2);

  // Decode the data.
  $signature = fboauth_base64_url_decode($encoded_signature);
  $data = json_decode(fboauth_base64_url_decode($payload), true);

  if (strtoupper($data['algorithm']) !== 'HMAC-SHA256') {
    watchdog('fboauth', 'A Facebook deauthorization request failed: Unknown signed request algorithm. Expected HMAC-SHA256.');
    return null;
  }

  // Check the signature.
  $expected_signature = hash_hmac('sha256', $payload, $secret, $raw = true);
  if ($signature !== $expected_signature) {
    watchdog('fboauth', 'A Facebook deauthorization request failed: Bad Signed JSON signature!');
    return null;
  }

  return $data;
}

/**
 * Helper function for signed_requests from Facebook.
 *
 * @param $input
 *   A string of text passed back from a Facebook signed request.
 * @return
 *   The decoded string.
 *
 * @see fboauth_parse_signed_request()
 */
function fboauth_base64_url_decode($input) {
  return base64_decode(strtr($input, '-_', '+/'));
}