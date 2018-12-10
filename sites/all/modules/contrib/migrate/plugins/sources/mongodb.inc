<?php

/**
 * @file
 * Define a MigrateSource for importing from MongoDB connections
 */

/**
 * Implementation of MigrateSource, to handle imports from MongoDB connections.
 */
class MigrateSourceMongoDB extends MigrateSource {

  /**
   * The mongodb collection object.
   *
   * @var MongoCollection
   */
  protected $collection;

  /**
   * The mongodb cursor object.
   *
   * @var MongoCursor
   */
  protected $cursor;

  /**
   * The mongodb query.
   *
   * @var array
   */
  protected $query;

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Simple initialization.
   */
  public function __construct(MongoCollection $collection, array $query,
                              array $fields = array(), array $sort = array('_id' => 1),
                              array $options = array()) {
    parent::__construct($options);

    $this->collection = $collection;
    $this->query = $query;
    $this->sort = $sort;
    $this->fields = $fields;
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    // The fields are passed to the constructor for this plugin.
    return $this->fields;
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    return $this->cursor->count(TRUE);
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * @return object
   */
  public function getNextRow() {
    $row = $this->cursor->getNext();

    if ($row) {
      return (object) $row;
    }

    return NULL;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   *
   * @return void
   */
  public function performRewind() {
    $keys = $this->getSourceKeyNameAndType();

    // If we have an existing idlist we use it.
    if ($this->idList) {
      foreach ($this->idList as $key => $id) {
        // Try make new ObjectID.
        $this->idList[$key] = $this->getMongoId($id, $keys);
      }

      $this->query[$keys[0]['name']]['$in'] = $this->idList;
    }

    migrate_instrument_start('MigrateSourceMongoDB execute');
    try {
      $this->cursor = $this->collection
        ->find($this->query)
        ->sort($this->sort);
      $this->cursor->timeout(-1);
    } catch (MongoCursorException $e) {
      Migration::displayMessage($e->getMessage(), 'error');
    }
    migrate_instrument_stop('MigrateSourceMongoDB execute');
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    if (is_null($this->cursor)) {
      $this->cursor = $this->collection
        ->find($this->query)
        ->sort($this->sort);
      $this->cursor->timeout(-1);
    }

    $query_info = $this->cursor->info();

    $query = 'query:    ' . drupal_json_encode($query_info['query']['$query']);
    $sort = 'order by: ' . drupal_json_encode($query_info['query']['$orderby']);
    $fields = 'fields:   ' . drupal_json_encode($query_info['fields']);

    return $query . PHP_EOL .
      $sort . PHP_EOL .
      $fields . PHP_EOL;
  }

  /**
   * Check if given document id is a mongo ObjectId and return mongo ObjectId
   * or simple value.
   *
   * @param mixed $document_id
   *   Document key value.
   * @param array $keys
   *   List of keys.
   *
   * @return type
   */
  public function getMongoId($document_id, $keys) {
    if ($keys[0]['name'] != '_id') {
      switch ($keys[0]['type']) {
        case 'int':
          return (int) $document_id;
          break;
        default:
          return $document_id;
      }
    }

    // Trying create Mongo ObjectId
    $mongoid = new MongoId($document_id);

    // If (string) $mongoid == $document_id we return $mongoid object
    if ((string) $mongoid == $document_id) {
      return $mongoid;
    }

    return $document_id;
  }

  /**
   * Get source keys array.
   */
  public function getSourceKeyNameAndType() {
    // Get the key name, and type.
    $keys = array();
    foreach ($this->activeMap->getSourceKey() as $field_name => $field_schema) {
      $keys[] = array(
        'name' => $field_name,
        'type' => $field_schema['type'],
      );
    }

    return $keys;
  }
}
