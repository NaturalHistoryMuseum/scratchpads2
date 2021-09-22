<?php

class DrupalOAuthClient {
  public $version = OAUTH_COMMON_VERSION_1_RFC;

  protected $consumer;
  protected $requestToken;
  protected $accessToken;
  protected $signatureMethod;

  /**
   * Creates an instance of the DrupalOAuthClient.
   *
   * @param DrupalOAuthConsumer $consumer
   *  The consumer.
   * @param OAuthToken $request_token
   *  Optional. A request token to use.
   * @param OAuthSignatureMethod $signature_method
   *  Optional. The signature method to use.
   * @param integer $version
   *  Optional. The version to use - either OAUTH_COMMON_VERSION_1_RFC or OAUTH_COMMON_VERSION_1.
   */
  public function __construct($consumer, $request_token = NULL, $signature_method = NULL, $version = NULL) {
    $this->consumer = $consumer;
    $this->requestToken = $request_token;
    $this->signatureMethod = $signature_method;

    if ($version) {
      $this->version = $version;
    }

    // Set to the default signature method if no method was specified
    if (!$this->signatureMethod) {
      if (!empty($this->consumer->configuration['signature_method'])) {
        $signature_method = substr(strtolower($this->consumer->configuration['signature_method']), 5);
      }
      else {
        $signature_method = 'SHA1';
      }
      $this->signatureMethod = self::signatureMethod($signature_method);
    }
  }

  /**
   * Convenience function to get signing method implementations.
   *
   * @param string $method
   *  Optional. The hmac hashing algorithm to use. Defaults to 'sha512' which
   *  has superseded sha1 as the recommended alternative.
   * @param bool $fallback_to_sha1
   *  Optional. Whether sha1 should be used as a fallback if the selected
   *  hashing algorithm is unavailable.
   * @return OAuthSignatureMethod
   *  The signature method object.
   */
  public static function signatureMethod($method = 'SHA1', $fallback_to_sha1 = TRUE) {
    $sign = NULL;

    if (in_array(drupal_strtolower($method), hash_algos())) {
      $sign = new OAuthSignatureMethod_HMAC($method);
    }
    else if ($fallback_to_sha1) {
      $sign = new OAuthSignatureMethod_HMAC('SHA1');
    }

    return $sign;
  }

  /**
   * Gets a request token from the provider.
   *
   * @param string $endpoint
   *   Optional. The endpoint path for the provider.
   *     - If you provide the full URL (e.g. "http://example.com/oauth/request_token"),
   *       then it will be used.
   *     - If you provide only the path (e.g. "oauth/request_token"), it will
   *       be converted into a full URL by prepending the provider_url.
   *     - If you provide nothing it will default to '/oauth/request_token'.
   * @param array $options
   *   An associative array of additional optional options, with the following keys:
   *     - 'params'
   *       An associative array of parameters that should be included in the
   *       request.
   *     - 'realm'
   *       A string to be used as the http authentication realm in the request.
   *     - 'get' (default FALSE)
   *       Whether to use GET as the HTTP-method instead of POST.
   *     - 'callback'
   *       A full URL of where the user should be sent after the request token
   *       has been authorized.
   *       Only used by versions higher than OAUTH_COMMON_VERSION_1.
   * @return DrupalOAuthToken
   *  The returned request token.
   */
  public function getRequestToken($endpoint = NULL, $options = array()) {
    if ($this->requestToken) {
      return clone $this->requestToken;
    }

    $options += array(
        'params' => array(),
        'realm' => NULL,
        'get' => FALSE,
        'callback' => NULL,
      );

    if (empty($endpoint)) {
      if (!empty($this->consumer->configuration['request_endpoint'])) {
        $endpoint = $this->consumer->configuration['request_endpoint'];
      }
      else {
        $endpoint = '/oauth/request_token';
      }
    }
    if ($this->version > OAUTH_COMMON_VERSION_1) {
      $options['params']['oauth_callback'] = $options['callback'] ? $options['callback'] : 'oob';
    }

    $response = $this->get($endpoint, array(
        'params' => $options['params'],
        'realm' => $options['realm'],
        'get' => $options['get'],
      ));

    $params = array();
    parse_str($response, $params);

    if (empty($params['oauth_token']) || empty($params['oauth_token_secret'])) {
      throw new Exception('No valid request token was returned');
    }

    if ($this->version > OAUTH_COMMON_VERSION_1 && empty($params['oauth_callback_confirmed'])) {
      $this->version = OAUTH_COMMON_VERSION_1;
    }

    $this->requestToken = new DrupalOAuthToken($params['oauth_token'], $params['oauth_token_secret'], $this->consumer, array(
      'type'    => OAUTH_COMMON_TOKEN_TYPE_REQUEST,
      'version' => $this->version,
    ));

    return clone $this->requestToken;
  }

