<?php
/**
 * @file
 * Integration layer to communicate with the Twitter REST API 1.1.
 * https://dev.twitter.com/docs/api/1.1
 *
 * Original work my James Walker (@walkah).
 * Upgraded to 1.1 by Juampy (@juampy72).
 */

/**
 * Exception handling class.
 */
class TwitterException extends Exception {}

/**
 * Primary Twitter API implementation class
 */
class Twitter {
  /**
   * @var $source the twitter api 'source'
   */
  protected $source = 'drupal';

  protected $signature_method;

  protected $consumer;

  protected $token;


  /********************************************//**
   * Authentication
   ***********************************************/
  /**
   * Constructor for the Twitter class
   */
  public function __construct($consumer_key, $consumer_secret, $oauth_token = NULL,
                              $oauth_token_secret = NULL) {
    $this->signature_method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
    if (!empty($oauth_token) && !empty($oauth_token_secret)) {
      $this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
    }
  }

  public function get_request_token($params = array()) {
    $oauth_callback = variable_get('twitter_oauth_callback_url', TWITTER_OAUTH_CALLBACK_URL);
    $url = variable_get('twitter_api', TWITTER_API) . '/oauth/request_token';
    try {
      $params = array_merge($params, array('oauth_callback' => url($oauth_callback, array('absolute' => TRUE))));
      $response = $this->auth_request($url, $params);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  public function get_authorize_url($token) {
    $url = variable_get('twitter_api', TWITTER_API) . '/oauth/authorize';
    $url.= '?oauth_token=' . $token['oauth_token'];

    return $url;
  }

  public function get_authenticate_url($token) {
    $url = variable_get('twitter_api', TWITTER_API) . '/oauth/authenticate';
    $url.= '?oauth_token=' . $token['oauth_token'];

    return $url;
  }

  /**
   * Request an access token to the Twitter API.
   * @see https://dev.twitter.com/docs/auth/implementing-sign-twitter
   *
   * @param string$oauth_verifier
   *   String an access token to append to the request or NULL.
   * @return
   *   String the access token or FALSE when there was an error.
   */
  public function get_access_token($oauth_verifier = NULL) {
    $url = variable_get('twitter_api', TWITTER_API) . '/oauth/access_token';

    // Adding parameter oauth_verifier to auth_request
    $parameters = array();
    if (!empty($oauth_verifier)) {
      $parameters['oauth_verifier'] = $oauth_verifier;
    }

    try {
      $response = $this->auth_request($url, $parameters);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  /**
   * Performs an authenticated request.
   */
  public function auth_request($url, $params = array(), $method = 'GET') {
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $params);
    $request->sign_request($this->signature_method, $this->consumer, $this->token);

    try {
      switch ($method) {
        case 'GET':
          return $this->request($request->to_url());
        case 'POST':
          return $this->request($request->get_normalized_http_url(), $request->get_parameters(), 'POST');
      }
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Performs a request.
   *
   * @throws TwitterException
   */
  protected function request($url, $params = array(), $method = 'GET') {
    $data = '';
    if (count($params) > 0) {
      if ($method == 'GET') {
        $url .= '?'. http_build_query($params, '', '&');
      }
      else {
        $data = http_build_query($params, '', '&');
      }
    }

    $headers = array();

    $headers['Authorization'] = 'Oauth';
    $headers['Content-type'] = 'application/x-www-form-urlencoded';

    $response = $this->doRequest($url, $headers, $method, $data);
    if (!isset($response->error)) {
      return $response->data;
    }
    else {
      // Extract response error.
      $error = $response->error;
      // See if there is an error message in the response's data.
      // This will be an error message from the Twitter API.
      if (isset($response->data)) {
        $data = $this->parse_response($response->data);
        if (isset($data['error'])) {
          $error .= "\n" . $data['error'];
        }
      }
      throw new TwitterException($error);
    }
  }
  
  /**
   *
   * @see https://dev.twitter.com/docs/api/1/post/statuses/retweet/%3Aid
   */
  public function retweet($tweet_id, $params = array()) {
    $params = array();
    $values = $this->call('statuses/retweet/' . $tweet_id, $params, 'POST', TRUE);
    return new TwitterStatus($values);
  }

  /**
   * Actually performs a request.
   *
   * This method can be easily overriden through inheritance.
   *
   * @param string $url
   *   The url of the endpoint.
   * @param array $headers
   *   Array of headers.
   * @param string $method
   *   The HTTP method to use (normally POST or GET).
   * @param array $data
   *   An array of parameters
   * @return
   *   stdClass response object.
   */
  protected function doRequest($url, $headers, $method, $data) {
    return drupal_http_request($url, array('headers' => $headers, 'method' => $method, 'data' => $data));
  }

  /**
   * @see https://www.drupal.org/node/985544
   */
  protected function parse_response($response) {
    $length = strlen(PHP_INT_MAX);
    $response = preg_replace('/"(id|in_reply_to_status_id|in_reply_to_user_id)":(\d{' . $length . ',})/', '"\1":"\2"', $response);
    return json_decode($response, TRUE);
  }

  /**
   * Creates an API endpoint URL.
   *
   * @param string $path
   *   The path of the endpoint.
   * @param string $format
   *   The format of the endpoint to be appended at the end of the path.
   * @return
   *   The complete path to the endpoint.
   */
  protected function create_url($path, $format = '.json') {
    $url =  variable_get('twitter_api', TWITTER_API) .'/1.1/'. $path . $format;
    return $url;
  }

  /********************************************//**
   * Helpers used to convert responses in objects
   ***********************************************/

  /**
   * Get an array of TwitterStatus objects from an API endpoint.
   */
  protected function get_statuses($path, $params = array()) {
    $values = $this->call($path, $params, 'GET');
    // Check on successfull call.
    if ($values) {
      $statuses = array();
      foreach ($values as $status) {
        $statuses[] = new TwitterStatus($status);
      }
      return $statuses;
    }
    // Call might return FALSE, e.g. on failed authentication.
    else {
      // As call already throws an exception, we can return an empty array to
      // break no code.
      return array();
    }
  }

  /**
   * Get an array of TwitterUser objects from an API endpoint
   */
  protected function get_users($path, $params = array()) {
    $values = $this->call($path, $params, 'GET');
    // Check on successfull call
    if ($values) {
      $users = array();
      foreach ($values as $user) {
        $users[] = new TwitterUser($user);
      }
      return $users;
    }
    // Call might return FALSE , e.g. on failed authentication
    else {
      // As call allready throws an exception, we can return an empty array to
      // break no code.
      return array();
    }
  }

  /********************************************//**
   * Timelines
   ***********************************************/
  /**
   * Returns the 20 most recent mentions (tweets containing a users's @screen_name).
   *
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/mentions_timeline
   */
  public function mentions_timeline($params = array()) {
    return $this->get_statuses('statuses/mentions_timeline', $params);
  }

  /**
   * Fetch a user's timeline
   *
   * Returns a collection of the most recent Tweets posted by the user indicated
   * by the screen_name or user_id parameters.
   *
   * @param mixed $id
   *   either a Twitter user_id or a Twitter screen_name.
   *
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/user_timeline
   */
  public function user_timeline($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->get_statuses('statuses/user_timeline', $params);
  }

  /**
   * Returns a collection of the most recent Tweets and retweets posted by
   * the authenticating user and the users they follow.
   *
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/home_timeline
   */
  public function home_timeline($params = array()) {
    return $this->get_statuses('statuses/home_timeline', $params);
  }

  /**
   * Returns the most recent tweets authored by the authenticating user
   * that have recently been retweeted by others.
   *
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/retweets_of_me
   */
  public function retweets_of_me($params = array()) {
    return $this->get_statuses('statuses/retweets_of_me', $params);
  }

  /********************************************//**
   * Tweets
   ***********************************************/
  /**
   * Returns up to 100 of the first retweets of a given tweet.
   *
   * @param int $id
   *   The numerical ID of the desired status.
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/retweets
   */
  public function statuses_retweets($id, $params = array()) {
    return $this->get_statuses('statuses/retweets/' . $id, $params);
  }

  /**
   * Destroys the status specified by the required ID parameter.
   *
   * @param array $params
   *   an array of parameters.
   *
   * @return
   *   TwitterStatus object if successful or FALSE.
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/destroy
   */
  public function statuses_destroy($id, $params = array()) {
    $values = $this->call('statuses/update', $params, 'POST');
    if ($values) {
      return new TwitterStatus($values);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Updates the authenticating user's current status, also known as tweeting.
   *
   * @param string $status
   *   The text of the status update (the tweet).
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/statuses/update
   */
  public function statuses_update($status, $params = array()) {
    $params['status'] = $status;
    $values = $this->call('statuses/update', $params, 'POST');
    return new TwitterStatus($values);
  }

  /**
   * Retweets a tweet. Returns the original tweet with retweet details embedded.
   *
   * @param int $id
   *   The numerical ID of the desired status.
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/statuses/retweet/%3Aid
   */
  public function statuses_retweet($id, $params = array()) {
    $values = $this->call('statuses/retweet/' . $id, $params, 'POST');
    return new TwitterStatus($values);
  }

  /**
   * Creates a Tweet with a picture attached.
   *
   * @param string $status
   *   The text of the status update (the tweet).
   * @param array $media
   *   An array of physical paths of images.
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/statuses/update_with_media
   */
  public function statuses_update_with_media($status, $media, $params = array()) {
    $params['status'] = $status;
    $params['media[]'] = '@{' . implode(',', $media) . '}';
    $values = $this->call('statuses/statuses/update_with_media', $params, 'POST');
    // @TODO support media at TwitterStatus class.
    return new TwitterStatus($values);
  }

  /**
   * Returns information allowing the creation of an embedded representation of
   * a Tweet on third party sites.
   *
   * @param mixed $id
   *   The Tweet/status ID or the URL of the Tweet/status to be embedded.
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/oembed
   */
  public function statuses_oembed($id, $params = array()) {
    if (is_numeric($id)) {
      $params['id'] = $id;
    }
    else {
      $params['url'] = $id;
    }
    return $this->call('statuses/oembed', $params, 'GET');
  }

  /********************************************//**
   * Search
   ***********************************************/
  /**
   * Returns a collection of relevant Tweets matching a specified query.
   *
   * @param string $query
   *   A UTF-8, URL-encoded search query of 1,000 characters maximum,
   *   including operators.
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/search/tweets
   */
  public function search_tweets($query, $params = array()) {
    $params['q'] = $query;
    return $this->get_statuses('statuses/oembed', $params);
  }

  /********************************************//**
   * Streaming
   ***********************************************/
  /**
   * Returns public statuses that match one or more filter predicates.
   *
   * At least one predicate parameter (follow, locations, or track) must be specified.
   *
   * @param string $follow
   *   A comma separated list of user IDs.
   * @param string $track
   *   Keywords to track.
   * @param string $locations
   *   Specifies a set of bounding boxes to track.
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/statuses/filter
   */
  public function statuses_filter($follow = '', $track = '', $locations = '', $params = array()) {
    if (!empty($follow)) {
      $params['follow'] = $follow;
    }
    if (!empty($track)) {
      $params['track'] = $track;
    }
    if (!empty($locations)) {
      $params['locations'] = $locations;
    }
    return $this->call('statuses/filter', $params, 'POST');
  }

  /**
   * Returns a small random sample of all public statuses.
   *
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/sample
   */
  public function statuses_sample($params = array()) {
    return $this->get_statuses('statuses/sample', $params);
  }

  /**
   * Returns all public statuses. Few applications require this level of access.
   *
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/statuses/firehose
   */
  public function statuses_firehose($params = array()) {
    return $this->get_statuses('statuses/firehose', $params);
  }

  /**
   * Streams messages for a single user.
   *
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/user
   */
  public function user($params = array()) {
    return $this->get_statuses('user', $params);
  }

  /**
   * Streams messages for a set of users.
   *
   * @param string $follow
   *   A comma separated list of user IDs
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/site
   */
  public function site($follow, $params = array()) {
    $params['follow'] = $follow;
    return $this->get_statuses('site', $params);
  }

  /********************************************//**
   * Direct Messages
   ***********************************************/
  /**
   * Returns the 20 most recent direct messages sent to the authenticating user.
   *
   * This method requires an access token with RWD (read, write & direct message)
   * permissions
   *
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of Twitter statuses.
   * @see https://dev.twitter.com/docs/api/1.1/get/direct_messages
   */
  public function direct_messages($params = array()) {
    return $this->get_statuses('direct_messages', $params);
  }

  /**
   * Returns the 20 most recent direct messages sent by the authenticating user.
   *
   * This method requires an access token with RWD (read, write & direct message)
   * permissions
   *
   * @param array $params
   *   An array of parameters.
   * @return
   *   Array of Twitter statuses.
   * @see https://dev.twitter.com/docs/api/1.1/get/direct_messages/sent
   */
  public function direct_messages_sent($params = array()) {
    return $this->get_statuses('direct_messages/sent', $params);
  }

  /**
   * Returns a single direct message, specified by an id parameter.
   *
   * This method requires an access token with RWD (read, write & direct message)
   * permissions
   *
   * @param int $id
   *   The ID of the direct message.
   * @return
   *   array of Twitter statuses.
   * @see https://dev.twitter.com/docs/api/1.1/get/direct_messages/show
   */
  public function direct_messages_show($id) {
    $params = array('id' => $id);
    return $this->get_statuses('direct_messages/show', $params);
  }

  /**
   * Destroys the direct message specified in the required ID parameter.
   *
   * This method requires an access token with RWD (read, write & direct message)
   * permissions
   *
   * @param int $id
   *   The ID of the direct message.
   * @param array $params
   *   An array of parameters.
   * @return
   *   The deleted direct message
   * @see https://dev.twitter.com/docs/api/1.1/post/direct_messages/destroy
   */
  public function direct_messages_destroy($id, $params = array()) {
    $params['id'] = $id;
    return $this->get_statuses('direct_messages/destroy', $params);
  }

  /**
   * Sends a new direct message to the specified user from the authenticating user.
   *
   * @param mixed $id
   *   The user ID or the screen name.
   * @param string $text
   *   The URL encoded text of the message.
   * @return
   *   array of Twitter statuses.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/direct_messages/new
   */
  public function direct_messages_new($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->call('direct_messages/new', $params, 'POST');
  }

  /********************************************//**
   * Friends & Followers
   ***********************************************/
  /**
   * Returns a cursored collection of user IDs for every user the specified user
   * is following.
   *
   * @param mixed $id
   *   The user ID or the screen name.
   * @return
   *   An array of user IDS.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/friends/ids
   */
  public function friends_ids($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->call('friends/ids', $params, 'GET');
  }

  /**
   * Returns a cursored collection of user IDs for every user following the
   * specified user.
   *
   * @param mixed $id
   *   The user ID or the screen name.
   * @return
   *   An array of user IDS.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/followers/ids
   */
  public function followers_ids($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->call('followers/ids', $params, 'GET');
  }

  /**
   * Returns the relationships of the authenticating user to the
   * comma-separated list of up to 100 screen_names or user_ids provided.
   *
   * @param string $screen_name
   *   A comma separated list of screen names.
   * @param string $user_id
   *   A comma separated list of user IDs.
   * @return
   *   An array of user IDs and relationships.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/friendships/lookup
   */
  public function friendships_lookup($screen_name = '', $user_id = '') {
    if (!empty($screen_name)) {
      $params['screen_name'] = $screen_name;
    }
    if (!empty($user_id)) {
      $params['user_id'] = $user_id;
    }
    return $this->call('friendships/lookup', $params, 'GET');
  }

  /**
   * Returns a collection of numeric IDs for every user who has a pending
   * request to follow the authenticating user.
   *
   * @param array $params
   *   An array of parameters.
   * @return
   *   An array of numeric user IDs.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/friendships/incoming
   */
  public function friendships_incoming($params = array()) {
    return $this->call('friendships/incoming', $params, 'GET');
  }

  /**
   * Returns a collection of numeric IDs for every protected user for whom
   * the authenticating user has a pending follow request.
   *
   * @param array $params
   *   An array of parameters.
   * @return
   *   An array of numeric user IDs.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/friendships/outgoing
   */
  public function friendships_outgoing($params = array()) {
    return $this->call('friendships/outgoing', $params, 'GET');
  }

  /**
   * Allows the authenticating users to follow the user specified in the
   * ID parameter.
   *
   * @param mixed $id
   *   The user ID or the screen name.
   * @param bool $follow
   *   Wether to enable notifications for the target user.
   * @return
   *   The befriended user in the requested format when successful, or a
   *   string describing the failure condition when unsuccessful.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/friendships/create
   */
  public function friendships_create($id, $follow = NULL) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    if ($follow !== NULL) {
      $params['follow'] = $id;
    }
    return $this->call('friendships/create', $params, 'POST');
  }

  /**
   * Allows the authenticating user to unfollow the user specified in the
   * ID parameter.
   *
   * @param mixed $id
   *   The user ID or the screen name.
   * @return
   *   The unfollowed user in the requested format when successful, or a
   *   string describing the failure condition when unsuccessful.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/friendships/destroy
   */
  public function friendships_destroy($id) {
    $params = array();
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->call('friendships/destroy', $params, 'POST');
  }

  /**
   * Allows one to enable or disable retweets and device notifications
   * from the specified user.
   *
   * @param mixed $id
   *   The user ID or the screen name.
   * @param bool $device
   *   Whether to enable/disable device notifications from the target user.
   * @param bool $retweets
   *   Whether to enable/disable retweets from the target user.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/friendships/update
   */
  public function friendships_update($id, $device = NULL, $retweets = NULL) {
    $params = array();
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    if ($device !== NULL) {
      $params['device'] = $device;
    }
    if ($retweets!== NULL) {
      $params['retweets'] = $retweets;
    }
    return $this->call('friendships/update', $params, 'POST');
  }

  /**
   * Returns detailed information about the relationship between two arbitrary
   * users.
   *
   * @param mixed $source_id
   *   The user ID or the screen name of the subject user.
   * @param mixed $target_id
   *   The user ID or the screen name of the target user.
   * @return
   *   An array of numeric user IDs.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/friendships/show
   */
  public function friendships_show($source_id, $target_id) {
    if (is_numeric($source_id)) {
      $params['source_id'] = $source_id;
    }
    else {
      $params['source_screen_name'] = $source_id;
    }
    if (is_numeric($target_id)) {
      $params['target_id'] = $target_id;
    }
    else {
      $params['target_screen_name'] = $target_id;
    }
    return $this->call('friendships/show', $params, 'GET');
  }

  /********************************************//**
   * Users
   ***********************************************/
  /**
   * Returns settings (including current trend, geo and sleep time
   * information) for the authenticating user.
   *
   * @return
   *   An array of settings.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/account/settings
   */
  public function account_settings() {
    return $this->call('account/settings', $params, 'GET');
  }

  /**
   * Returns an HTTP 200 OK response code and a representation of the
   * requesting user if authentication was successful; returns a 401
   * status code and an error message if not.
   *
   * @param array $params
   *   An array of parameters.
   * @return
   *   A TwitterUser object or FALSE.
   * @see https://dev.twitter.com/docs/api/1.1/get/account/verify_credentials
   */
  public function verify_credentials($params = array()) {
    $values = $this->call('account/verify_credentials', $params, 'GET');
    if (!$values) {
      return FALSE;
    }
    return new TwitterUser($values);
  }

  /**
   * Updates the authenticating user's settings.
   *
   * @param array $params
   *   An array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/settings
   */
  public function account_settings_update($params = array()) {
    return $this->call('account/settings', $params, 'POST');
  }

  /**
   * Sets which device Twitter delivers updates to for the authenticating user.
   *
   * @param string $device
   *   A string which must be one of: sms, none.
   * @param bool $include_entities
   *   Whether tweets should include entities or not.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/update_delivery_device
   */
  public function account_update_delivery_device($device, $include_entities = NULL) {
    $params = array('device' => $device);
    if ($include_entities !== NULL) {
      $params['include_entities'] = $include_entities;
    }
    return $this->call('account/settings', $params, 'POST');
  }

  /**
   * Sets values that users are able to set under the "Account" tab of their
   * settings page.
   *
   * @param array $params
   *   An array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/update_profile
   */
  public function account_update_profile($params = array()) {
    return $this->call('account/update_profile', $params, 'POST');
  }

  /**
   * Updates the authenticating user's profile background image.
   *
   * This method can also be used to enable or disable the profile
   * background image.
   * At least one of image, tile or use must be provided when making this
   * request.
   *
   * @param string $image
   *   A base64-encoded. Must be a valid GIF, JPG, or PNG image of less
   *   than 800 kilobytes in size.
   * @param bool $tile
   *   Whether or not to tile the background image.
   * @param bool $use
   *   Whether or not to use the background image.
   * @param array $params
   *   An array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/update_profile_background_image
   */
  public function account_update_profile_background_image($image = NULL, $tile = NULL,
                                                          $use = NULL, $params = array()) {
    if ($image !== NULL) {
      $params['image'] = $image;
    }
    if ($tile !== NULL) {
      $params['tile'] = $tile;
    }
    if ($use !== NULL) {
      $params['use'] = $use;
    }
    return $this->call('account/update_profile_background_image', $params, 'POST');
  }

  /**
   * Sets one or more hex values that control the color scheme of the
   * authenticating user's profile page on twitter.com.
   *
   * @param array $params
   *   An array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/update_profile_colors
   */
  public function account_update_profile_colors($params = array()) {
    return $this->call('account_update_profile_colors', $params, 'POST');
  }

  /**
   * Updates the authenticating user's profile image.
   *
   * @param string $image
   *   The avatar image for the profile, base64-encoded. Must be a valid
   *   GIF, JPG, or PNG
   * @param array $params
   *   An array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/update_profile_image
   */
  public function account_update_profile_image($image, $params = array()) {
    $params['image'] = $image;
    return $this->call('account_update_profile_image', $params, 'POST');
  }

  /**
   * Returns a collection of user objects that the authenticating user is
   * blocking.
   *
   * @param array $params
   *   An array of parameters.
   * @return
   *   A TwitterUser object or FALSE.
   * @see https://dev.twitter.com/docs/api/1.1/get/blocks/list
   */
  public function blocks_list($params = array()) {
    $values = $this->call('blocks/list', $params, 'GET');
    if (!$values) {
      return FALSE;
    }
    return new TwitterUser($values);
  }

  /**
   * Returns an array of numeric user ids the authenticating user is blocking.
   *
   * @param array $params
   *   An array of parameters.
   * @return
   *   A TwitterUser object or FALSE.
   * @see https://dev.twitter.com/docs/api/1.1/get/blocks/ids
   */
  public function blocks_ids($params = array()) {
    return $this->call('blocks/ids', $params, 'GET');
  }

  /**
   * Blocks the specified user from following the authenticating user.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @param array $params
   *   An array of parameters.
   * @see https://dev.twitter.com/docs/api/1.1/post/blocks/create
   */
  public function blocks_create($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    $params['image'] = $image;
    return $this->call('blocks/create', $params, 'POST');
  }

  /**
   * Un-blocks the user specified in the ID parameter for the authenticating
   * user.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @param array $params
   *   An array of parameters.
   * @see https://dev.twitter.com/docs/api/1.1/post/blocks/destroy
   */
  public function blocks_destroy($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    $params['image'] = $image;
    return $this->call('blocks/destroy', $params, 'POST');
  }

  /**
   * Returns fully-hydrated user objects for up to 100 users per request,
   * as specified by comma-separated values passed to the user_id and/or
   * screen_name parameters.
   *
   * @param string $screen_name
   *   A comma separated list of screen names.
   * @param string user_id
   *   A comma separated list of user IDs.
   * @param bool $include_entities
   *   Whether to include entities or not.
   * @see https://dev.twitter.com/docs/api/1.1/get/users/lookup
   */
  protected function users_lookup($screen_name = NULL, $user_id = NULL,
                                  $include_entities = NULL) {
    if ($screen_name !== NULL) {
      $params['screen_name'] = $screen_name;
    }
    if ($user_id !== NULL) {
      $params['user_id'] = $user_id;
    }
    if ($include_entities !== NULL) {
      $params['include_entities'] = $include_entities;
    }
    return $this->get_users('users/lookup', $params);
  }

  /**
   * Returns a variety of information about the user specified by the required
   * screen_name parameter.
   *
   * @param string $screen_name
   *   The screen name of a Twitter user.
   * @param bool $include_entities
   *   Whether to include entities or not.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/users/show
   */
  public function users_show($screen_name, $include_entities = NULL) {
    $params = array();
    $params['screen_name'] = $screen_name;
    if ($include_entities !== NULL) {
      $params['include_entities'] = $include_entities;
    }
    $values = $this->call('users/show', $params, 'GET');
    return new TwitterUser($values);
  }

  /**
   * Provides a simple, relevance-based search interface to public user
   * accounts on Twitter.
   *
   * @param string $query
   *   The search query to run against people search.
   * @param array $params
   *   an array of parameters.
   * @return
   *   array of TwitterUser objects.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/users/search
   */
  public function users_search($query, $params = array()) {
    $params['q'] = $query;
    return $this->get_users('users/search', $params);
  }

  /**
   * Returns a collection of users that the specified user can "contribute" to.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @param array $params
   *   an array of parameters.
   * @see https://dev.twitter.com/docs/api/1.1/get/users/contributees
   */
  public function users_contributees($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->get_users('users/contributees', $params);
  }

  /**
   * Returns a collection of users who can contribute to the specified account.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @param array $params
   *   an array of parameters.
   * @see https://dev.twitter.com/docs/api/1.1/get/users/contributors
   */
  public function users_contributors($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->get_users('users/contributors', $params);
  }

  /**
   * Removes the uploaded profile banner for the authenticating user.
   *
   * @see https://dev.twitter.com/docs/api/1.1/post/account/remove_profile_banner
   */
  public function account_remove_profile_banner() {
    return $this->call('account/remove_profile_banner', array(), 'POST');
  }

  /**
   * Uploads a profile banner on behalf of the authenticating user.
   *
   * @param string $banner
   *   The Base64-encoded or raw image data being uploaded as the user's new
   *   profile banner.
   * @param array $params
   *   An array of parameters.
   * @see https://dev.twitter.com/docs/api/1.1/post/account/update_profile_banner
   */
  public function account_update_profile_banner($banner, $params = array()) {
    $params['banner'] = $banner;
    return $this->call('account/update_profile_banner', $params, 'POST');
  }

  /**
   * Returns a map of the available size variations of the specified user's
   * profile banner.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @see https://dev.twitter.com/docs/api/1.1/get/users/profile_banner
   */
  public function account_profile_banner($id) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->call('account/profile_banner', $params, 'GET');
  }

  /********************************************//**
   * Favorites
   ***********************************************/
  /**
   * Returns the 20 most recent favorited tweets for a user.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @param array $params
   *   an array of parameters.
   *
   * @see https://dev.twitter.com/docs/api/1.1/get/favorites/list
   */
  public function favorites_list($id, $params = array()) {
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    return $this->get_statuses('favorites/list', $params);
  }

  /********************************************//**
   * Utilities
   ***********************************************/
  /**
   * Calls a Twitter API endpoint.
   */
  public function call($path, $params = array(), $method = 'GET') {
    $url = $this->create_url($path);

    try {
      $response = $this->auth_request($url, $params, $method);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      throw $e;
    }

    if (!$response) {
      return FALSE;
    }

    return $this->parse_response($response);
  }
}

/**
 * Class for containing an individual twitter status.
 */
class TwitterStatus {
  /**
   * @var created_at
   */
  public $created_at;

  public $id;

  public $text;

  public $source;

  public $truncated;

  public $favorited;

  public $in_reply_to_status_id;

  public $in_reply_to_user_id;

  public $in_reply_to_screen_name;

  public $user;

  public $entities;

  public $retweeted_status;

  /**
   * Constructor for TwitterStatus
   */
  public function __construct($values = array()) {
    $this->created_at = $values['created_at'];
    $this->id = $values['id'];
    $this->text = $values['text'];
    $this->source = $values['source'];
    $this->truncated = $values['truncated'];
    $this->favorited = $values['favorited'];
    $this->in_reply_to_status_id = $values['in_reply_to_status_id'];
    $this->in_reply_to_user_id = $values['in_reply_to_user_id'];
    $this->in_reply_to_screen_name = $values['in_reply_to_screen_name'];

    // This is not passed in for the first tweet added while attaching a new
    // account to the system.
    if (!empty($values['entities'])) {
      $this->entities = $values['entities'];
    }

    if (isset($values['user'])) {
      $this->user = new TwitterUser($values['user']);
    }

    // Load full retweeted_status (original tweet) if retweet detected.
    if (isset($values['retweeted_status'])) {
      $this->retweeted_status = new TwitterStatus($values['retweeted_status']);
    }
  }

  /**
   * Returns the status URL at Twitter.com
   *
   * @return
   *   String URL or FALSE if no user object is present.
   */
  public function getURL() {
    if (empty($this->user->screen_name)) {
      return FALSE;
    }
    return TWITTER_HOST . '/' . $this->user->screen_name . '/status/' . $this->id;
  }
}

class TwitterUser {

  public $id;

  public $screen_name;

  public $name;

  public $location;

  public $description;

  public $followers_count;

  public $friends_count;

  public $statuses_count;

  public $favourites_count;

  public $url;

  public $protected;

  public $profile_image_url;

  public $profile_background_color;

  public $profile_text_color;

  public $profile_link_color;

  public $profile_sidebar_fill_color;

  public $profile_sidebar_border_color;

  public $profile_background_image_url;

  public $profile_background_tile;

  public $verified;

  public $created_at;

  public $created_time;

  public $utc_offset;

  public $status;

  protected $oauth_token;

  protected $oauth_token_secret;

  public function __construct($values = array()) {
    $this->id = $values['id'];
    $this->screen_name = $values['screen_name'];
    $this->name = $values['name'];
    $this->location = $values['location'];
    $this->description = $values['description'];
    $this->url = $values['url'];
    $this->followers_count = $values['followers_count'];
    $this->friends_count = $values['friends_count'];
    $this->statuses_count = $values['statuses_count'];
    $this->favourites_count = $values['favourites_count'];
    $this->protected = $values['protected'];
    $this->profile_image_url = $values['profile_image_url'];
    $this->profile_background_color = $values['profile_background_color'];
    $this->profile_text_color = $values['profile_text_color'];
    $this->profile_link_color = $values['profile_link_color'];
    $this->profile_sidebar_fill_color = $values['profile_sidebar_fill_color'];
    $this->profile_sidebar_border_color = $values['profile_sidebar_border_color'];
    $this->profile_background_image_url = $values['profile_background_image_url'];
    $this->profile_background_tile = $values['profile_background_tile'];
    $this->verified = $values['verified'];
    $this->created_at = $values['created_at'];
    if (!empty($values['uid'])) {
      $this->uid = $values['uid'];
    }
    if (!empty($values['created_at']) && $created_time = strtotime($values['created_at'])) {
      $this->created_time = $created_time;
    }
    $this->utc_offset = $values['utc_offset']?$values['utc_offset']:0;

    if (isset($values['status'])) {
      $this->status = new TwitterStatus($values['status']);
    }
  }

  /**
   * Returns an array with the authentication tokens.
   *
   * @return
   *   array with the oauth token key and secret.
   */
  public function get_auth() {
    return array('oauth_token' => $this->oauth_token, 'oauth_token_secret' => $this->oauth_token_secret);
  }

  /**
   * Sets the authentication tokens to a user.
   *
   * @param array $values
   *   Array with 'oauth_token' and 'oauth_token_secret' keys.
   */
  public function set_auth($values) {
    $this->oauth_token = isset($values['oauth_token'])?$values['oauth_token']:NULL;
    $this->oauth_token_secret = isset($values['oauth_token_secret'])?$values['oauth_token_secret']:NULL;
  }

  /**
   * Checks whether the account is authenticated or not.
   *
   * @return
   *   boolean TRUE when the account is authenticated.
   */
  public function is_auth() {
    return !empty($this->oauth_token) && !empty($this->oauth_token_secret);
  }
}
