<?php
/**
 * @file
 */

/**
 * Return information about consumers related to a user.
 *
 * @param int $uid
 *  User ID to retrieve consumer info for.
 * @return array
 *  An array of consumer info
 */
function oauth_common_user_consumers($uid) {
  $result = db_query('SELECT c.secret, c.configuration, pc.*
    FROM {oauth_common_consumer} c
    INNER JOIN {oauth_common_provider_consumer} pc ON pc.csid = c.csid
    WHERE pc.uid = :uid', array(':uid' => $uid));

  $consumers = array();
  while ($consumer = DrupalOAuthConsumer::fromResult($result)) {
    $consumers[] = $consumer;
  }

  return $consumers;
}

/**
 * Return information about access tokens related to a user.
 *
 * @param int $uid
 *  User ID to retrieve consumer info for.
 * @return array
 *  An array of consumer info
 */
function oauth_common_user_access_tokens($uid) {
  $result = db_query("SELECT * FROM {oauth_common_token} WHERE uid = :uid AND type = :type", array(
    ':uid' => $uid,
    ':type' => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
  ));
  $tokens = array();
  while ($token = DrupalOAuthToken::fromResult($result)) {
    $tokens[] = $token;
  }
  return $tokens;
}

/**
 * Verifies the request
 *
 * @return array
 *  An array containing three elements. The first is TRUE if the request was
 *  signed, otherwise FALSE. Then comes the validated consumer and token objects.
 */
function oauth_common_verify_request() {
  $req = DrupalOAuthRequest::from_request();
  // Verify
  $consumer_key = $req->get_parameter('oauth_consumer_key');
  if (!empty($consumer_key)) {
    $consumer = DrupalOAuthConsumer::loadProviderByKey($consumer_key);
    if ($consumer) {
      $context = oauth_common_context_load($consumer->context);

      if (!$context) {
        throw new Exception('No OAuth context found');
      }

      _oauth_common_verify_body_hash($req);

      // Only verify request if we got a signature
      $signature = $req->get_parameter('oauth_signature');
      if (!empty($signature)) {
        $server = new DrupalOAuthServer($context);
        return array_merge(array(TRUE), $server->verify_request($req));
      }
      else {
        $token_key = $req->get_parameter('oauth_token');
        if (empty($token_key) || !($token = DrupalOAuthToken::loadbyKey($token_key, $consumer))) {
          $token = NULL;
        }
        return array(FALSE, $consumer, $token);
      }
    }
  }
  return array(FALSE, NULL, NULL);
}

function _oauth_common_verify_body_hash($req) {
  $body_hash = $req->get_parameter('oauth_body_hash');
  if ($body_hash && module_exists('inputstream')) {
    $hres = hash_init('sha1');
    $stream = fopen('drupal://input', 'r');
    hash_update_stream($hres, $stream);
    fclose($stream);
    $sha1 = hash_final($hres, TRUE);
    if ($sha1 !== base64_decode($body_hash)) {
      throw new OAuthException("Invalid body hash");
    }
  }
}