  /**
   * Constructs the url that the user should be sent to to authorize the
   * request token.
   *
   * @param string $endpoint
   *   Optional. The endpoint path for the provider.
   *     - If you provide the full URL (e.g. "http://example.com/oauth/authorize"),
   *       then it will be used.
   *     - If you provide only the path (e.g. "oauth/authorize"), it will
   *       be converted into a full URL by prepending the provider_url.
   *     - If you provide nothing it will default to '/oauth/authorize'.
   * @param array $options
   *   An associative array of additional optional options, with the following keys:
   *     - 'params'
   *       An associative array of parameters that should be included in the
   *       request.
   *     - 'callback'
   *       A full URL of where the user should be sent after the request token
   *       has been authorized.
   *       Only used by version OAUTH_COMMON_VERSION_1.
   * @return string
   *  The url.
   */
  public function getAuthorizationUrl($endpoint = NULL, $options = array()) {
    $options += array(
        'params' => array(),
        'callback' => NULL,
      );

    if (empty($endpoint)) {
      if (!empty($this->consumer->configuration['authorization_endpoint'])) {
        $endpoint = $this->consumer->configuration['authorization_endpoint'];
      }
      else {
        $endpoint = '/oauth/authorize';
      }
    }
    if ($this->version == OAUTH_COMMON_VERSION_1 && $options['callback']) {
      $options['params']['oauth_callback'] = $options['callback'];
    }
    $options['params']['oauth_token'] = $this->requestToken->key;

    $endpoint = $this->getAbsolutePath($endpoint);
    $append_query = strpos($endpoint, '?') === FALSE ? '?' : '&';
    return $endpoint . $append_query . http_build_query($options['params'], NULL, '&');
  }

  /**
   * Fetches the access token using the request token.
   *
   * @param string $endpoint
   *   Optional. The endpoint path for the provider.
   *     - If you provide the full URL (e.g. "http://example.com/oauth/access_token"),
   *       then it will be used.
   *     - If you provide only the path (e.g. "oauth/access_token"), it will
   *       be converted into a full URL by prepending the provider_url.
   *     - If you provide nothing it will default to '/oauth/access_token'.
   * @param array $options
   *   An associative array of additional optional options, with the following keys:
   *     - 'params'
   *       An associative array of parameters that should be included in the
   *       request.
   *     - 'realm'
   *       A string to be used as the http authentication realm in the request.
   *     - 'get' (default FALSE)
   *       Whether to use GET as the HTTP-method instead of POST.
   *     - 'verifier'
   *       A string containing a verifier for he user from the provider.
   *       Only used by versions higher than OAUTH_COMMON_VERSION_1.
   * @return DrupalOAuthToken
   *  The access token.
   */
  public function getAccessToken($endpoint = NULL, $options = array()) {
    if ($this->accessToken) {
      return clone $this->accessToken;
    }

    $options += array(
        'params' => array(),
        'realm' => NULL,
        'get' => FALSE,
        'verifier' => NULL,
      );

    if (empty($endpoint)) {
      if (!empty($this->consumer->configuration['access_endpoint'])) {
        $endpoint = $this->consumer->configuration['access_endpoint'];
      }
      else {
        $endpoint = '/oauth/access_token';
      }
    }
    if ($this->version > OAUTH_COMMON_VERSION_1 && $options['verifier'] !== NULL) {
      $options['params']['oauth_verifier'] = $options['verifier'];
    }

    $response = $this->get($endpoint, array(
        'token' => TRUE,
        'params' => $options['params'],
        'realm' => $options['realm'],
        'get' => $options['get'],
      ));

    $params = array();
    parse_str($response, $params);

    if (empty($params['oauth_token']) || empty($params['oauth_token_secret'])) {
      throw new Exception('No valid access token was returned');
    }

    // Check if we've has recieved this token previously and if so use the old one
    //TODO: Is this safe!? What if eg. multiple users are getting the same access token from the provider?
    $this->accessToken = DrupalOAuthToken::loadByKey($params['oauth_token'], $this->consumer);
    //TODO: Can a secret change even though the token doesn't? If so it needs to be changed.
    if (!$this->accessToken) {
      $this->accessToken = new DrupalOAuthToken($params['oauth_token'], $params['oauth_token_secret'], $this->consumer, array(
        'type' => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
      ));
    }

    return clone $this->accessToken;
  }

