<?php

class DrupalOAuthToken extends OAuthToken {
  public $tid = 0;
  public $expires = 0;
  public $type = OAUTH_COMMON_TOKEN_TYPE_REQUEST;
  public $uid = 0;

  public $created = 0;
  public $changed = 0;
  public $services = array();
  public $authorized = 0;

  public $in_database = FALSE;

  public function __construct($key, $secret, $consumer, $params = array()) {
    foreach ($params as $param_key => $value) {
      if (isset($this->$param_key)) {
        $this->$param_key = $value;
      }
    }

    // Backwards compatibility with 6.x-3.0-beta3
    if (empty($consumer) || is_array($consumer)) {
      if (is_array($consumer)) {
        $params = $consumer;
      }
      if (!empty($params['csid'])) {
        $consumer = DrupalOAuthConsumer::loadById($params['csid'], isset($params['services']));
      }
    }

    if (!is_object($consumer)) {
      throw new OAuthException("Needs an associated consumer");
    }
    else {
      $this->consumer = $consumer;
    }

    parent::__construct($key, $secret);
  }

  /**
   * Writes the token to the database
   *
   * @return void
   */
  public function write() {
    $update = !empty($this->tid);

    $primary = $update ? array('tid') : array();

    if ($this->consumer->provider_consumer) {
      $this->changed = REQUEST_TIME;

      $values = array(
        'token_key'  => $this->key,
        'changed'    => $this->changed,
        'services'   => json_encode($this->services),
        'authorized' => $this->authorized,
      );

      if ($update) {
        $values['tid'] = $this->tid;
      }
      else {
        $this->created = REQUEST_TIME;
        $values['created'] = $this->created;
      }

      $ready = drupal_write_record('oauth_common_provider_token', $values, $primary);

      if (!$ready) {
        throw new OAuthException("Couldn't save token");
      }
    }

    $values = array(
      'csid'      => $this->consumer->csid,
      'key_hash'  => sha1($this->key),
      'token_key' => $this->key,
      'secret'    => $this->secret,
      'expires'   => $this->expires,
      'type'      => $this->type,
      'uid'       => $this->uid,
    );

    if ($update) {
      $values['tid'] = $this->tid;
    }

    drupal_write_record('oauth_common_token', $values, $primary);

    $this->tid = $values['tid'];
    $this->in_database = TRUE;

    if (!$update) {
      $values = array(
        'tid'       => $this->tid,
        'token_key' => $this->key,
      );
      drupal_write_record('oauth_common_provider_token', $values, array('token_key'));
    }
  }

  /**
   * Deletes the token from the database
   *
   * @return void
   */
  public function delete() {
    self::deleteToken($this->key, $this->consumer);
  }

  /**
   * Deletes the token with the key from the database
   *
   * @param string $key
   *  The key of the token to delete.
   * @param object $consumer
   *  The consumer for which to fetch a token
   * @return void
   */
  public static function deleteToken($key, $consumer) {
    //TODO: Ensure backwards compatibility
    $condition = db_and()->condition('key_hash', sha1($key))->condition('csid', $consumer->csid);

    db_delete('oauth_common_provider_token')
      ->condition('tid', db_select('oauth_common_token', 't')->condition($condition)->fields('t', array('tid')), 'IN')
      ->execute();

    db_delete('oauth_common_token')
      ->condition($condition)
      ->execute();
  }

  /**
   * Deprecated - Gets the token with the specified key
   *
   * @param string $key
   * The key of the token to get
   * @param bool $provider_token
   * Whether the token to load is a provider token.
   * @return DrupalOAuthToken
   * The loaded token object or FALSE if load failed
   */
  public static function load($key, $provider_token = TRUE) {
    return DrupalOAuthToken::loadByKey($key, !$provider_token, FALSE);
  }

  /**
   * Gets the token with the specified key
   *
   * @param string $key
   *  The key of the token to get
   * @param boolean|object $consumer
   *  The consumer for which to fetch a token or FALSE to fetch a provider token
   * @param int $type
   *  Used internally for backwards compatibility with ::load()
   * @return DrupalOAuthToken
   *  The loaded token object or FALSE if load failed
   */
  public static function loadByKey($key, $consumer = FALSE, $type = OAUTH_COMMON_TOKEN_TYPE_ACCESS) {
    $query = db_select('oauth_common_token', 't');

    $query
      ->condition('t.key_hash', sha1($key))
      ->fields('t');

    // Only add if defined - needed for backwards compatibility with deprecated DrupalOAuthToken::load() from 6.x-3.0-beta3
    if ($type !== FALSE) {
      $query->condition('t.type', $type);
    }

    if (!$consumer || is_object($consumer) && $consumer->provider_consumer) {
      $query->join('oauth_common_provider_token', 'pt', 'pt.tid = t.tid');
      $query->fields('pt', array('created', 'changed', 'services', 'authorized'));
    }

    // Only fetch non-provider tokens - needed for backwards compatibility with deprecated DrupalOAuthToken::load() from 6.x-3.0-beta3
    if ($consumer === TRUE) {
      $query->leftJoin('oauth_common_provider_token', 'pt', 'pt.tid = t.tid');
      $query->isNull('pt.tid');
    }
    else if ($consumer) {
      $query->condition('t.csid', $consumer->csid);
    }

    return self::fromResult($query->execute(), $consumer);
  }

  /**
   * Gets the token with the specified id
   *
   * @param int $id
   *  The id of the token to get
   * @param boolean $load_provider_data
   *  Whether to load provider related data or not
   * @return DrupalOAuthToken
   *  The loaded token object or FALSE if load failed
   */
  public static function loadById($tid, $load_provider_data = TRUE) {
    $query = db_select('oauth_common_token', 't');

    if (is_numeric($tid)) {
      $query
        ->condition('t.tid', $tid)
        ->fields('t');

      if ($load_provider_data) {
        $query->join('oauth_common_provider_token', 'pt', 'pt.tid = t.tid');
        $query->fields('pt', array('created', 'changed', 'services', 'authorized'));
      }

      return self::fromResult($query->execute());
    }
    else {
      return FALSE;
    }
  }

  /**
   * Constructs a token from a db-result resource
   *
   * @param resource $res
   *  A database result resource
   * @return DrupalOAuthToken
   *  The constructed token object or NULL if no rows could be read or construction failed
   */
  public static function fromResult($res, $consumer = FALSE) {
    //TODO: Ensure this works with old inputs?
    if ($data = $res->fetchAssoc()) {
      if (isset($data['services'])) {
        $data['services'] = json_decode($data['services']);
      }
      $data['in_database'] = TRUE;

      if (is_object($consumer) && $consumer->csid == $data['csid']) {
        $token_consumer = $consumer;
      }
      else {
        $token_consumer = DrupalOAuthConsumer::loadById($data['csid'], isset($data['services']));
      }

      return new DrupalOAuthToken($data['token_key'], $data['secret'], $token_consumer, $data);
    }
    return NULL;
  }
}
