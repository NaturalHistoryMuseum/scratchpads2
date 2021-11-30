<?php
/**
 * @file
 * Drush commands for the Twitter module.
 */

/**
 * Implements COMMANDFILE_drush_command()
 */
function twitter_drush_command() {
  $items = array();

  $items['twitter-search'] = array(
    'description' => 'Searches the Twitter API for something.',
    'arguments' => array(
      'keyword' => 'The keyword you are searching for. Add @ for usernames and # for hashtags.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'limit' => 'Limit the number of results to be printed.',
      'randomize' => 'Randomize the result.',
    ),
    'examples' => array(
      'drush twitter-search \'#drupal\'' => 'Pull tweets containing the hashtag #drupal.',
      'drush twitter-search \'#drupalconmunich\' --limit=1 --randomize' =>
        'Picks a random tweet containing \'#drupalconmunich\'.',
    ),
    'aliases' => array('tws'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
  );

  $items['twitter-tweet'] = array(
    'aliases' => array('tweet'),
    'callback' => 'twitter_drush_tweet',
    'drupal dependencies' => array('twitter'),
    'description' => 'Tweets from command line, using one of the authenticated global accounts or accounts owned by a specific user.',
    'arguments' => array(
      'tweet' => 'Text of the tweet.',
      'screen_name' => 'Screen name of the account to post from.',
    ),
    'examples' => array(
      'drush tweet \'It works!\'' => 'Post a short test tweet. You will be prompted to choose among authenticated accounts.',
      'drush tweet \'Now what?\' JohnDoe' => 'Post another short tweet on behalf of \'JohnDoe\'.',
      'drush tweet --user=42 \'Now what?\' JohnDoe' => 'Post another short tweet on behalf of \'JohnDoe\', which is owned by user 42.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Implements drush_COMMANDFILE_COMMANDNAME()
 *
 * Searches for a keyword at Twitter and return the results.
 */
function drush_twitter_search($keyword) {
  // Set up a Twitter object to query with.
  module_load_include('inc', 'twitter');
  $twitter = twitter_connect(NULL, TRUE, TRUE);
  if (empty($twitter)) {
    drush_set_error(dt('At least one Twitter account must be authenticated at admin/config/services/twitter in order to perform requests to the Twitter API.'));
    return FALSE;
  }

  // Build the query and get the response.
  $url = variable_get('twitter_api', TWITTER_API) . '/1.1/search/tweets.json';
  $data = $twitter->auth_request($url, array('q' => $keyword));
  $data = json_decode($data);

  if (!count($data->statuses)) {
    drush_set_error(dt('No tweets found for this keyword.'));
  }
  else {
    drush_print(dt('There are !total tweets containing \'@keyword\'.', array(
      '!total' => $data->search_metadata->count,
      '@keyword' => $keyword,
    )));
    $tweets = $data->statuses;

    // Should we randomize?
    if (drush_get_option('randomize')) {
      $results = shuffle($tweets);
    }

    // Should we limit the list of results?
    if (drush_get_option('limit')) {
      $tweets = array_slice($tweets, 0, drush_get_option('limit'));
    }

    // Print results
    foreach ($tweets as $tweet) {
      drush_print('');
      drush_print(dt('User "@!user", tweeted "!tweet".', array(
        '!user' => $tweet->user->screen_name,
        '!tweet' => $tweet->text,
      )));
      drush_print('');
    }
  }
}

/**
 * Callback for Drush command 'twitter-tweet'.
 *
 * Posts a new status to Twitter using one of authenticated accounts.
 */
function twitter_drush_tweet($tweet = NULL, $screen_name = NULL) {
  // Bail out if no tweet was supplied.
  if (!isset($tweet)) {
    return drush_set_error(dt('No tweet text was supplied.'));
  }

  // Bail out if the tweet is too long.
  if (drupal_strlen($tweet) > 140) {
    return drush_set_error(dt('This is !chars chars long, only 140 allowed.', array('!chars' => drupal_strlen($tweet))));
  }

  // Work out what user account to test from.
  $uid = intval(drush_get_option('user'));
  $account = user_load($uid);
  if ($uid > 0 && empty($account)) {
    return dt('User account @uid could not be found.', array('@uid' => $uid));
  }

  // Verify the user has permission to post to Twitter.
  if (!user_access('post to twitter', $account)) {
    return dt('User account @uid does not have permission to tweet.', array('@uid' => $uid));
  }

  // Get a list of all authenticated accounts for the current user. If no user
  // was selected it will default to the anonymous user and therefore only list
  // global accounts.
  module_load_include('inc', 'twitter');
  $access_global = user_access('post to twitter with global account', $account);
  $accounts = array();
  foreach (twitter_load_authenticated_accounts($account->uid, $access_global) as $twitter_account) {
    $accounts[$twitter_account->screen_name] = $twitter_account;
  }

  // Bail out if there are no authenticated accounts.
  if (empty($accounts)) {
    if ($uid > 0) {
      $message = dt('There are no global authenticated Twitter accounts or accounts owned by user @uid to post from.', array('@uid' => $uid));
    }
    else {
      $message = dt('There are no global authenticated Twitter accounts to post from.');
    }
    return drush_set_error($message);
  }

  // Bail out if a screen name was supplied but is not valid.
  if ($screen_name && !key_exists($screen_name, $available)) {
    return drush_set_error(dt("'!name' not available. Omit screen name to be prompted to choose among available accounts.", array('!name' => $screen_name)));
  }

  // If no screen name was supplied, prompt for it.
  if (empty($screen_name)) {
    $screen_name = drush_choice($accounts, 'Choose an account to post from.', '!key');
    if ($screen_name === FALSE) {
      return;
    }
  }

  // Tweet the message.
  try {
    twitter_set_status($accounts[$screen_name], $tweet);
    drush_log(dt('If you see nothing above this line, we successfully posted to Twitter.'), 'success');
  }
  // Pretend to catch exceptions. Somehow, they're not willing to be catched.
  catch (TwitterException $e) {
    drush_set_error(dt('An error occurred when posting to Twitter: @message', array('@message' => $e->getMessage())));
  }
}
