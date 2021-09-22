<?php

/**
 * @file
 *
 * Extend Linkit with the ability to ping remote sites and provide information
 * about their status. The main purpose is to find broken links easier.
 */

/**
 * The path info callback for the Linkit external plugin. If the path given is
 * an absolute URL, then return information about it. Uses cURL for fetching
 * status about an URL.
 *
 * @see linkit.api.php
 */
function _linkit_external_path_info($path_info, $profile) {
  if (!function_exists('curl_init') || !isset($path_info['safe_url'])) {
    return FALSE;
  }
  $ch = curl_init($path_info['safe_url']);
  curl_setopt_array($ch, array(
    CURLOPT_RETURNTRANSFER => TRUE,
    CURLOPT_FOLLOWLOCATION => TRUE,
    CURLOPT_MAXREDIRS => 5,
    CURLOPT_NOBODY => TRUE,
    CURLOPT_PROTOCOLS => CURLPROTO_HTTP | CURLPROTO_HTTPS | CURLPROTO_FTP, // TODO: Ftp must be allowed in linkit.module
    CURLOPT_TIMEOUT_MS => $profile->data['advanced']['ajaxtimeout'],
    CURLOPT_USERAGENT => 'Linkit/2.x Drupal/7.x',
  ));
  $document = curl_exec($ch);

  if ($status = curl_error($ch)) {
    $class = 'status-warning';
  }
  else {
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    $status = _linkit_external_http_status($http_code);
    if ($http_code >= 200 && $http_code < 300) {
      $class = 'status-ok';
    }
    else {
      $class = 'status-warning';
    }
  }
  $result = array(
    'path' => $path_info['url'],
    'title' => check_plain($title ? $title : $path_info['url']),
    'description' => $status,
    'addClass' => $class,
  );
  curl_close($ch);
  return $result;
}

/**
 * Retrieve text status about a certain http status code. The most common HTTP
 * status codes yields a human readable description.
 *
 * @param $http_code
 *   The HTTP status code
 *
 * @return
 *   A simple text string with human readable status information.
 */
function _linkit_external_http_status($http_code) {
  if ($http_code >= 200 && $http_code < 300) {
    $status = 'Connection successful. HTTP status: ' . $http_code .
              ($http_code == 200 ? ' (OK)' : '');
  }
  elseif ($http_code >= 300 && $http_code < 400) {
    $status = 'Too many redirects. HTTP status: ' . $http_code;
  }
  elseif ($http_code >= 400 && $http_code < 500) {
    $status = 'Client error. HTTP status: ' . $http_code;
    switch ($http_code) {
      case 403:
        $status .= ' (Forbidden)';
        break;
      case 404:
        $status .= ' (Page not found)';
        break;
    }
  }
  elseif ($http_code >= 500 && $http_code < 600) {
    $status = 'Server error. HTTP status: ' . $http_code .
              ($http_code == 500 ? ' (Internal server error)' : '');
  }
  else {
    $status = 'An error occured. HTTP status: ' . $http_code;
  }
  return $status;
}
