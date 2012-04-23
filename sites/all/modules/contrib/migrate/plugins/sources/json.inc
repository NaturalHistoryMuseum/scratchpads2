<?php

/**
 * @file
 * Support for migration from JSON sources.
 */

/**
 * Implementation of MigrateList, for retrieving a list of IDs to be migrated
 * from a JSON object.
 */
class MigrateListJSON extends MigrateList {
  /**
   * A URL pointing to an JSON object containing a list of IDs to be processed.
   *
   * @var string
   */
  protected $listUrl;

  protected $httpOptions;

  public function __construct($list_url, $http_options = array()) {
    parent::__construct();
    $this->listUrl = $list_url;
    $this->httpOptions = $http_options;
  }

  /**
   * Our public face is the URL we're getting items from
   *
   * @return string
   */
  public function __toString() {
    return $this->listUrl;
  }

  /**
   * Load the JSON at the given URL, and return an array of the IDs found within it.
   *
   * @return array
   */
  public function getIdList() {
    migrate_instrument_start("Retrieve $this->listUrl");
    if (empty($this->httpOptions)) {
      $json = file_get_contents($this->listUrl);
    }
    else {
      $response = drupal_http_request($this->listUrl, $this->httpOptions);
      $json = $response->data;
    }
    migrate_instrument_stop("Retrieve $this->listUrl");
    if ($json) {
      $data = drupal_json_decode($json);
      if ($data) {
        return $this->getIDsFromJSON($data);
      }
    }
    Migration::displayMessage(t('Loading of !listurl failed:',
        array('!listurl' => $this->listUrl)));
    return NULL;
  }

  /**
   * Given an array generated from JSON, parse out the IDs for processing
   * and return them as an array. The default implementation assumes the IDs are
   * simply the values of the top-level elements - in most cases, you will need
   * to override this to reflect your particular JSON structure.
   *
   * @param array $data
   *
   * @return array
   */
  protected function getIDsFromJSON(array $data) {
    return $data;
  }

  /**
   * Return a count of all available IDs from the source listing. The default
   * implementation assumes the count of top-level elements reflects the number
   * of IDs available - in many cases, you will need to override this to reflect
   * your particular JSON structure.
   */
  public function computeCount() {
    $count = 0;
    if (empty($this->httpOptions)) {
      $json = file_get_contents($this->listUrl);
    }
    else {
      $response = drupal_http_request($this->listUrl, $this->httpOptions);
      $json = $response->data;
    }
    if ($json) {
      $data = drupal_json_decode($json);
      if ($data) {
        $count = count($data);
      }
    }
    return $count;
  }
}

/**
 * Implementation of MigrateItem, for retrieving a parsed JSON object given
 * an ID provided by a MigrateList class.
 */
class MigrateItemJSON extends MigrateItem {
  /**
   * A URL pointing to a JSON object containing the data for one item to be
   * migrated.
   *
   * @var string
   */
  protected $itemUrl;

  protected $httpOptions;

  public function __construct($item_url, $http_options) {
    parent::__construct();
    $this->itemUrl = $item_url;
    $this->httpOptions = $http_options;
  }

  /**
   * Implementors are expected to return an object representing a source item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  public function getItem($id) {
    $item_url = $this->constructItemUrl($id);
    // Get the JSON object at the specified URL
    $json = $this->loadJSONUrl($item_url);
    if ($json) {
      return $json;
    }
    else {
      $migration = Migration::currentMigration();
      $message =  t('Loading of !objecturl failed:', array('!objecturl' => $item_url));
      $migration->getMap()->saveMessage(
        array($id), $message, MigrationBase::MESSAGE_ERROR);
      return NULL;
    }
  }

  /**
   * The default implementation simply replaces the :id token in the URL with
   * the ID obtained from MigrateListJSON. Override if the item URL is not
   * so easily expressed from the ID.
   *
   * @param mixed $id
   */
  protected function constructItemUrl($id) {
    return str_replace(':id', $id, $this->itemUrl);
  }

  /**
   * Default JSON loader - just pull and decode. This can be overridden for
   * preprocessing of JSON (removal of unwanted elements, caching of JSON if the
   * source service is slow, etc.)
   */
  protected function loadJSONUrl($item_url) {
    if (empty($this->httpOptions)) {
      $json = file_get_contents($item_url);
    }
    else {
      $response = drupal_http_request($item_url, $this->httpOptions);
      $json = $response->data;
    }
    return json_decode($json);
  }
}
