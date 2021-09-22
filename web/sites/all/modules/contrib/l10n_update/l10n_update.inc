<?php

/**
 * @file
 *   Reusable API for l10n remote updates.
 */

include_once DRUPAL_ROOT . '/includes/locale.inc';
module_load_include('locale.inc', 'l10n_update');

/**
 * Default update server, filename and URL.
 */
define('L10N_UPDATE_DEFAULT_SERVER', 'localize.drupal.org');
define('L10N_UPDATE_DEFAULT_SERVER_URL', 'http://localize.drupal.org/l10n_server.xml');
define('L10N_UPDATE_DEFAULT_UPDATE_URL', 'http://ftp.drupal.org/files/translations/%core/%project/%project-%release.%language.po');

// Translation filename, will be used just for local imports
define('L10N_UPDATE_DEFAULT_FILENAME', '%project-%release.%language.po');

// Translation status: String imported from po
define('L10N_UPDATE_STRING_DEFAULT', 0);

// Translation status: Custom string, overridden original import
define('L10N_UPDATE_STRING_CUSTOM', 1);

/**
 * Retrieve data for default server.
 *
 * @return array
 *   Server parameters:
 *     name :       Localization server name
 *     server_url : Localization server URL where language list can be retrieved.
 *     update_url : URL containing po file pattern.
 */
function l10n_update_default_server() {
  return array(
    'name' => variable_get('l10n_update_default_server', L10N_UPDATE_DEFAULT_SERVER),
    'server_url' => variable_get('l10n_update_default_server_url', L10N_UPDATE_DEFAULT_SERVER_URL),
    'update_url' => variable_get('l10n_update_default_update_url', L10N_UPDATE_DEFAULT_UPDATE_URL),
  );
}

/**
 * Download and import remote translation file.
 *
 * @param $download_url
 *   Download URL.
 * @param $locale
 *   Language code.
 * @param $mode
 *   Download mode. How to treat exising and modified translations.
 *
 * @return boolean
 *   TRUE on success.
 */
function l10n_update_download_import($download_url, $locale, $mode = LOCALE_IMPORT_OVERWRITE) {
  if ($file = l10n_update_download_file($download_url)) {
    $result = l10n_update_import_file($file, $locale, $mode);
    return $result;
  }
}

/**
 * Import local file into the database.
 *
 * @param $file
 *   File object of localy stored file
 *   or path to localy stored file.
 * @param $locale
 *   Language code.
 * @param $mode
 *   Download mode. How to treat exising and modified translations.
 *
 * @return boolean
 *   Result array on success. FALSE on failure
 */
function l10n_update_import_file($file, $locale, $mode = LOCALE_IMPORT_OVERWRITE) {
  // If the file is a uri, create a $file object
  if (is_string($file)) {
    $uri = $file;
    $file = new stdClass();
    $file->uri = $uri;
    $file->filename = $uri;
  }
  return _l10n_update_locale_import_po($file, $locale, $mode, 'default');
}

/**
 * Get remote file and download it to a temporary path.
 *
 * @param $download_url
 *   URL of remote file.
 * @param $destination
 *   URL of local destination file. By default the download will be stored
 *   in a temporary file.
 */
function l10n_update_download_file($download_url, $destination = NULL) {
  $t = get_t();
  $variables['%download_link'] = $download_url;

  // Create temporary file or use specified file destination.
  // Temporary files get a 'translation-' file name prefix.
  $file = $destination ? $destination : drupal_tempnam(file_directory_temp(), 'translation-');

  if ($file) {
    $variables['%tmpfile'] = $file;
    // We download and store the file (in one if statement! Isnt't that neat ;) ).
    // @todo remove the timeout once we use the batch API to download the files.
    if (($contents = drupal_http_request($download_url, array('timeout' => 90))) && $contents->code == 200 && $file_result = file_put_contents($file, $contents->data)) {
      watchdog('l10n_update', 'Successfully downloaded %download_link to %tmpfile', $variables);
      return $file;
    }
    else {
      if (isset($contents->error)) {
        watchdog('l10n_update', 'An error occured during the download operation: %error.', array('%error' => $contents->error), WATCHDOG_ERROR);
      }
      elseif (isset($contents->code) && $contents->code != 200) {
        watchdog('l10n_update', 'An error occured during the download operation: HTTP status code %code.', array('%code' => $contents->code), WATCHDOG_ERROR);
      }
      if (isset($file_result)) {
        // file_put_contents() was called but returned FALSE.
        watchdog('l10n_update', 'Unable to save %download_link file to %tmpfile.', $variables, WATCHDOG_ERROR);
      }
    }

  }
  else {
    $variables['%tmpdir'] = file_directory_temp();
    watchdog('l10n_update', 'Error creating temporary file for download in %tmpdir. Remote file is %download_link.', $variables, WATCHDOG_ERROR);
  }
}

