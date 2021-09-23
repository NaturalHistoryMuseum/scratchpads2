<?php

class DrupalOAuthRequest extends OAuthRequest {
  /**
   * Creates a OAuthRequest object from the current request
   *
   * @param string $http_method
   * @param string $http_url
   * @param array $parameters
   * @return OAuthRequest
   *  A OAuthRequest generated from the request
   */
  public static function from_request($http_method = NULL, $http_url = NULL, $parameters = NULL) {
    // Preparations that has to be made if we're going to detect parameters
    if ($parameters == NULL) {
      $qs = $_SERVER['QUERY_STRING'];
      $q = $_GET['q'];

      // Unset $_GET['q'] if it was created by a redirect
      if (isset($_SERVER['REDIRECT_URL'])) {
        $q = FALSE;
      }
      // Check that the q parameter hasn't been created or altered by drupal
      elseif (isset($_GET['q'])) {
        $get = array();
        parse_str($_SERVER['QUERY_STRING'], $get);
        // The q parameter was in the original request, make sure it hasn't been altered
        if (isset($get['q'])) {
          $q = $get['q'];
        }
        // The q parameter was set by drupal, unset it
        else {
          $q = FALSE;
        }
      }

      $parsed = array();
      parse_str($_SERVER['QUERY_STRING'], $parsed);
      if ($q === FALSE) {
        unset($parsed['q']);
      }
      else {
        $parsed['q'] = $q;
      }
      $_SERVER['QUERY_STRING'] = http_build_query($parsed, '', '&');
    }
    $req = parent::from_request($http_method, $http_url, $parameters);

    // Restore $_SERVER['QUERY_STRING'] if it was touched
    if (isset($qs)) {
      $_SERVER['QUERY_STRING'] = $qs;
    }

    return $req;
  }
}
