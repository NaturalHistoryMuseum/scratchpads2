<?php

/**
 * @file
 * Download via HTTP.
 *
 * Support caching, HTTP Basic Authentication, detection of RSS/Atom feeds,
 * redirects.
 */

/**
 * Error code for when the URL could not be parsed.
 *
 * @var int
 */
define('FEEDS_ERROR_PARSE_ERROR', -1001);

/**
 * Error code for when the scheme of an URL could not be determined.
 *
 * An example of a scheme is 'http'.
 *
 * @var int
 */
define('FEEDS_ERROR_NO_SCHEME', -1002);

/**
 * Error code for when the scheme of an URL is not supported.
 *
 * @var int
 */
define('FEEDS_ERROR_INVALID_SCHEME', -1003);

/**
 * PCRE for finding the link tags in html.
 */
define('HTTP_REQUEST_PCRE_LINK_TAG', '/<link((?:[\x09\x0A\x0B\x0C\x0D\x20]+[^\x09\x0A\x0B\x0C\x0D\x20\x2F\x3E][^\x09\x0A\x0B\x0C\x0D\x20\x2F\x3D\x3E]*(?:[\x09\x0A\x0B\x0C\x0D\x20]*=[\x09\x0A\x0B\x0C\x0D\x20]*(?:"(?:[^"]*)"|\'(?:[^\']*)\'|(?:[^\x09\x0A\x0B\x0C\x0D\x20\x22\x27\x3E][^\x09\x0A\x0B\x0C\x0D\x20\x3E]*)?))?)*)[\x09\x0A\x0B\x0C\x0D\x20]*(>(.*)<\/link>|(\/)?>)/si');

/**
 * PCRE for matching all the attributes in a tag.
 */
define('HTTP_REQUEST_PCRE_TAG_ATTRIBUTES', '/[\x09\x0A\x0B\x0C\x0D\x20]+([^\x09\x0A\x0B\x0C\x0D\x20\x2F\x3E][^\x09\x0A\x0B\x0C\x0D\x20\x2F\x3D\x3E]*)(?:[\x09\x0A\x0B\x0C\x0D\x20]*=[\x09\x0A\x0B\x0C\x0D\x20]*(?:"([^"]*)"|\'([^\']*)\'|([^\x09\x0A\x0B\x0C\x0D\x20\x22\x27\x3E][^\x09\x0A\x0B\x0C\x0D\x20\x3E]*)?))?/');

/**
 * For cUrl specific errors.
 */
class HRCurlException extends Exception {}

/**
 * For HTTP requests that do not return a 2xx code.
 */
class FeedsHTTPRequestException extends Exception {}

/**
 * Discovers RSS or atom feeds at the given URL.
 *
 * If document in given URL is an HTML document, function attempts to discover
 * RSS or Atom feeds.
 *
 * @param string $url
 *   The url of the feed to retrieve.
 * @param array $options
 *   An optional array of options.
 *   For valid options, see feeds_http_request().
 *
 * @return bool|string
 *   The discovered feed, or FALSE if the URL is not reachable or there was an
 *   error.
 */
function http_request_get_common_syndication($url, $options = array()) {
  $download = feeds_http_request($url, $options);

  // Cannot get the feed, return.
  // feeds_http_request() always returns 200 even if its 304.
  if ($download->code != 200) {
    return FALSE;
  }

  // Drop the data into a separate variable so all manipulations of the html
  // will not effect the actual object that exists in the static cache.
  // @see feeds_http_request()
  $downloaded_string = $download->data;
  // If this happens to be a feed then just return the url.
  if (isset($download->headers['content-type']) && http_request_is_feed($download->headers['content-type'], $downloaded_string)) {
    return $url;
  }

  $discovered_feeds = http_request_find_feeds($downloaded_string);
  foreach ($discovered_feeds as $feed_url) {
    $absolute = http_request_create_absolute_url($feed_url, $url);
    if (!empty($absolute)) {
      // @TODO: something more intelligent?
      return $absolute;
    }
  }
}

/**
 * Get the content from the given URL.
 *
 * @param string $url
 *   A valid URL (not only web URLs).
 * @param string $username
 *   If the URL uses authentication, supply the username.
 * @param string $password
 *   If the URL uses authentication, supply the password.
 * @param bool $accept_invalid_cert
 *   Whether to accept invalid certificates.
 * @param int $timeout
 *   Timeout in seconds to wait for an HTTP get request to finish.
 *
 * @return object
 *   An object that describes the data downloaded from $url.
 */