/**
 * Get names for the language list from locale system.
 *
 * @param $string_list
 *   Comma separated list of language codes.
 *   Language codes must exist in languages from _locale_get_predefined_list().
 * @return array
 *   Array of language names keyed by language code.
 */
function l10n_update_get_language_names($string_list) {
  $t = get_t();
  $language_codes = array_map('trim', explode(',', $string_list));
  $languages = _locale_get_predefined_list();
  $result = array();
  foreach ($language_codes as $lang) {
    if (array_key_exists($lang, $languages)) {
      // Try to use verbose locale name
      $name = $lang;
      $name = $languages[$name][0] . (isset($languages[$name][1]) ? ' ' . $t('(@language)', array('@language' => $languages[$name][1])) : '');
      $result[$lang] = $name;
    }
  }
  return $result;
}

/**
 * Build project data as an object.
 *
 * @param $name
 *   Project name.
 * @param $version
 *   Project version.
 * @param $server
 *   Localisation server name.
 * @param $path
 *   Localisation server URL.
 * @return object
 *   Project object containing the supplied data.
 */
function _l10n_update_build_project($name, $version = NULL, $server = L10N_UPDATE_DEFAULT_SERVER, $path = L10N_UPDATE_DEFAULT_SERVER_URL) {
  $project = new stdClass();
  $project->name = $name;
  $project->version = $version;
  $project->l10n_server = $server;
  $project->l10n_path = $path;
  return $project;
}

/**
 * Update the file history table.
 *
 * @param $file
 *   Object representing the file just imported or downloaded.
 * @return integer
 *   FALSE on failure. Otherwise SAVED_NEW or SAVED_UPDATED.
 *   @see drupal_write_record()
 */
function l10n_update_file_history($file) {
  // Update or write new record
  if (db_query("SELECT project FROM {l10n_update_file} WHERE project = :project AND language = :language", array(':project' => $file->project, ':language' => $file->language))->fetchField()) {
    $update = array('project', 'language');
  }
  else {
    $update = array();
  }
  return drupal_write_record('l10n_update_file', $file, $update);
}

/**
 * Delete the history of downloaded translations.
 *
 * @param string $langcode
 *   Language code of the file history to be deleted.
 */
function l10n_update_delete_file_history($langcode) {
  db_delete('l10n_update_file')
    ->condition('language', $langcode)
    ->execute();
}

/**
 * Flag the file history as up to date.
 *
 * Compare history data in the {l10n_update_file} table with translations
 * available at translations server(s). Update the 'last_checked' timestamp of
 * the files which are up to date.
 *
 * @param $available
 *   Available translations as retreived from remote server.
 */
function l10n_update_flag_history($available) {
  if ($history = l10n_update_get_history()) {
    foreach($history as $name => $project) {
      foreach ($project as $langcode => $current) {
        if (isset($available[$name][$langcode])) {
          $update = $available[$name][$langcode];
          // When the available update is equal to the current translation the current
          // is marked checked in the {l10n_update_file} table.
          if (_l10n_update_source_compare($current, $update) == 0 && $current->version == $update->version) {
            db_update('l10n_update_file')
              ->fields(array(
                'last_checked' => REQUEST_TIME,
              ))
              ->condition('project', $current->project)
              ->condition('language', $current->language)
              ->execute();
          }
        }
      }
    }
  }
}

/**
 * Check if remote file exists and when it was last updated.
 *
 * @param $url
 *   URL of remote file.
 * @param $headers
 *   HTTP request headers.
 * @return object
 *   Result object containing the HTTP request headers, response code, headers,
 *   data, redirect status and updated timestamp.
 *   @see l10n_update_http_request()
 */
function l10n_update_http_check($url, $headers = array()) {
  $result = l10n_update_http_request($url, array('headers' => $headers, 'method' => 'HEAD'));
  if (!isset($result->error)) {
    if ($result && $result->code == 200) {
      $result->updated = isset($result->headers['last-modified']) ? strtotime($result->headers['last-modified']) : 0;
    }
    return $result;
  }
  else {
    switch ($result->code) {
      case 404:
        // File not found occurs when a translation file is not yet available
        // at the translation server. But also if a custom module or custom
        // theme does not define the location of a translation file. By default
        // the file is checked at the translation server, but it will not be
        // found there.
        watchdog('l10n_update', 'File not found: @uri.', array('@uri' => $url));
        return TRUE;
      case 0:
        watchdog('l10n_update', 'Error occurred when trying to check @remote: @errormessage.', array('@errormessage' => $result->error, '@remote' => $url), WATCHDOG_ERROR);
        break;
      default:
        watchdog('l10n_update', 'HTTP error @errorcode occurred when trying to check @remote.', array('@errorcode' => $result->code, '@remote' => $url), WATCHDOG_ERROR);
        break;
    }
  }
  return $result;
}

