<?php

class DrupalOAuthConsumer extends OAuthConsumer {
  public $csid = 0;

  public $uid = 0;
  public $name = '';
  public $context = '';
  public $created = 0;
  public $changed = 0;
  public $callback_url = 'oob';
  public $configuration = array();

  public $provider_consumer = FALSE;
  public $in_database = FALSE;

  function __construct($key, $secret, $params = array()) {
    // Backwards compatibility with 6.x-3.0-beta3
    if (is_string($params)) {
      $callback_url = $params;
      if (func_num_args() > 4) {
        $params = func_get_arg(4);
      }
      else {
        $params = array();
      }
      $params['callback_url'] = $callback_url;
    }

    foreach ($params as $param_key => $value) {
      if (isset($this->$param_key)) {
        $this->$param_key = $value;
      }
    }

    if (!empty($this->created)) {
      $this->provider_consumer = TRUE;
    }

    parent::__construct($key, $secret, $this->callback_url);
  }

  /**
   * Writes the consumer to the database
   *
   * @return void
   */
  public function write() {
    $update = !empty($this->csid);
    $primary = $update ? array('csid') : array();

    if ($this->provider_consumer) {
      $this->changed = REQUEST_TIME;

      $values = array(
        'consumer_key'  => $this->key,
        'created'       => $this->created,
        'changed'       => $this->changed,
        'uid'           => $this->uid,
        'name'          => $this->name,
        'context'       => $this->context,
        'callback_url'  => $this->callback_url,
      );

      if ($update) {
        $values['csid'] = $this->csid;
      }
      else {
        $this->created = REQUEST_TIME;
        $values['created'] = $this->created;
      }

      $ready = drupal_write_record('oauth_common_provider_consumer', $values, $primary);

      if (!$ready) {
        throw new OAuthException("Couldn't save consumer");
      }
    }

    $values = array(
      'key_hash'      => sha1($this->key),
      'consumer_key'  => $this->key,
      'secret'        => $this->secret,
      'configuration' => serialize(empty($this->configuration) ? array() : $this->configuration),
    );

    if ($update) {
      $values['csid'] = $this->csid;
    }

    drupal_write_record('oauth_common_consumer', $values, $primary);

    $this->csid = $values['csid'];
    $this->in_database = TRUE;

    if (!$update) {
      $values = array(
        'csid' => $this->csid,
        'consumer_key' => $this->key,
      );
      drupal_write_record('oauth_common_provider_consumer', $values, array('consumer_key'));
    }
  }

  /**
   * Deletes the consumer from the database
   *
   * @return void
   */
  public function delete() {
    self::deleteConsumer($this->csid);
  }

  /**
   * Deletes the consumer with the id from the database.
   *
   * @param string $csid
   *  The consumer id.
   * @return void
   */
  public static function deleteConsumer($csid) {
    //TODO: Add compatibility layer?
    $condition = db_and()->condition('csid', $csid);

    db_delete('oauth_common_provider_token')
      ->condition('tid', db_select('oauth_common_token', 't')->condition($condition)->fields('t', array('tid')), 'IN')
      ->execute();

    foreach (array('oauth_common_token', 'oauth_common_provider_consumer', 'oauth_common_consumer') as $table) {
      db_delete($table)
        ->condition($condition)
        ->execute();
    }
  }

  /**
   * Deprecated - Gets the consumer with the specified key
   *
   * @param string $key
   *  The key of the consumer to get
   * @param bool $provider_consumer
   *  Optional. Whether the consumer we're about to load is a provider or
   *  consumer consumer. Defaults to TRUE.
   * @return DrupalOAuthConsumer
   *  The loaded consumer object or FALSE if load failed
   */
  public static function load($key, $provider_consumer = TRUE) {
    return DrupalOAuthConsumer::loadProviderByKey($key, $provider_consumer);
  }

  /**
   * Gets a provider consumer with the specified id
   *
   * @param int $id
   *  The id of the consumer to get
   * @param boolean $load_provider_data
   *  Whether to load provider related data or not
   * @return DrupalOAuthConsumer
   *  The loaded consumer object or FALSE if load failed
   */
  public static function loadById($csid, $load_provider_data = TRUE) {
    $query = db_select('oauth_common_consumer', 'c');

    if (is_numeric($csid)) {
      $query
        ->condition('c.csid', $csid)
        ->fields('c', array('csid', 'consumer_key', 'secret', 'configuration'));

      if ($load_provider_data) {
        $query->leftJoin('oauth_common_provider_consumer', 'pc', 'pc.csid = c.csid');
        $query->fields('pc', array('created', 'changed', 'uid', 'name', 'context', 'callback_url'));
      }

      return self::fromResult($query->execute());
    }
    else {
      return FALSE;
    }
  }

  /**
   * Gets a provider consumer with the specified key
   *
   * @param string $key
   *  The key of the consumer to get
   * @param boolean $provider
   *  Used internally for backwards compatibility with ::load()
   * @return DrupalOAuthConsumer
   *  The loaded consumer object or FALSE if load failed
   */
  public static function loadProviderByKey($key, $provider = TRUE) {
    $query = db_select('oauth_common_consumer', 'c');

    $query
      ->condition('c.key_hash', sha1($key))
      ->fields('c', array('secret', 'configuration'));

    if ($provider) {
      $query->join('oauth_common_provider_consumer', 'pc', 'pc.csid = c.csid');
      $query->fields('pc');
    }
    else {
      // For backwards compatibility with deprecated DrupalOAuthConsumer::load() from 6.x-3.0-beta3
      $query->leftJoin('oauth_common_provider_consumer', 'pc', 'pc.csid = c.csid');
      $query
        ->fields('c', array('csid', 'consumer_key'))
        ->fields('pc', array('created', 'changed', 'uid', 'name', 'context', 'callback_url'))
        ->isNull('pc.csid');
    }

    return self::fromResult($query->execute());
  }

  /**
   * Constructs a consumer from a db-result resource
   *
   * @param resource $res
   *  A database result resource
   * @return DrupalOAuthConsumer
   *  The constructed consumer object or NULL if no rows could be read or construction failed
   */
  public static function fromResult($res) {
    //TODO: Ensure this works with old inputs?
    if ($data = $res->fetchAssoc()) {
      if (!empty($data['configuration'])) {
        $data['configuration'] = unserialize($data['configuration']);
      }
      $data['in_database'] = TRUE;
      return new DrupalOAuthConsumer($data['consumer_key'], $data['secret'], $data);
    }
    return NULL;
  }
}
