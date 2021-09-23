<?php

/**
 * This hook allows to alter the cookies that CookieGuard recognises as:
 * - Essential
 * - Non-Essential
 * - Unknown
 * 
 * @param $cookies
 *   An array of CookieGuard cookies
 */
function hook_cookies_alter(&$cookies){}

/**
 * This hook allows a module to inform the CookieGuard module about the cookies
 * that it sets.  The function should return a keyed array of "cookie" arrays.
 * A cookie array should take the form:
 * 
 * array(
 *   'name' => Translated human readable name of the cookie e.g. Has Javascript
 *   'keys' => A comma delimitted list of cookie keys (e.g. "has_js,DRUPAL_UID")
 *   'description' => Translated description of the cookie
 *   'essential' => TRUE/FALSE - Whether or not the cookie is required.
 * )
 *
 * @param $cookies
 *   An array of CookieGuard cookies
 */
function hook_cookies(&$cookies){}