/**
 * Perform an HTTP request.
 *
 * We cannot use drupal_http_request() at install, see http://drupal.org/node/527484
 *
 * This is a flexible and powerful HTTP client implementation. Correctly
 * handles GET, POST, PUT or any other HTTP requests. Handles redirects.
 *
 * @param $url
 *   A string containing a fully qualified URI.
 * @param array $options
 *   (optional) An array that can have one or more of the following elements:
 *   - headers: An array containing request headers to send as name/value pairs.
 *   - method: A string containing the request method. Defaults to 'GET'.
 *   - data: A string containing the request body, formatted as
 *     'param=value&param=value&...'. Defaults to NULL.
 *   - max_redirects: An integer representing how many times a redirect
 *     may be followed. Defaults to 3.
 *   - timeout: A float representing the maximum number of seconds the function
 *     call may take. The default is 30 seconds. If a timeout occurs, the error
 *     code is set to the HTTP_REQUEST_TIMEOUT constant.
 *   - context: A context resource created with stream_context_create().
 *
 * @return object
 *   An object that can have one or more of the following components:
 *   - request: A string containing the request body that was sent.
 *   - code: An integer containing the response status code, or the error code
 *     if an error occurred.
 *   - protocol: The response protocol (e.g. HTTP/1.1 or HTTP/1.0).
 *   - status_message: The status message from the response, if a response was
 *     received.
 *   - redirect_code: If redirected, an integer containing the initial response
 *     status code.
 *   - redirect_url: If redirected, a string containing the URL of the redirect
 *     target.
 *   - error: If an error occurred, the error message. Otherwise not set.
 *   - headers: An array containing the response headers as name/value pairs.
 *     HTTP header names are case-insensitive (RFC 2616, section 4.2), so for
 *     easy access the array keys are returned in lower case.
 *   - data: A string containing the response body that was received.
 */
