<?php

/**
 * @file
 * Twitter API functions
 */

module_load_include('php', 'oauth_common', 'lib/OAuth');

/**
 * Connect to the Twitter API.
 *
 * @param object $account
 *   An authenticated twitter_account object to be used to authenticate against
 *   Twitter.
 * @return
 *   a Twitter object ready to be used to query the Twitter API or FALSE.
 */
function twitter_connect($account = NULL) {
  if (!$account) {
    // Load the first authenticated account.
    $twitter_uid = db_query("SELECT twitter_uid
                             FROM {twitter_account}
                             WHERE oauth_token <> ''
                               AND oauth_token_secret <> '' ")->fetchField();
    $account = twitter_account_load($twitter_uid);
  }
  if ($account) {
    $auth = $account->get_auth();
    if (isset($auth['oauth_token']) && isset($auth['oauth_token_secret'])) {
      return new Twitter(variable_get('twitter_consumer_key', ''), variable_get('twitter_consumer_secret', ''),
                              $auth['oauth_token'], $auth['oauth_token_secret']);
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
 *   int Twitter User id or string Twitter user screen name.
 *
 * @return
 *   TwitterUser object or NULL.
 */
function twitter_account_load($id) {
  $values = db_query('SELECT *
                      FROM {twitter_account}
                      WHERE twitter_uid = :id_1
                      OR screen_name  = :id_2',
                      array(':id_1' => $id, ':id_2' => $id))
              ->fetchAssoc();
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
 * Loads all Twitter accounts added by a user.
 *
 * @return
 *   array of TwitterUser objects.
 */
function twitter_account_load_all() {
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
 * Returns a list of authenticated Twitter accounts.
 *
 * @return
 *   array of TwitterUser objects.
 */
function twitter_load_authenticated_accounts() {
  $accounts = twitter_account_load_all();
  $auth_accounts = array();
  foreach ($accounts as $index => $account) {
    if ($account->is_auth()) {
      $auth_accounts[] = $account;
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
  return db_query("SELECT * FROM {twitter} WHERE twitter_id = :status_id",
           array(':status_id' => $status_id))->fetchObject();
}

/**
 * Saves a TwitterStatus object to {twitter}
 */
function twitter_status_save($status) {
  $row = array(
    'twitter_id' => $status->id,
    'screen_name' => $status->user->screen_name,
    'created_time' => strtotime($status->created_at),
    'text' => $status->text,
    'source' => $status->source,
    'in_reply_to_status_id' => ($status->in_reply_to_status_id > 0) ? (string) $status->in_reply_to_status_id : NULL,
    'in_reply_to_user_id' => (int) $status->in_reply_to_user_id,
    'in_reply_to_screen_name' => $status->in_reply_to_screen_name,
    'truncated' => (int) $status->truncated,
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
  return $twitter->statuses_update($status);
}

/**
 * Fetches a user's timeline.
 */
function twitter_fetch_user_timeline($id) {
  $account = twitter_account_load($id);
  $since = db_query("SELECT MAX(twitter_id) FROM {twitter} WHERE screen_name = :screen_name", array(':screen_name' => $account->screen_name))->fetchField();

  // Connect to the Twitter's API.
  $twitter = twitter_connect();
  $params = array();
  if ($since) {
    $params['since_id'] = $since;
  }

  // Fetch tweets.
  $statuses = $twitter->user_timeline($id, $params);
  foreach ($statuses as $status) {
    twitter_status_save($status);
  }

  if (count($statuses) > 0) {
    // Update account details.
    twitter_account_save($statuses[0]->user);
  }
}

/**
 * Fetches user's mentions of an authenticated account.
 */
function twitter_fetch_mentions_timeline($id) {
  $account = twitter_account_load($id);
  // Connect to Twitter's API using the authenticated account to fetch mentions.
  $twitter = twitter_connect($account);

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
  $account = twitter_account_load($twitter_uid);

  // Delete from {twitter_account}.
  $query = db_delete('twitter_account');
  $query->condition('twitter_uid', $twitter_uid);
  $query->execute();

  // Delete from {twitter}.
  $query = db_delete('twitter');
  $query->condition('screen_name', $account->screen_name);
  $query->execute();

  // Delete from {authmap}.
  $query = db_delete('authmap');
  $query->condition('authname', $twitter_uid);
  $query->condition('module', 'twitter');
  $query->execute();
}
