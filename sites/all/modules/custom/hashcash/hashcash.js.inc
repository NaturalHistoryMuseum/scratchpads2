<?php

/**
 * Enter description here...
 *
 * @param unknown_type $form_id
 */
function hashcash_get_hashcash($form_id){
  // Set the headers and cache stuff so that this URL/Page is _NEVER_ cached.
  header("Content-Type: text/plain; charset=UTF-8");
  // Cookie domain is added to hashcash to prevent same hashcash being used on multiple sites
  global $cookie_domain;
  print '1:' . date('ymd') . ':' . $form_id . ':' . ip_address() . ':' . $cookie_domain . ':';
  // Prevent Boost caching this.
  $GLOBALS['_boost_cache_this'] = FALSE;
}