function l10n_update_http_request($url, array $options = array()) {
  $result = new stdClass();

  // Parse the URL and make sure we can handle the schema.
  $uri = @parse_url($url);

  if ($uri == FALSE) {
    $result->error = 'unable to parse URL';
    $result->code = -1001;
    return $result;
  }

  if (!isset($uri['scheme'])) {
    $result->error = 'missing schema';
    $result->code = -1002;
    return $result;
  }

  timer_start(__FUNCTION__);

  // Merge the default options.
  $options += array(
    'headers' => array(),
    'method' => 'GET',
    'data' => NULL,
    'max_redirects' => 3,
    'timeout' => 30.0,
    'context' => NULL,
  );

  // Merge the default headers.
  $options['headers'] += array(
    'User-Agent' => 'Drupal (+http://drupal.org/)',
  );

  // stream_socket_client() requires timeout to be a float.
  $options['timeout'] = (float) $options['timeout'];

  // Use a proxy if one is defined and the host is not on the excluded list.
  $proxy_server = variable_get('proxy_server', '');
  if ($proxy_server && _drupal_http_use_proxy($uri['host'])) {
    // Set the scheme so we open a socket to the proxy server.
    $uri['scheme'] = 'proxy';
    // Set the path to be the full URL.
    $uri['path'] = $url;
    // Since the URL is passed as the path, we won't use the parsed query.
    unset($uri['query']);

    // Add in username and password to Proxy-Authorization header if needed.
    if ($proxy_username = variable_get('proxy_username', '')) {
      $proxy_password = variable_get('proxy_password', '');
      $options['headers']['Proxy-Authorization'] = 'Basic ' . base64_encode($proxy_username . (!empty($proxy_password) ? ":" . $proxy_password : ''));
    }
    // Some proxies reject requests with any User-Agent headers, while others
    // require a specific one.
    $proxy_user_agent = variable_get('proxy_user_agent', '');
    // The default value matches neither condition.
    if ($proxy_user_agent === NULL) {
      unset($options['headers']['User-Agent']);
    }
    elseif ($proxy_user_agent) {
      $options['headers']['User-Agent'] = $proxy_user_agent;
    }
  }

  switch ($uri['scheme']) {
    case 'proxy':
      // Make the socket connection to a proxy server.
      $socket = 'tcp://' . $proxy_server . ':' . variable_get('proxy_port', 8080);
      // The Host header still needs to match the real request.
      $options['headers']['Host'] = $uri['host'];
      $options['headers']['Host'] .= isset($uri['port']) && $uri['port'] != 80 ? ':' . $uri['port'] : '';
      break;

    case 'http':
    case 'feed':
      $port = isset($uri['port']) ? $uri['port'] : 80;
      $socket = 'tcp://' . $uri['host'] . ':' . $port;
      // RFC 2616: "non-standard ports MUST, default ports MAY be included".
      // We don't add the standard port to prevent from breaking rewrite rules
      // checking the host that do not take into account the port number.
      $options['headers']['Host'] = $uri['host'] . ($port != 80 ? ':' . $port : '');
      break;

    case 'https':
      // Note: Only works when PHP is compiled with OpenSSL support.
      $port = isset($uri['port']) ? $uri['port'] : 443;
      $socket = 'ssl://' . $uri['host'] . ':' . $port;
      $options['headers']['Host'] = $uri['host'] . ($port != 443 ? ':' . $port : '');
      break;

    default:
      $result->error = 'invalid schema ' . $uri['scheme'];
      $result->code = -1003;
      return $result;
  }

  if (empty($options['context'])) {
    $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout']);
  }
  else {
    // Create a stream with context. Allows verification of a SSL certificate.
    $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout'], STREAM_CLIENT_CONNECT, $options['context']);
  }

  // Make sure the socket opened properly.
  if (!$fp) {
    // When a network error occurs, we use a negative number so it does not
    // clash with the HTTP status codes.
    $result->code = -$errno;
    $result->error = trim($errstr) ? trim($errstr) : t('Error opening socket @socket', array('@socket' => $socket));

    // Mark that this request failed. This will trigger a check of the web
    // server's ability to make outgoing HTTP requests the next time that
    // requirements checking is performed.
    // See system_requirements().
    // variable_set('drupal_http_request_fails', TRUE);

    return $result;
  }

  // Construct the path to act on.
  $path = isset($uri['path']) ? $uri['path'] : '/';
  if (isset($uri['query'])) {
    $path .= '?' . $uri['query'];
  }

  // Only add Content-Length if we actually have any content or if it is a POST
  // or PUT request. Some non-standard servers get confused by Content-Length in
  // at least HEAD/GET requests, and Squid always requires Content-Length in
  // POST/PUT requests.
  $content_length = strlen($options['data']);
  if ($content_length > 0 || $options['method'] == 'POST' || $options['method'] == 'PUT') {
    $options['headers']['Content-Length'] = $content_length;
  }

  // If the server URL has a user then attempt to use basic authentication.
  if (isset($uri['user'])) {
    $options['headers']['Authorization'] = 'Basic ' . base64_encode($uri['user'] . (isset($uri['pass']) ? ':' . $uri['pass'] : ''));
  }

  // If the database prefix is being used by SimpleTest to run the tests in a copied
  // database then set the user-agent header to the database prefix so that any
  // calls to other Drupal pages will run the SimpleTest prefixed database. The
  // user-agent is used to ensure that multiple testing sessions running at the
  // same time won't interfere with each other as they would if the database
  // prefix were stored statically in a file or database variable.
  $test_info = &$GLOBALS['drupal_test_info'];
  if (!empty($test_info['test_run_id'])) {
    $options['headers']['User-Agent'] = drupal_generate_test_ua($test_info['test_run_id']);
  }

  $request = $options['method'] . ' ' . $path . " HTTP/1.0\r\n";
  foreach ($options['headers'] as $name => $value) {
    $request .= $name . ': ' . trim($value) . "\r\n";
  }
  $request .= "\r\n" . $options['data'];
  $result->request = $request;
  // Calculate how much time is left of the original timeout value.
  $timeout = $options['timeout'] - timer_read(__FUNCTION__) / 1000;
  if ($timeout > 0) {
    stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
    fwrite($fp, $request);
  }

  // Fetch response. Due to PHP bugs like http://bugs.php.net/bug.php?id=43782
  // and http://bugs.php.net/bug.php?id=46049 we can't rely on feof(), but
  // instead must invoke stream_get_meta_data() each iteration.
  $info = stream_get_meta_data($fp);
  $alive = !$info['eof'] && !$info['timed_out'];
  $response = '';

  while ($alive) {
    // Calculate how much time is left of the original timeout value.
    $timeout = $options['timeout'] - timer_read(__FUNCTION__) / 1000;
    if ($timeout <= 0) {
      $info['timed_out'] = TRUE;
      break;
    }
    stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
    $chunk = fread($fp, 1024);
    $response .= $chunk;
    $info = stream_get_meta_data($fp);
    $alive = !$info['eof'] && !$info['timed_out'] && $chunk;
  }
  fclose($fp);

  if ($info['timed_out']) {
    $result->code = HTTP_REQUEST_TIMEOUT;
    $result->error = 'request timed out';
    return $result;
  }
  // Parse response headers from the response body.
  // Be tolerant of malformed HTTP responses that separate header and body with
  // \n\n or \r\r instead of \r\n\r\n.
  list($response, $result->data) = preg_split("/\r\n\r\n|\n\n|\r\r/", $response, 2);
  $response = preg_split("/\r\n|\n|\r/", $response);

  // Parse the response status line.
  list($protocol, $code, $status_message) = explode(' ', trim(array_shift($response)), 3);
  $result->protocol = $protocol;
  $result->status_message = $status_message;

  $result->headers = array();

  // Parse the response headers.
  while ($line = trim(array_shift($response))) {
    list($name, $value) = explode(':', $line, 2);
    $name = strtolower($name);
    if (isset($result->headers[$name]) && $name == 'set-cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$name] .= ',' . trim($value);
    }
    else {
      $result->headers[$name] = trim($value);
    }
  }

  $responses = array(
    100 => 'Continue',
    101 => 'Switching Protocols',
    200 => 'OK',
    201 => 'Created',
    202 => 'Accepted',
    203 => 'Non-Authoritative Information',
    204 => 'No Content',
    205 => 'Reset Content',
    206 => 'Partial Content',
    300 => 'Multiple Choices',
    301 => 'Moved Permanently',
    302 => 'Found',
    303 => 'See Other',
    304 => 'Not Modified',
    305 => 'Use Proxy',
    307 => 'Temporary Redirect',
    400 => 'Bad Request',
    401 => 'Unauthorized',
    402 => 'Payment Required',
    403 => 'Forbidden',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    406 => 'Not Acceptable',
    407 => 'Proxy Authentication Required',
    408 => 'Request Time-out',
    409 => 'Conflict',
    410 => 'Gone',
    411 => 'Length Required',
    412 => 'Precondition Failed',
    413 => 'Request Entity Too Large',
    414 => 'Request-URI Too Large',
    415 => 'Unsupported Media Type',
    416 => 'Requested range not satisfiable',
    417 => 'Expectation Failed',
    500 => 'Internal Server Error',
    501 => 'Not Implemented',
    502 => 'Bad Gateway',
    503 => 'Service Unavailable',
    504 => 'Gateway Time-out',
    505 => 'HTTP Version not supported',
  );
  // RFC 2616 states that all unknown HTTP codes must be treated the same as the
  // base code in their class.
  if (!isset($responses[$code])) {
    $code = floor($code / 100) * 100;
  }
  $result->code = $code;

  switch ($code) {
    case 200: // OK
    case 304: // Not modified
      break;
    case 301: // Moved permanently
    case 302: // Moved temporarily
    case 307: // Moved temporarily
      $location = $result->headers['location'];
      $options['timeout'] -= timer_read(__FUNCTION__) / 1000;
      if ($options['timeout'] <= 0) {
        $result->code = HTTP_REQUEST_TIMEOUT;
        $result->error = 'request timed out';
      }
      elseif ($options['max_redirects']) {
        // Redirect to the new location.
        $options['max_redirects']--;
        $result = l10n_update_http_request($location, $options);
        $result->redirect_code = $code;
      }
      if (!isset($result->redirect_url)) {
        $result->redirect_url = $location;
      }
      break;
    default:
      $result->error = $status_message;
  }

  return $result;
}

/**
 * Build abstract translation source, to be mapped to a file or a download.
 *
 * @param $project
 *   Project object containing data to be inserted in the template.
 * @param $template
 *   String containing place holders. Available place holders:
 *   - '%project': Project name.
 *   - '%release': Poject version.
 *   - '%core': Project core version.
 *   - '%language': Language code.
 *   - '%filename': Project file name.
 * @return string
 *   String with replaced place holders.
 */
function l10n_update_build_string($project, $template) {
  $variables = array(
    '%project' => $project->name,
    '%release' => $project->version,
    '%core' => $project->core,
    '%language' => isset($project->language) ? $project->language : '%language',
    '%filename' => isset($project->filename) ? $project->filename : '%filename',
  );
  return strtr($template, $variables);
}
