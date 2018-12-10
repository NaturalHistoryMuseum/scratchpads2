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
   * Load the JSON at the given URL, and return an array of the IDs found
   * within it.
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
      if ($data !== NULL) {
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
        $count = count($this->getIDsFromJSON($data));
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

  public function __construct($item_url, $http_options = array()) {
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
      $message = t('Loading of !objecturl failed:', array('!objecturl' => $item_url));
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
    return preg_replace(array_fill(0, count($id), '/:id/'), $id, $this->itemUrl, 1);
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

/**
 * An iterator over a JSON file. As is, this assumes that the file is structured
 * as an array of objects, e.g.:
 *  [{"id":"53","field1":"value1"},{"id":"54","field1":"value2"}]
 * To deal with different structures, extend this class and override next().
 */
class MigrateJSONReader implements Iterator {

  /**
   * URL of the source JSON file.
   *
   * @var string
   */
  public $url;

  /**
   * Handle of the JSON file we're currently parsing.
   *
   * @var resource
   */
  protected $fileHandle;

  /**
   * Current element object when iterating.
   *
   * @var
   */
  protected $currentElement = NULL;

  /**
   * Value of the ID for the current element when iterating.
   *
   * @var string
   */
  protected $currentId = NULL;

  /**
   * Initialize the members.
   *
   * @param $json_url
   *  URL or filespec of the JSON file to be parsed.
   * @param $id_field
   *  Name of the field within each object containing its unique ID.
   */
  public function __construct($json_url, $id_field) {
    $this->url = $json_url;
    $this->idField = $id_field;
  }

  /**
   * Implementation of Iterator::rewind().
   *
   * @return void
   */
  public function rewind() {
    // Close any open file - we open the files lazily in next().
    if ($this->fileHandle) {
      fclose($this->fileHandle);
      $this->fileHandle = NULL;
    }

    // Load the first element.
    $this->next();
  }

  /**
   * Obtain the next non-whitespace character from the JSON file.
   *
   * @return string
   *  The next non-whitespace character, or FALSE on end-of-file.
   */
  protected function getNonBlank() {
    while (($c = fgetc($this->fileHandle)) !== FALSE && trim($c) == '') {
    }
    return $c;
  }

  /**
   * Implementation of Iterator::next().
   *
   * Populates currentElement (the object being retrieved) and currentId (that
   * object's unique identifier) from the specified JSON file. Sets both to
   * NULL at end-of-file. Handles properly-formed JSON, as well as some improper
   * coding (specifically that generated in Ning exports).
   *
   * @return void
   */
  public function next() {
    migrate_instrument_start('MigrateJSONReader::next');

    $this->currentElement = $this->currentId = NULL;

    // Open the file and position it if necessary
    if (!$this->fileHandle) {
      $this->fileHandle = fopen($this->url, 'r');
      if (!$this->fileHandle) {
        Migration::displayMessage(t('Could not open JSON file !url',
          array('!url' => $this->url)));
        return;
      }

      // We're expecting an array of characters, so the first character should be [.
      $char = $this->getNonBlank();
      // Ning exports are wrapped in bogus (), so skip a leading (
      if ($char == '(') {
        $char = $this->getNonBlank();
      }
      if ($char != '[') {
        Migration::displayMessage(t('!url is not a JSON file containing an array of objects',
          array('!url' => $this->url)));
        return;
      }
    }

    // We expect to be positioned either at an object (beginning with {) or
    // the end of the file (we should see a ] indicating this). Or, an
    // object-separating comma, to be skipped. Note that this treats
    // commas as optional between objects, which helps with processing
    // malformed JSON with missing commas (as in Ning exports).
    $c = $this->getNonBlank();
    if ($c == ',') {
      $c = $this->getNonBlank();
    }
    // Ning sometimes emits a ] where there should be a comma.
    elseif ($c == ']') {
      $c = $this->getNonBlank();
      if ($c != '{') {
        $c = NULL;
      }
    }
    // We expect to be at the first character of an object now.
    if ($c == '{') {
      // Start building a JSON string for this object.
      $json = $c;
      // Look for the closing }, ignoring brackets in strings, tracking nested
      // brackets. Watch out for escaped quotes, but also note that \\" is not
      // an escaped quote.
      $depth = 1;
      $in_string = FALSE;
      $in_escape = FALSE;
      while (($c = fgetc($this->fileHandle)) !== FALSE) {
        $json .= $c;
        if ($in_string) {
          // Quietly accept an escaped character
          if ($in_escape) {
            $in_escape = FALSE;
          }
          else {
            switch ($c) {
              // Unescaped " means end of string
              case '"':
                $in_string = FALSE;
                break;
              // Unescaped \\ means start of escape
              case '\\':
                $in_escape = TRUE;
                break;
            }
          }
        }
        else {
          // Outside of strings, recognize {} as depth changes, " as start of
          // string.
          switch ($c) {
            case '{':
              $depth++;
              break;
            case '}':
              $depth--;
              break;
            case '"':
              $in_string = TRUE;
              break;
          }
          // We've found our match, exit the loop.
          if ($depth < 1) {
            break;
          }
        }
      }

      // Turn the JSON string into an object.
      $this->currentElement = json_decode($json);
      $this->currentId = $this->currentElement->{$this->idField};
    }
    else {
      $this->currentElement = NULL;
      $this->currentId = NULL;
    }
    migrate_instrument_stop('MigrateJSONReader::next');
  }

  /**
   * Implementation of Iterator::current().
   *
   * @return null|object
   */
  public function current() {
    return $this->currentElement;
  }

  /**
   * Implementation of Iterator::key().
   *
   * @return null|string
   */
  public function key() {
    return $this->currentId;
  }

  /**
   * Implementation of Iterator::valid().
   *
   * @return bool
   */
  public function valid() {
    return !empty($this->currentElement);
  }
}

/**
 * Implementation of MigrateSource, to handle imports from stand-alone JSON
 * files.
 */
class MigrateSourceJSON extends MigrateSource {

  /**
   * The MigrateJSONReader object serving as a cursor over the JSON source file.
   *
   * @var MigrateJSONReader
   */
  protected $reader;

  /**
   * Name of the field within the JSON object holding the ID value.
   *
   * @var string
   */
  protected $idField;

  /**
   * The source URLs to load JSON from
   *
   * @var array
   */
  protected $sourceUrls = array();

  /**
   * Holds our current position within the $source_urls array
   *
   * @var int
   */
  protected $activeUrl = NULL;

  /**
   * Store the reader class used to query JSON so we can create reader objects
   * on the fly.
   *
   * @var string
   */
  protected $readerClass = '';

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Source constructor.
   *
   * @param string or array $url
   *  URL(s) of the JSON source data.
   * @param string $id_field
   *  Name of the field within the JSON object holding the ID value.
   * @param array $fields
   *  Optional - keys are field names, values are descriptions. Use to override
   *  the default descriptions, or to add additional source fields which the
   *  migration will add via other means (e.g., prepareRow()).
   * @param boolean $options
   *  Options applied to this source. In addition to the standard MigrateSource
   *  options, we support:
   *  - reader_class: The reader class to instantiate for traversing the JSON -
   *    defaults to MigrateJSONReader (any substitutions must be derived from
   *    MigrateJSONReader).
   */
  public function __construct($urls, $id_field, array $fields = array(),
                              array $options = array()) {
    parent::__construct($options);
    $this->idField = $id_field;
    if (empty($options['reader_class'])) {
      $reader_class = 'MigrateJSONReader';
    }
    else {
      $reader_class = $options['reader_class'];
    }

    if (!is_array($urls)) {
      $urls = array($urls);
    }

    $this->sourceUrls = $urls;
    $this->activeUrl = NULL;
    $this->readerClass = $reader_class;
    $this->fields = $fields;
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    // Clump the urls into a string
    // This could cause a problem when using a lot of urls, may need to hash
    $urls = implode(', ', $this->sourceUrls);
    return 'urls = ' . $urls;
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    return $this->fields;
  }

  /**
   * Returns the active Url.
   *
   * @return string
   */
  public function activeUrl() {
    if (isset($this->activeUrl)) {
      return $this->sourceUrls[$this->activeUrl];
    }
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    $count = 0;
    foreach ($this->sourceUrls as $url) {
      $reader = new $this->readerClass($url, $this->idField);
      foreach ($reader as $element) {
        $count++;
      }
    }

    return $count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   */
  public function performRewind() {
    // Set the reader back to the beginning of the file (positioned to the
    // first matching element), then apply our logic to make sure we have the
    // first element fulfilling our logic (idlist/map/prepareRow()).
    $this->activeUrl = NULL;
    if ($this->reader) {
      $this->reader->rewind();
      $this->reader = NULL;
    }
  }

  /**
   * Implementation of MigrationSource::getNextRow().
   *
   * @return stdClass
   *  data for the next row from the JSON source files
   */
  public function getNextRow() {
    migrate_instrument_start('MigrateSourceJSON::next');

    $row = NULL;

    // The reader is lazy loaded, so it may not be defined yet, need to test if set
    if ($this->reader) {
      // attempt to load the next row
      $this->reader->next();
    }

    // Test the reader for a valid row
    if (isset($this->reader) && $this->reader->valid()) {
      $row = $this->reader->current();
    }
    else {
      // The current source is at the end, try to load the next source
      if ($this->getNextSource()) {
        $row = $this->reader->current();
      }
    }

    migrate_instrument_stop('MigrateSourceJSON::next');
    return $row;
  }

  /**
   * Advances the reader to the next source from source_urls
   *
   * @return bool
   *  TRUE if a valid source was loaded
   */
  public function getNextSource() {
    migrate_instrument_start('MigrateSourceJSON::nextSource');

    // Return value
    $status = FALSE;

    while ($this->activeUrl === NULL || (count($this->sourceUrls) - 1) > $this->activeUrl) {
      if (is_null($this->activeUrl)) {
        $this->activeUrl = 0;
      }
      else {
        // Increment the activeUrl so we try to load the next source
        $this->activeUrl = $this->activeUrl + 1;
      }

      $this->reader = new $this->readerClass($this->sourceUrls[$this->activeUrl], $this->idField);
      $this->reader->rewind();

      if ($this->reader->valid()) {
        // We have a valid source
        $status = TRUE;
        break;
      }
    }

    migrate_instrument_stop('MigrateSourceJSON::nextSource');
    return $status;
  }
}
