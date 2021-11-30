<?php

/**
 * @file
 * Twitter API functions
 */

module_load_include('php', 'oauth_common', 'lib/OAuth');

/**
 * Connect to the Twitter API.
 *
 * @param object $twitter_account
 *   An authenticated twitter_account object to be used to authenticate against
 *   Twitter.
 * @param bool $access_global
 *   Whether or not to load the global accounts too.
 * @param bool $force
 *   Loads accounts regardless of other access limits. Should be used with care
 *   and only for limited scenarios that do would not pose a security risk.
 *
 * @return
 *   A Twitter object ready to be used to query the Twitter API or FALSE.
 */
function twitter_connect($twitter_account = NULL, $access_global = FALSE, $force = FALSE) {
  // If no account was specified, load the first authenticated account.
  if (empty($twitter_account)) {
    // Load all authenticated accounts, passing in the same access requirements.
    $accounts = twitter_load_authenticated_accounts(NULL, $access_global, $force);

    // If no accounts were found, there's a problem.
    if (empty($accounts)) {
      watchdog('twitter', 'There are no authenticated Twitter accounts to use for API connections.');
    }

    // Grab the first authenticated account.
    else {
      $twitter_account = reset($accounts);
    }
  }

  if (!empty($twitter_account)) {
    $auth = $twitter_account->get_auth();
    if (isset($auth['oauth_token']) && isset($auth['oauth_token_secret'])) {
      return new Twitter(
        variable_get('twitter_consumer_key', ''),
        variable_get('twitter_consumer_secret', ''),
        $auth['oauth_token'],
        $auth['oauth_token_secret']
      );
    }
  }

  return FALSE;
}

/**
 * Saves a TwitterUser object to {twitter_account}
 */
function twitter_account_save($twitter_user, $save_auth = FALSE) {
  $values = (array) $twitter_user;
  $values['twitter_uid'] = $values['id'];
  foreach (array('protected', 'verified', 'profile_background_tile') as $k) {
    if (isset($values[$k])) {
      $values[$k] = (int) $values[$k];
    }
  }

  if ($save_auth) {
    $values += $twitter_user->get_auth();
  }
  $schema = drupal_get_schema('twitter_account');
  foreach ($values as $k => $v) {
    if (!isset($schema['fields'][$k])) {
      unset($values[$k]);
    }
  }
  db_merge('twitter_account')
    ->key(array('twitter_uid' => $values['twitter_uid']))
    ->fields($values)
    ->execute();

  // Notify other modules of the twitter account save
  module_invoke_all('twitter_account_save', $values);
}

/**
 * Load a Twitter account from {twitter_account}.
 *
 * @param mixed $id
 *   Either the Twitter User ID or screen name.
 * @param boolean $type
 *   By default the $id is assumed to be a Twitter UID, set to FALSE if $id is a
 *   screen name.
 *
 * @return
 *   TwitterUser object or NULL.
 */
function twitter_account_load($id, $is_uid = TRUE) {
  // The ID is the Twitter UID.
  if ($is_uid) {
    $query = db_query('SELECT *
                       FROM {twitter_account}
                       WHERE twitter_uid = :twitter_uid',
                       array(':twitter_uid' => $id));
  }
  // Otherwise use the screen name.
  else {
    $query = db_query('SELECT *
                       FROM {twitter_account}
                       WHERE screen_name = :screen_name',
                       array(':screen_name' => $id));
  }
  $values = $query->fetchAssoc();
  if (!empty($values)) {
    $values['id'] = $values['twitter_uid'];
    $account = new TwitterUser($values);
    $account->set_auth($values);
    $account->import = $values['import'];
    $account->mentions = $values['mentions'];
    $account->is_global = $values['is_global'];
    return $account;
  }
  return NULL;
}

/**
 * Loads all Twitter accounts added by a Drupal user.
 *
 * This excludes Twitter accounts added automatically when e.g. pulling mentions
 * of an account from the Twitter API.
 *
 * @return array
 *   A list of TwitterUser objects.
 */