  /**
   * Make an OAuth request.
   *
   * @param string $path
   *   The path being requested.
   *     - If you provide the full URL (e.g. "http://example.com/oauth/request_token"),
   *       then it will be used.
   *     - If you provide only the path (e.g. "oauth/request_token"), it will
   *       be converted into a full URL by prepending the provider_url.
   * @param array $options
   *   An associative array of additional options, with the following keys:
   *     - 'token' (default FALSE)
   *       Whether a token should be used or not.
   *     - 'params'
   *       An associative array of parameters that should be included in the
   *       request.
   *     - 'realm'
   *       A string to be used as the http authentication realm in the request.
   *     - 'get' (default FALSE)
   *       Whether to use GET as the HTTP-method instead of POST.
   * @return string
   *   a string containing the response body.
   */
  protected function get($path, $options = array()) {
    $options += array(
        'token' => FALSE,
        'params' => array(),
        'realm' => NULL,
        'get' => FALSE,
      );

    if (empty($options['realm']) && !empty($this->consumer->configuration['authentication_realm'])) {
      $options['realm'] = $this->consumer->configuration['authentication_realm'];
    }

    $token = $options['token'] ? $this->requestToken : NULL;
    $path = $this->getAbsolutePath($path);

    $req = OAuthRequest::from_consumer_and_token($this->consumer, $token,
      $options['get'] ? 'GET' : 'POST', $path, $options['params']);
    $req->sign_request($this->signatureMethod, $this->consumer, $token);

    $url = $req->get_normalized_http_url();
    $params = array();
    foreach ($req->get_parameters() as $param_key => $param_value) {
      if (substr($param_key, 0, 5) != 'oauth') {
        $params[$param_key] = $param_value;
      }
    }
    if (!empty($params)) {
      $url .= '?' . http_build_query($params);
    }

    $headers = array(
      'Accept: application/x-www-form-urlencoded',
      $req->to_header($options['realm']),
    );

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    if (!$options['get']) {
      curl_setopt($ch, CURLOPT_POST, 1);
      curl_setopt($ch, CURLOPT_POSTFIELDS, '');
    }
    $oauth_version = _oauth_common_version();
    curl_setopt($ch, CURLOPT_USERAGENT, 'Drupal/' . VERSION . ' OAuth/' . $oauth_version);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($ch, CURLOPT_HEADER, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    $response = curl_exec($ch);
    $error = curl_error($ch);
    curl_close($ch);

    if ($error) {
      throw new Exception($error);
    }

    $result = $this->interpretResponse($response);
    if ($result->responseCode != 200) {
      throw new Exception('Failed to fetch data from url "' . $path . '" (HTTP response code ' . $result->responseCode . ' ' . $result->responseMessage . '): ' . $result->body, $result->responseCode);
    }

    return $result->body;
  }

  /**
   * Makes sure a path is an absolute path
   *
   * Prepends provider url if the path isn't absolute.
   *
   * @param string $path
   *  The path to make absolute.
   * @return string
   *  The absolute path.
   */
  protected function getAbsolutePath($path) {
    $protocols = array(
      'http',
      'https'
    );
    $protocol = strpos($path, '://');
    $protocol = $protocol ? substr($path, 0, $protocol) : '';
    if (!in_array($protocol, $protocols)) {
      $path = $this->consumer->configuration['provider_url'] . $path;
    }
    return $path;
  }

  protected function interpretResponse($res) {
    list($headers, $body) = preg_split('/\r\n\r\n/', $res, 2);

    $obj = (object) array(
      'headers' => $headers,
      'body' => $body,
    );

    $matches = array();
    if (preg_match('/HTTP\/1.\d (\d{3}) (.*)/', $headers, $matches)) {
      $obj->responseCode = trim($matches[1]);
      $obj->responseMessage = trim($matches[2]);

      // Handle HTTP/1.1 100 Continue
      if ($obj->responseCode == 100) {
        return $this->interpretResponse($body);
      }
    }

    return $obj;
  }
}