function http_request_get($url, $username = NULL, $password = NULL, $accept_invalid_cert = FALSE, $timeout = NULL) {
  return feeds_http_request($url, array(
    'username' => $username,
    'password' => $password,
    'accept_invalid_cert' => $accept_invalid_cert,
    'timeout' => $timeout,
  ));
}

/**
 * Get the content from the given URL.
 *
 * @param string $url
 *   A valid URL (not only web URLs).
 * @param array $options
 *   (optional) An array that can have one or more of the following elements:
 *   - username: (string) If the URL uses authentication, supply the username.
 *   - password: (string) If the URL uses authentication, supply the password.
 *   - accept_invalid_cert: (bool) Whether to accept invalid certificates.
 *     Defaults to FALSE.
 *   - timeout: (integer) Timeout in seconds to wait for an HTTP get request to
 *     finish. Defaults to 30 seconds.
 *   - cache_http_result: (bool) Whether to cache the HTTP result. Defaults to
 *     TRUE.
 *
 * @return object
 *   An object that describes the data downloaded from $url.
 */
function feeds_http_request($url, array $options = array()) {
  $options += array(
    'username' => NULL,
    'password' => NULL,
    'accept_invalid_cert' => FALSE,
    'cache_http_result' => TRUE,
  );

  // Make sure a request timeout is set.
  if (empty($options['timeout'])) {
    $options['timeout'] = variable_get('http_request_timeout', 30);
  }

  // Intra-pagedownload cache, avoid to download the same content twice within
  // one page download (it's possible, compatible and parse calls).
  $cached_urls = &drupal_static(__FUNCTION__, array());
  if (!empty($cached_urls[$url])) {
    $cache = http_request_get_cache($url);
    if ($cache->data) {
      return $cache->data;
    }
  }

  if (!$options['username'] && valid_url($url, TRUE)) {
    // Handle password protected feeds.
    $url_parts = parse_url($url);
    if (!empty($url_parts['user'])) {
      $options['password'] = urldecode($url_parts['pass']);
      $options['username'] = urldecode($url_parts['user']);
    }
  }

  $curl = http_request_use_curl();

  // Only download and parse data if really needs refresh.
  // Based on "Last-Modified" and "If-Modified-Since".
  $headers = array();
  if ($options['cache_http_result'] && $cache = http_request_get_cache($url)) {
    $last_result = $cache->data;
    $last_headers = array_change_key_case($last_result->headers);

    if (!empty($last_headers['etag'])) {
      if ($curl) {
        $headers[] = 'If-None-Match: ' . $last_headers['etag'];
      }
      else {
        $headers['If-None-Match'] = $last_headers['etag'];
      }
    }
    if (!empty($last_headers['last-modified'])) {
      if ($curl) {
        $headers[] = 'If-Modified-Since: ' . $last_headers['last-modified'];
      }
      else {
        $headers['If-Modified-Since'] = $last_headers['last-modified'];
      }
    }
    if (!empty($options['username']) && !$curl) {
      $headers['Authorization'] = 'Basic ' . base64_encode($options['username'] . ':' . $options['password']);
    }
  }

  // Support the 'feed' and 'webcal' schemes by converting them into 'http'.
  $url = strtr($url, array('feed://' => 'http://', 'webcal://' => 'http://'));

  if ($curl) {
    $headers[] = 'User-Agent: Drupal (+http://drupal.org/)';
    $result = new stdClass();
    $result->headers = array();

    // Parse the URL and make sure we can handle the schema.
    // cURL can only support either http:// or https://.
    // CURLOPT_PROTOCOLS is only supported with cURL 7.19.4.
    $uri = parse_url($url);
    if ($uri === FALSE) {
      $result->error = 'unable to parse URL';
      $result->code = FEEDS_ERROR_PARSE_ERROR;
    }
    elseif (!isset($uri['scheme'])) {
      $result->error = 'missing schema';
      $result->code = FEEDS_ERROR_NO_SCHEME;
    }
    else {
      switch ($uri['scheme']) {
        case 'http':
        case 'https':
          // Valid scheme.
          break;

        default:
          $result->error = 'invalid schema ' . $uri['scheme'];
          $result->code = FEEDS_ERROR_INVALID_SCHEME;
          break;
      }
    }

    // If the scheme was valid, continue to request the feed using cURL.
    if (empty($result->error)) {
      $download = curl_init($url);
      curl_setopt($download, CURLOPT_FOLLOWLOCATION, TRUE);
      if (!empty($options['username'])) {
        curl_setopt($download, CURLOPT_USERPWD, $options['username'] . ':' . $options['password']);
        curl_setopt($download, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
      }
      curl_setopt($download, CURLOPT_HTTPHEADER, $headers);
      curl_setopt($download, CURLOPT_HEADER, TRUE);
      curl_setopt($download, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($download, CURLOPT_ENCODING, '');
      curl_setopt($download, CURLOPT_TIMEOUT, $options['timeout']);

      $proxy_server = variable_get('proxy_server');

      if ($proxy_server && _drupal_http_use_proxy($uri['host'])) {
        curl_setopt($download, CURLOPT_PROXY, $proxy_server);
        curl_setopt($download, CURLOPT_PROXYPORT, variable_get('proxy_port', 8080));

        // Proxy user/password.
        if ($proxy_username = variable_get('proxy_username')) {
          $username_password = $proxy_username . ':' . variable_get('proxy_password', '');

          curl_setopt($download, CURLOPT_PROXYUSERPWD, $username_password);
          curl_setopt($download, CURLOPT_PROXYAUTH, variable_get('proxy_auth_method', CURLAUTH_BASIC));
        }
      }

      if ($options['accept_invalid_cert']) {
        curl_setopt($download, CURLOPT_SSL_VERIFYPEER, 0);
        curl_setopt($download, CURLOPT_SSL_VERIFYHOST, 0);
      }
      $header = '';
      $result->data = curl_exec($download);
      if (curl_error($download)) {
        throw new HRCurlException(
          t('cURL error (@code) @error for @url', array(
            '@code' => curl_errno($download),
            '@error' => curl_error($download),
            '@url' => $url,
          )), curl_errno($download)
        );
      }

      // When using a proxy, remove extra data from the header which is not
      // considered by CURLINFO_HEADER_SIZE (possibly cURL bug).
      // This data is only added when to HTTP header when working with a proxy.
      // Example string added: <HTTP/1.0 200 Connection established\r\n\r\n>
      // This was fixed in libcurl version 7.30.0 (0x71e00) (April 12, 2013),
      // so this workaround only removes the proxy-added headers if we are using
      // an older version of libcurl.
      $curl_ver = curl_version();

      if ($proxy_server && $curl_ver['version_number'] < 0x71e00 && _drupal_http_use_proxy($uri['host'])) {
        $http_header_break = "\r\n\r\n";
        $response = explode($http_header_break, $result->data);
        if (count($response) > 2) {
          $result->data = substr($result->data, strlen($response[0] . $http_header_break), strlen($result->data));
        }
      }

      $header_size = curl_getinfo($download, CURLINFO_HEADER_SIZE);
      $header = substr($result->data, 0, $header_size - 1);
      $result->data = substr($result->data, $header_size);
      $headers = preg_split("/(\r\n){2}/", $header);
      $header_lines = preg_split("/\r\n|\n|\r/", end($headers));
      // Skip HTTP response status.
      array_shift($header_lines);

      while ($line = trim(array_shift($header_lines))) {
        list($header, $value) = explode(':', $line, 2);
        // Normalize the headers.
        $header = strtolower($header);

        if (isset($result->headers[$header]) && $header == 'set-cookie') {
          // RFC 2109: the Set-Cookie response header comprises the token Set-
          // Cookie:, followed by a comma-separated list of one or more cookies.
          $result->headers[$header] .= ',' . trim($value);
        }
        else {
          $result->headers[$header] = trim($value);
        }
      }
      $result->code = curl_getinfo($download, CURLINFO_HTTP_CODE);

      curl_close($download);
    }
  }
  else {
    $result = drupal_http_request($url, array(
      'headers' => $headers,
      'timeout' => $options['timeout'],
    ));
    $result->headers = isset($result->headers) ? $result->headers : array();
  }

  $result->code = isset($result->code) ? $result->code : 200;

  // In case of 304 Not Modified try to return cached data.
  if ($result->code == 304) {

    if (isset($last_result->data)) {
      $last_result->from_cache = TRUE;
      return $last_result;
    }
    else {
      // It's a tragedy, this file must exist and contain good data.
      // In this case, clear cache and repeat.
      http_request_clear_cache($url);
      return feeds_http_request($url, $options);
    }
  }

  // Set caches if asked.
  if ($options['cache_http_result']) {
    http_request_set_cache($url, $result);
    // In the static cache, mark this URL as being cached.
    $cached_urls[$url] = TRUE;
  }

  return $result;
}

/**
 * Checks the result of the HTTP Request.
 *
 * @param string $url
 *   The URL that was requested.
 * @param object $result
 *   The HTTP Request result.
 *
 * @throws FeedsHTTPRequestException
 *   In case the result code of the HTTP request is not in the 2xx series.
 */
function http_request_check_result($url, $result) {
  if (!in_array($result->code, array(200, 201, 202, 203, 204, 205, 206))) {
    $vars = array(
      '@url' => $url,
      '@code' => $result->code,
      '@error' => isset($result->error) ? $result->error : 'Unknown error',
    );

    switch ($result->code) {
      case FEEDS_ERROR_PARSE_ERROR:
        $message = t('Download of @url failed because it could not be parsed.', $vars);
        break;

      case FEEDS_ERROR_NO_SCHEME:
        $message = t("Download of @url failed because its scheme could not be determined. The URL is expected to start with something like '@example'.", $vars + array(
          '@example' => 'http://',
        ));
        break;

      case FEEDS_ERROR_INVALID_SCHEME:
        $message = t('Download of @url failed because its scheme is not supported: @error. Examples of supported schemes are: @supported.', $vars + array(
          '@supported' => implode(', ', array('http', 'https')),
        ));
        break;

      default:
        if (isset($result->error)) {
          $message = t('Download of @url failed with code @code and the following error: @error.', $vars);
        }
        else {
          $message = t('Download of @url failed with code @code.', $vars);
        }
        break;
    }

    throw new FeedsHTTPRequestException($message);
  }
}

/**
 * Decides if it's possible to use cURL or not.
 *
 * @return bool
 *   TRUE if cURL may be used, FALSE otherwise.
 */
function http_request_use_curl() {
  // Allow site administrators to choose to not use cURL.
  if (variable_get('feeds_never_use_curl', FALSE)) {
    return FALSE;
  }

  // Check that the PHP cURL extension has been enabled.
  if (!extension_loaded('curl')) {
    return FALSE;
  }

  // cURL below PHP 5.6.0 must not have open_basedir or safe_mode enabled.
  if (version_compare(PHP_VERSION, '5.6.0', '<')) {
    // phpcs:ignore PHPCompatibility.IniDirectives.RemovedIniDirectives.safe_modeDeprecatedRemoved
    return !ini_get('safe_mode') && !ini_get('open_basedir');
  }

  // cURL in PHP 5.6.0 and above re-enables CURLOPT_FOLLOWLOCATION with
  // open_basedir so there is no need to check for this.
  return TRUE;
}

/**
 * Clear cache for a specific URL.
 *
 * @param string $url
 *   The URL to clear.
 */
function http_request_clear_cache($url) {
  cache_clear_all(hash('sha256', $url), 'cache_feeds_http');
}

/**
 * Gets the cache for a specific URL.
 *
 * @param string $url
 *   The URL to find the cached item.
 *
 * @return object|false
 *   The cache or FALSE on failure.
 */
function http_request_get_cache($url) {
  return cache_get(hash('sha256', $url), 'cache_feeds_http');
}

/**
 * Sets the cache for a specific URL.
 *
 * @param string $url
 *   The URL to cache.
 * @param object $result
 *   The result of the HTTP request.
 */
function http_request_set_cache($url, $result) {
  $item = ($result instanceof FeedsHTTPCacheItem) ? $result : new FeedsHTTPCacheItem(hash('sha256', $url), $result);
  $item->cacheSet();
}

/**
 * Returns if the provided $content_type is a feed.
 *
 * @param string $content_type
 *   The Content-Type header.
 *
 * @param string $data
 *   The actual data from the http request.
 *
 * @return bool
 *   Returns TRUE if this is a parsable feed.
 */
function http_request_is_feed($content_type, $data) {
  $pos = strpos($content_type, ';');
  if ($pos !== FALSE) {
    $content_type = substr($content_type, 0, $pos);
  }
  $content_type = strtolower($content_type);
  if (strpos($content_type, 'xml') !== FALSE) {
    return TRUE;
  }

  // @TODO: Sometimes the content-type can be text/html but still be a valid
  // feed.
  return FALSE;
}

/**
 * Finds potential feed tags in the HTML document.
 *
 * @param string $html
 *   The html string to search.
 *
 * @return array
 *   An array of href to feeds.
 */
function http_request_find_feeds($html) {
  $matches = array();
  preg_match_all(HTTP_REQUEST_PCRE_LINK_TAG, $html, $matches);
  $links = $matches[1];
  $valid_links = array();

  // Build up all the links information.
  foreach ($links as $link_tag) {
    $attributes = array();
    $candidate = array();

    preg_match_all(HTTP_REQUEST_PCRE_TAG_ATTRIBUTES, $link_tag, $attributes, PREG_SET_ORDER);
    foreach ($attributes as $attribute) {
      // Find the key value pairs, attribute[1] is key and attribute[2] is the
      // value.  However, if the link tag used single quotes, the value might
      // be in attribute[3] instead.
      if (empty($attribute[2])) {
        $attribute[2] = $attribute[3];
      }
      if (!empty($attribute[1]) && !empty($attribute[2])) {
        $candidate[drupal_strtolower($attribute[1])] = drupal_strtolower(decode_entities($attribute[2]));
      }
    }

    // Examine candidate to see if it s a feed.
    // @TODO: could/should use http_request_is_feed ??
    if (isset($candidate['rel']) && $candidate['rel'] == 'alternate') {
      if (isset($candidate['href']) && isset($candidate['type']) && strpos($candidate['type'], 'xml') !== FALSE) {
        // All tests pass, its a valid candidate.
        $valid_links[] = $candidate['href'];
      }
    }
  }

  return $valid_links;
}

/**
 * Create an absolute url.
 *
 * @param string $url
 *   The href to transform.
 * @param string $base_url
 *   The url to be used as the base for a relative $url.
 *
 * @return string
 *   An absolute url
 */
function http_request_create_absolute_url($url, $base_url) {
  $url = trim($url);
  if (valid_url($url, TRUE)) {
    // Valid absolute url already.
    return $url;
  }

  // Turn relative url into absolute.
  if (valid_url($url, FALSE)) {
    // Produces variables $scheme, $host, $user, $pass, $path, $query and
    // $fragment.
    $parsed_url = parse_url($base_url);
    if ($parsed_url === FALSE) {
      // Invalid $base_url.
      return FALSE;
    }

    $path = isset($parsed_url['path']) ? $parsed_url['path'] : '';
    if (strlen($path) > 0 && substr($path, -1) != '/') {
      // Path ends not with '/', so remove all before previous '/'.
      $path = dirname($path);
    }

    // Adding to the existing path.
    $cparts = array();
    if ($url[0] == '/') {
      $cparts = array_filter(explode("/", $url));
    }
    else {
      // Backtracking from the existing path.
      $path_cparts = array_filter(explode("/", $path));
      $url_cparts = array_filter(explode("/", $url));
      $cparts = array_merge($path_cparts, $url_cparts);
    }

    $remove_parts = 0;
    // Start from behind.
    $reverse_cparts = array_reverse($cparts);
    foreach ($reverse_cparts as $i => &$part) {
      if ($part == '.') {
        $part = NULL;
      }
      elseif ($part == '..') {
        $part = NULL;
        $remove_parts++;
      }
      elseif ($remove_parts > 0) {
        // If the current part isn't "..", and we had ".." before, then delete
        // the part.
        $part = NULL;
        $remove_parts--;
      }
    }
    $cparts = array_filter(array_reverse($reverse_cparts));
    $path = implode("/", $cparts);

    // Build the prefix to the path.
    $absolute_url = '';
    if (isset($parsed_url['scheme'])) {
      $absolute_url = $parsed_url['scheme'] . '://';
    }

    if (isset($parsed_url['user'])) {
      $absolute_url .= $parsed_url['user'];
      if (isset($pass)) {
        $absolute_url .= ':' . $parsed_url['pass'];
      }
      $absolute_url .= '@';
    }
    if (isset($parsed_url['host'])) {
      $absolute_url .= $parsed_url['host'] . '/';
    }

    $absolute_url .= $path;

    if (valid_url($absolute_url, TRUE)) {
      return $absolute_url;
    }
  }
  return FALSE;
}
