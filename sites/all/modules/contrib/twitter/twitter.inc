<?php

/**
 * @file
 * Twitter API functions
 */

module_load_include('php', 'oauth_common', 'lib/OAuth');
require_once(libraries_get_path('twitter') . '/twitter.lib.php');

/**
 * Connect to the Twitter API.
 */
function twitter_connect($account) {
  $auth = $account->get_auth();
  if (isset($auth['oauth_token']) && isset($auth['oauth_token_secret'])) {
    return new Twitter(variable_get('twitter_consumer_key', ''), variable_get('twitter_consumer_secret', ''),
                            $auth['oauth_token'], $auth['oauth_token_secret']);
  }
  else {
    return FALSE;
  }
}

/**
 * Saves a TwitterUser object to {twitter_account}
 */
function twitter_account_save($twitter_user, $save_auth = FALSE, $account = NULL) {
  $values = (array) $twitter_user;
  $values['twitter_uid'] = $values['id'];
  // bool => int for DB storage
  foreach (array('protected', 'verified', 'profile_background_tile') as $k) {
    if (isset($values[$k])) {
      $values[$k] = (int) $values[$k];
    }
  }

  if ($save_auth) {
    $values += $twitter_user->get_auth();
    if (empty($account)) {
      global $user;
      $account = $user;
    }
    $values['uid'] = $account->uid;
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
  if (!empty($account)) {
    module_invoke_all('twitter_account_save', $account, $values);
  }
}

/**
 * Load a Twitter account from {twitter_account}.
 *
 * @param $id
 *   Twitter UID
 *
 * @return
 *   TwitterUser object
 *
 */
function twitter_account_load($id) {
  if ( $values = db_query("SELECT * FROM {twitter_account} WHERE twitter_uid = :twitter_uid", array(':twitter_uid' => $id))->fetchAssoc() ) {
    $values['id'] = $values['twitter_uid'];
    $account = new TwitterUser($values);
    $account->set_auth($values);
    $account->uid = $values['uid'];
    $account->import = $values['import'];
    $account->is_global = $values['is_global'];
    return $account;
  }
}

/**
 * Saves a TwitterStatus object to {twitter}
 */
function twitter_status_save($status) {
  $status = array(
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
    ->key(array('twitter_id' => $status['twitter_id']))
    ->fields($status)
    ->execute();
  // Let other modules know that an status has been just saved.
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
 * Fetches a user's timeline
 */
function twitter_fetch_user_timeline($id) {
  $account = twitter_account_load($id);

  $since = db_query("SELECT MAX(twitter_id) FROM {twitter} WHERE screen_name = :screen_name", array(':screen_name' => $account->screen_name))->fetchField();

  $twitter = twitter_connect($account);

  $params = array();
  if ($since) {
    $params['since_id'] = $since;
  }

  $statuses = $twitter->user_timeline($account->id, $params);
  foreach ($statuses as $status) {
    twitter_status_save($status);
  }

  if (count($statuses) > 0) {
    twitter_account_save($statuses[0]->user);
  }

  db_update('twitter_account')
    ->fields(array(
      'last_refresh' => REQUEST_TIME,
    ))
    ->condition('twitter_uid', $account->id)
    ->execute();
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
