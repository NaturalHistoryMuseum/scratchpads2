<?php

/**
 * Database abstraction class
 */
class DrupalOAuthDataStore extends OAuthDataStore {
  private $context;

  public function __construct($context) {
    $this->context = $context;
  }

  /**
   * Check if consumer exists from a given consumer key.
   *
   * @param string $consumer_key
   *   The consumer key.
   * @param bool $provider_consumer
   *  Whether the consumer should be a provider consumer
   * @return OAuthConsumer
   *  The consumer
   * @throws OAuthException
   *  An exception is thrown when the consumer cannot be found
   */
  public function lookup_consumer($consumer_key, $provider_consumer = TRUE) {
    $consumer = DrupalOAuthConsumer::loadProviderByKey($consumer_key);
    if (!$consumer) {
      throw new OAuthException('Consumer not found');
    }
    return $consumer;
  }

  /**
   * Check if the token exists.
   *
   * @param OAuthConsumer $consumer
   *  The consumer.
   * @param string $token_type
   *  The type of the token: 'request' or 'access'.
   * @param string $token
   *  The token key.
   * @return DrupalOauthToken
   *  The matching token
   * @throws OAuthException
   *  An exception is thrown when the token cannot be found or doesn't match
   */
  public function lookup_token($consumer, $token_type, $token) {
    $type = ($token_type == 'request' ? OAUTH_COMMON_TOKEN_TYPE_REQUEST : OAUTH_COMMON_TOKEN_TYPE_ACCESS);
    $token = DrupalOAuthToken::loadByKey($token, $consumer, $type);
    if (!$token) {
      throw new OAuthException('Token not found');
    }
    return $token;
  }

  /**
   * Check if the nonce value exists. If not, generate one.
   *
   * @param OAuthConsumer $consumer
   *   The service consumer information with both key
   *   and secret values.
   * @param OAuthToken $token
   *   The current token.
   * @param string $nonce
   *   A new nonce value, in case a one doesn't current exit.
   * @param int $timestamp
   *   The current time.
   * @return string
   *   The existing nonce value or NULL in
   *   case it doesn't exist.
   */
  public function lookup_nonce($consumer, $token, $nonce, $timestamp) {
    if (strlen($nonce) > 255) {
      throw new OAuthException('Nonces may not be longer than 255 characters');
    }

    $stored_nonce = db_query(
      "SELECT nonce FROM {oauth_common_nonce}
      WHERE nonce = :nonce AND timestamp <= :timestamp and token_key = :token_key", array(
        ':nonce'     => $nonce,
        ':timestamp' => $timestamp,
        ':token_key' => $token ? $token->key : '',
      ));

    if (!$stored_nonce->rowCount()) {
      $values = array(
        'nonce'     => $nonce,
        'timestamp' => $timestamp,
        'token_key' => $token ? $token->key : '',
      );
      drupal_write_record('oauth_common_nonce', $values);
      return NULL;
    }

    return $stored_nonce;
  }

  /**
   * Generate a new request token.
   *
   * @param OAuthConsumer $consumer
   *  The consumer to generate a token for.
   * @return DrupalOAuthToken
   *  The request token
   */
  function new_request_token($consumer, $callback = NULL) {
    $token = new DrupalOAuthToken(user_password(32), user_password(32), $consumer, array(
      'type'    => OAUTH_COMMON_TOKEN_TYPE_REQUEST,
      'uid'     => 0,
      'expires' => REQUEST_TIME + variable_get('oauth_common_request_token_lifetime', 7200),
    ));
    $token->write();
    return $token;
  }

  /**
   * Generate a new access token and delete the old request token.
   *
   * @param DrupalOAuthToken $token_old
   *   The old request token.
   * @param OAuthConsumer $consumer
   *   The service consumer information.
   */
  function new_access_token($token_old, $consumer, $verifier = NULL) {
    module_load_include('inc', 'oauth_common');

    if ($token_old && $token_old->authorized) {
      $token_new = new DrupalOAuthToken(user_password(32), user_password(32), $consumer, array(
        'type'       => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
        'uid'        => $token_old->uid,
        'services'   => isset($token_old->services) ? $token_old->services : NULL,
        'authorized' => 1,
      ));
      $token_old->delete();
      $token_new->write();
      return $token_new;
    }

    throw new OAuthException('Invalid request token');
  }
}