function twitter_load_accounts() {
  $accounts = array();
  $result = db_query('SELECT twitter_uid
    FROM {twitter_account}
    WHERE uid <> 0
    ORDER BY screen_name');
  foreach ($result as $account) {
    $accounts[] = twitter_account_load($account->twitter_uid);
  }
  return $accounts;
}

/**
 * Bare list of all account names, keyed by Twitter ID.
 *
 * @return array
 *   A list of TwitterUser objects.
 */
function twitter_load_account_names() {
  $accounts = array();
  foreach (twitter_load_accounts() as $account) {
    $accounts[$account->id] = check_plain($account->name);
  }
  return $accounts;
}

/**
 * Returns a list of authenticated global or user-specific Twitter accounts.
 *
 * @param int $uid
 *   Optional Drupal user ID to limit the results against.
 * @param bool $access_global
 *   Whether or not to load the global accounts too.
 * @param bool $force
 *   Loads accounts regardless of other access limits. Should be used with care
 *   and only for limited scenarios that do would not pose a security risk.
 *
 * @return array
 *   List of TwitterUser objects. Will always include global accounts, will
 *   optionally include accounts from a specific user.
 */
function twitter_load_authenticated_accounts($uid = NULL, $access_global = TRUE, $force = FALSE) {
  $auth_accounts = array();

  // Load every Twitter account, check to see if each one is suitable.
  foreach (twitter_load_accounts() as $index => $twitter_account) {
    // Only include authenticated accounts.
    if ($twitter_account->is_auth()) {
      // Only include either global accounts or, if a $uid was passed in,
      // accounts that are owned by the requested user.
      if ($force
        || ($access_global && $twitter_account->is_global)
        || (isset($uid) && $uid == $twitter_account->uid)) {
        $auth_accounts[] = $twitter_account;
      }
    }
  }

  return $auth_accounts;
}

/**
 * Load a Twitter status.
 *
 * @param $status_id
 *   The status id of this tweet.
 *
 * @return
 *   An instance of stdClass object with the Tweet data or FALSE.
 */
function twitter_status_load($status_id) {
  $row = db_query("SELECT * FROM {twitter} WHERE twitter_id = :status_id",
            array(':status_id' => $status_id))->fetchObject();

  if (!empty($row->entities)) {
    $row->entities = unserialize($row->entities);
  }

  return $row;
}

/**
 * Saves a TwitterStatus object to {twitter}
 */
function twitter_status_save($status) {
  // RT's get special handling.
  if (!empty($status->retweeted_status)) {
    $text = 'RT @' . $status->retweeted_status->user->screen_name . ': ' . $status->retweeted_status->text;
  }
  else {
    $text = $status->text;
  }

  $row = array(
    'twitter_id' => $status->id,
    'screen_name' => $status->user->screen_name,
    'created_time' => strtotime($status->created_at),
    'text' => $text,
    'source' => $status->source,
    'in_reply_to_status_id' => ($status->in_reply_to_status_id > 0) ? (string) $status->in_reply_to_status_id : NULL,
    'in_reply_to_user_id' => $status->in_reply_to_user_id,
    'in_reply_to_screen_name' => $status->in_reply_to_screen_name,
    'truncated' => (int) $status->truncated,
    'entities' => isset($status->entities) ? serialize($status->entities) : NULL,
  );
  db_merge('twitter')
    ->key(array('twitter_id' => $row['twitter_id']))
    ->fields($row)
    ->execute();
  // Let other modules know that a status has been saved.
  module_invoke_all('twitter_status_save', $status);
}

/**
 * Post a message to twitter
 *
 * @param $twitter_account
 *   object with a Twitter account.
 * @param $status
 *   string message to publish.
 * @return
 *   array response from Twitter API.
 */
function twitter_set_status($twitter_account, $status) {
  $twitter = twitter_connect($twitter_account);
  if (empty($twitter)) {
    return FALSE;
  }
  return $twitter->statuses_update($status);
}

/**
 * Retrieve an embedded Tweet.
 *
 * @param int $tweet_id
 *   Status ID of the tweet.
 *
 * @return array
 *   Response from Twitter API.
 */
function twitter_statuses_oembed($tweet_id) {
  // Need to request any global account, though preferably a global account.
  $twitter = twitter_connect(NULL, TRUE, TRUE);
  if (empty($twitter)) {
    watchdog('twitter', 'Unable to load an authenticated Twitter account to embed a tweet.');
    return FALSE;
  }

  else {
    $params = array(
      'hide_media' => !variable_get('twitter_media', FALSE),
      'hide_thread' => !variable_get('twitter_conversation', FALSE),
      'align' => variable_get('twitter_align', 'none'),
    );
    return $twitter->statuses_oembed($tweet_id, $params);
  }
}

/**
 * Fetches a user's timeline.
 */
function twitter_fetch_user_timeline($id) {
  $twitter_account = twitter_account_load($id);
  $since = db_query("SELECT MAX(twitter_id) FROM {twitter} WHERE screen_name = :screen_name", array(':screen_name' => $twitter_account->screen_name))->fetchField();

  // Connect to the Twitter's API. If the account is authenticated, use that
  // account, otherwise grab the first authenticated account.
  if ($twitter_account->is_auth()) {
    $twitter = twitter_connect($twitter_account);
  }
  else {
    $twitter = twitter_connect(NULL, TRUE, TRUE);
  }

  // That should have worked, but there might have been a problem.
  if (empty($twitter)) {
    watchdog('twitter', 'Unable to authenticate to download tweets for the %name account.', array('%name' => $twitter_account->screen_name));
    return FALSE;
  }

  // Fetch tweets.
  $params = array();
  if (!empty($since)) {
    $params['since_id'] = $since;
  }

  // Trigger hook_twitter_prefetch_timeline().
  // Doing it this way so that the hook may adjust the parameters.
  $hook = 'twitter_prefetch_timeline';
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    if (function_exists($function)) {
      $function($twitter_account, $params);
    }
  }

  // Load the timeline.
  $statuses = $twitter->user_timeline($id, $params);

  // Trigger hook_twitter_statuses_alter().
  drupal_alter('twitter_statuses', $statuses, $twitter_account);

  // Save each tweet.
  if (count($statuses) > 0) {
    watchdog('twitter', 'Downloaded %count tweets for the %name account.', array('%count' => count($statuses), '%name' => $twitter_account->screen_name));

    foreach ($statuses as $status) {
      twitter_status_save($status);
    }
  }

  // Trigger hook_twitter_insert_statuses().
  module_invoke_all('twitter_insert_statuses', $statuses, $twitter_account);

  // Update account details.
  if (count($statuses) > 0) {
    twitter_account_save($statuses[0]->user);
  }
}

