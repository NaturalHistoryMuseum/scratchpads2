<?php

/**
 * @file
 * twitter pull class implementation
 */

class twitter_puller {

  var $twitkey;
  var $num_items;
  var $tweets;
  var $rts;
  var $exclude_replies;

  /**
  * @param $twitkey
  *     Twitter key, which can be a username (prepended with @) or hashtag (prepended with #)
  * @param $num_items
  *     maximum number of tweets to pull from Twitter.
  */
  function __construct($twitkey, $num_items) {

    $this->twitkey = $twitkey;
    $this->num_items = $num_items;

    $this->check_arguments();

  }

  function check_arguments() {

    if (empty($this->twitkey) || drupal_strlen($this->twitkey) < 2) {
      throw new Exception(t('Twitter key may not be empty.'));
    }

    $num = intval($this->num_items);
    if ($num <= 0 || $num > 200) {
      throw new Exception(t('Number of Twitter items to pull must be a positive integer less than or equal to 200.'));
    }

  }

  function get_items() {

    /* First try to use the twitter module to get the tweets
     * At some point, authentication will be required and if you have the
     * twitter module set up properly, it will handle the authenticated session.
     */
    if ($this->twitter_get_items()) {
      return;
    }

    watchdog('Twitter Pull', 'Twitter Pull is using an Unauthenticated request to twitter apis.  Download, enable and configure the twitter module to allow for authenticated requests.');
    $prefix = drupal_substr($this->twitkey, 0, 1);
    $slash = strpos($this->twitkey, '/', 1);
    $num = intval($this->num_items);
    $rts = !empty($this->rts)?'1':'false';
    $exclude_replies = !empty($this->exclude_replies)?'true':'false';

    // lists have the format @username/listname
    if ($prefix == '@' && $slash !== FALSE) {
      $username = drupal_substr($this->twitkey, 1, $slash - 1);
      $listname = drupal_substr($this->twitkey, $slash + 1);
      $url = 'http://api.twitter.com/1/' . urlencode($username) . '/lists/' . urlencode($listname) . '/statuses.json?per_page=' . $num;
    }
    // if the first character is @, then consider the key a username
    elseif ($prefix == "@") {
      $key = drupal_substr($this->twitkey, 1);
      $url = 'http://api.twitter.com/1/statuses/user_timeline.json?screen_name=' . urlencode($key) . '&include_rts=' . $rts . '&count=' . $num . '&exclude_replies=' . $exclude_replies;
    }
    // if the first character is ~, then consider the key a favorites feed
    elseif ($prefix == "~") {
      $key = drupal_substr($this->twitkey, 1);
      $url = 'http://api.twitter.com/1/favorites/' . urlencode($key) . '.json?count=' . $num;
    }
    // otherwise, use the key as a search term
    else {
      if ($num > 100) {
        $num = 100;
      }
      $url = 'http://search.twitter.com/search.json?q=' . urlencode($this->twitkey) . '&rpp=' . $num;
    }

    $ret = drupal_http_request($url, array('timeout' => 2));

    if ($ret->code < 200 || $ret->code > 399) {
      $errmsg = t('An unknown error occurred.');
      if (isset($ret->error) && !empty($ret->error)) {
        $errmsg = check_plain($ret->error);
      }
      elseif (isset($ret->data) && !empty($ret->data)) {
        $errdata = json_decode($ret->data);
        if (isset($errdata->error) && !empty($errdata->error)) {
          $errmsg = check_plain($errmsg->error);
        }
      }
      if ($ret->code == 400) {
        $errmsg .= ' ' . t('This site may be subject to rate limiting. For more information, see: http://apiwiki.twitter.com/Rate-limiting');
      }
      throw new Exception(t('Could not retrieve data from Twitter.') . ' ' . $errmsg);
    }

    $items = json_decode($ret->data);
    $this->parse_items($items);

  }

