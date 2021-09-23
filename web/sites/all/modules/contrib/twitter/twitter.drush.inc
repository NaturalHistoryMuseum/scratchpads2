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
  return $items;
}

/**
 * Implements drush_COMMANDFILE_COMMANDNAME()
 *
 * Searches for a keyword at Twitter and return the results.
 */
function drush_twitter_search($keyword) {
  $keyword = urlencode($keyword);
  // This is not even using the Twitter library at twitter.lib.inc, but it will.
  $url = 'http://search.twitter.com/search.json?rpp=100&q=';

  $response = drupal_http_request($url . $keyword);

  if (isset($response->code) && ($response->code == 200)) {
    $data = json_decode($response->data);
    if (!count($data->results)) {
      drush_set_error(dt('No tweets found for this keyword.'));
    }
    else {
      drush_print(dt('There are !total tweets containing \'@keyword\'.', array(
        '!total' => count($data->results),
        '@keyword' => $keyword,
      )));
      $tweets = $data->results;

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
          '!user' => $tweet->from_user,
          '!tweet' => $tweet->text,
        )));
        drush_print('');
      }
    }
  }
  else {
    drush_set_error(dt('There was an error. Full raw response was !response', array(
      '!response' => print_r($response, TRUE)
    )));
  }
}