/**
 * Retweet a tweet.
 */
function twitter_retweet($twitter_account, $tweet_id) {
  module_load_include('lib.php', 'twitter');

  $twitter = twitter_connect($twitter_account);
  $twitter->retweet($tweet_id, $twitter_account);
}

/**
 * Fetches user's mentions of an authenticated account.
 */
function twitter_fetch_mentions_timeline($id) {
  $twitter_account = twitter_account_load($id);

  // Connect to Twitter's API using the authenticated account to fetch mentions.
  $twitter = twitter_connect($twitter_account);
  if (empty($twitter)) {
    watchdog('twitter', 'Unable to authenticate to download mentions for the %name account.', array('%name' => $twitter_account->screen_name));
    return FALSE;
  }

  $params = array();
  $statuses = $twitter->mentions_timeline($params);
  foreach ($statuses as $status) {
    if (!twitter_account_load($status->user->id)) {
      twitter_account_save($status->user);
    }
    twitter_status_save($status);
  }
}

/**
 * Pulls tweets from the database.
 *
 * @param string $screen_name
 *   Optionally provide a screen_name to filter.
 */
function twitter_tweets($screen_name = NULL) {
  $query = db_select('twitter', 't')
    ->fields('t');
  if (isset($screen_name)) {
    $query->condition('t.screen_name', $screen_name);
  }
  $result = $query->execute();

  $tweets = array();
  foreach ($result as $row) {
    if (!empty($row->entitites)) {
      $row->entities = unserialize($row->entities);
    }
    $tweets[] = $row;
  }
  return $tweets;
}

/**
 * Delete a twitter account and its statuses.
 *
 * @param $twitter_uid
 *   An integer with the Twitter UID.
 */
function twitter_account_delete($twitter_uid) {
  $twitter_account = twitter_account_load($twitter_uid);

  // Delete from {twitter_account}.
  $query = db_delete('twitter_account');
  $query->condition('twitter_uid', $twitter_uid);
  $query->execute();

  // Delete from {twitter}.
  $query = db_delete('twitter');
  $query->condition('screen_name', $twitter_account->screen_name);
  $query->execute();

  // Delete from {authmap}.
  $query = db_delete('authmap');
  $query->condition('authname', $twitter_uid);
  $query->condition('module', 'twitter');
  $query->execute();
}