  /**
   * Use the twitter module to get the results.
   *
   *  - The twitter module will use an authenticated session to get the tweets
   *  - via the twiter 1.1 API.
   */
  function twitter_get_items() {
    // Check for the twitter module.
    if (!module_exists('twitter')) {
      return FALSE;
    }
    // Load the twitter module.
    module_load_include('inc', 'twitter');

    // Get twitkey statistics.
    $prefix = drupal_substr($this->twitkey, 0, 1);
    $slash = strpos($this->twitkey, '/', 1);
    $num = intval($this->num_items);
    $key = drupal_substr($this->twitkey, 1);
    $rts = !empty($this->rts)?'1':'false';
    $exclude_replies = !empty($this->exclude_replies)?'true':'false';

    // Start building the parameters for the twitter api.
    $params = array(
      'count' => $num,
    );

    // Determin the type of request.
    // Set up the path and params according to the type of request.
    switch ($prefix) {
      case "@":
        if ($slash === FALSE) {
          // Just a normal user timeline.
          $path                         = "statuses/user_timeline";
          $params['screen_name']        = $key;
          $params['include_rts']        = $rts;
          $params['exclude_replies']    = $exclude_replies;
        }
        else {
          // Since we have at least one slash, we are going to get a list.
          $path                         = "lists/statuses";
          $params['owner_screen_name']  = drupal_substr($this->twitkey, 1, $slash - 1);
          $params['slug']               = drupal_substr($this->twitkey, $slash + 1);
          $params['include_rts']        = $rts;
          $params['exclude_replies']    = $exclude_replies;
        }
        break;

      case "~":
        // Looking for favorites.
        $path                         = "favorites/list";
        $params['screen_name']        = $key;
        break;

      default:
        // Default to a not=rmal search.
        $path             = "search/tweets";
        $params['count']  = $num > 100 ? 100 : $num;
        $params['q']      = $this->twitkey;
        break;
    }

    $twitter = twitter_connect();
    if (!$twitter) {
      watchdog('Twitter Pull', 'Twitter module is not properly configured and could not be used.');
      return FALSE;
    }

    // Try to load the status via the twitter api (from the twitter module).
    $result = $twitter->call($path, $params, "GET");

    // Create/Empty the tweets array.
    $this->tweets = array();

    // Search results return metadata and the rusults in a sub-array
    // We need to parse the actual result array.
    $result = isset($result['statuses']) ? $result['statuses'] : $result;

    // Proccess the tweets for twitter_pull compatibility.
    $this->parse_items($result);

    // Return if we got any results.
    return (count($this->tweets) > 0);
  }

  function parse_items($items) {
    $tweets = array();

    //-- If search response then items are one level lower.
    if (isset($items->results) && is_array($items->results)) {
      $items = $items->results;
    }

    if (is_array($items)) {
      $items = array_slice($items, 0, $this->num_items);
      foreach ($items as $item) {
        $obj = new stdClass();

        // Convert arrays to objects.
        $item = json_decode(json_encode($item), FALSE);

        $twitter_user = $item->user;
        if (variable_get('twitter_pull_retweet_user', TRUE) && !empty($item->retweeted_status)) {
          $twitter_user = $item->retweeted_status->user;
        }
        // when importing lists $user is an array and not an object, secure $user as object.
        $twitter_user = (object) $twitter_user;
        
        $obj->id = check_plain($item->id_str);
        // The name is the user's 'human-readable' name.
        $obj->username = check_plain(!empty($twitter_user->name) ? $twitter_user->name : $item->from_user);
        // The screen_name is the Twitter machine name, i.e., the string that is
        // commonly seen with an '@' prefixed.
        $obj->screenname = check_plain(!empty($twitter_user->screen_name) ? $twitter_user->screen_name : $item->from_user);

        //retrieve the user photo
        $obj->userphoto = check_plain(!empty($twitter_user->profile_image_url) ? $twitter_user->profile_image_url : $item->profile_image_url);
        $obj->userphoto_https = check_plain(!empty($twitter_user->profile_image_url_https) ? $twitter_user->profile_image_url_https : $item->profile_image_url_https);

        $obj->text = filter_xss($item->text);
        //-- Convert date to unix timestamp so themer can easily work with it.
        $obj->timestamp = strtotime($item->created_at);

        $tweets[] = $obj;
      }
    }

    $this->tweets = $tweets;
  }

}
