<?php

/**
 * @file
 * Define a MigrateSource class for importing from Oracle databases.
 */

/**
 * Implementation of MigrateSource, to handle imports from remote Oracle
 * servers.
 */
class MigrateSourceOracle extends MigrateSource {

  /**
   * Array containing information for connecting to Oracle:
   *  username - Username to connect as
   *  password - Password for logging in
   *  connection_string - See
   * http://us.php.net/manual/en/function.oci-connect.php.
   *
   * @var array
   */
  protected $configuration;

  /**
   * The active Oracle connection for this source.
   *
   * @var resource
   */
  protected $connection;

  public function getConnection() {
    return $this->connection;
  }

  /**
   * The SQL query from which to obtain data. Is a string.
   */
  protected $query;

  /**
   * The result object from executing the query - traversed to process the
   * incoming data.
   */
  protected $result;

  /**
   * Character set to use in retrieving data.
   *
   * @var string
   */
  protected $characterSet;

  /**
   * Return an options array for Oracle sources.
   *
   * @param string $character_set
   *  Character set to use in retrieving data. Defaults to 'UTF8'.
   * @param boolean $cache_counts
   *  Indicates whether to cache counts of source records.
   */
  static public function options($character_set = 'UTF8', $cache_counts = FALSE) {
    return compact('character_set', 'cache_counts');
  }

  /**
   * Simple initialization.
   */
  public function __construct(array $configuration, $query, $count_query,
                              array $fields, array $options = array()) {
    parent::__construct($options);
    $this->query = $query;
    $this->countQuery = $count_query;
    $this->configuration = $configuration;
    $this->fields = $fields;
    if (empty($options['character_set'])) {
      $this->characterSet = 'UTF8';
    }
    else {
      $this->characterSet = $options['character_set'];
    }
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    return $this->query;
  }

  /**
   * Connect lazily to the DB server.
   */
  protected function connect() {
    if (!isset($this->connection)) {
      if (!extension_loaded('oci8')) {
        throw new Exception(t('You must configure the oci8 extension in PHP.'));
      }
      $this->connection = oci_connect($this->configuration['username'],
        $this->configuration['password'], $this->configuration['connection_string'],
        $this->characterSet);
    }
    if ($this->connection) {
      return TRUE;
    }
    else {
      $e = oci_error();
      throw new Exception($e['message']);
      return FALSE;
    }
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
    migrate_instrument_start('MigrateSourceOracle count');
    if ($this->connect()) {
      $statement = oci_parse($this->connection, $this->countQuery);
      if (!$statement) {
        $e = oci_error($this->connection);
        throw new Exception($e['message'] . "\n" . $e['sqltext']);
      }
      $result = oci_execute($statement);
      if (!$result) {
        $e = oci_error($statement);
        throw new Exception($e['message'] . "\n" . $e['sqltext']);
      }
      $count_array = oci_fetch_array($statement);
      $count = reset($count_array);
    }
    else {
      // Do something else?
      $count = FALSE;
    }
    migrate_instrument_stop('MigrateSourceOracle count');
    return $count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   */
  public function performRewind() {
    $keys = array();
    foreach ($this->activeMap->getSourceKey() as $field_name => $field_schema) {
      // Allow caller to provide an alias to table containing the primary key.
      if (!empty($field_schema['alias'])) {
        $field_name = $field_schema['alias'] . '.' . $field_name;
      }
      $keys[] = $field_name;
    }

    /*
     * Replace :criteria placeholder with idlist or highwater clauses. We
     * considered supporting both but it is not worth the complexity. Run twice
     * instead.
     */
    if (!empty($this->idList)) {
      // TODO: Sanitize. not critical as this is admin supplied data in drush.
      $this->query = str_replace(':criteria',
        $keys[0] . ' IN (' . implode(',', $this->idList) . ')', $this->query);
    }
    else {
      if (isset($this->highwaterField['name']) && $highwater = $this->activeMigration->getHighwater()) {
        if (empty($this->highwaterField['alias'])) {
          $highwater_name = $this->highwaterField['name'];
        }
        else {
          $highwater_name = $this->highwaterField['alias'] . '.' . $this->highwaterField['name'];
        }
        $this->query = str_replace(':criteria', "$highwater_name > '$highwater'", $this->query);
      }
      else {
        // No idlist or highwater. Replace :criteria placeholder with harmless WHERE
        // clause instead of empty since we don't know if an AND follows.
        $this->query = str_replace(':criteria', '1=1', $this->query);
      }
    }

    migrate_instrument_start('oracle_query');
    $this->connect();
    $this->result = oci_parse($this->connection, $this->query);
    if (!$this->result) {
      $e = oci_error($this->connection);
      throw new Exception($e['message'] . "\n" . $e['sqltext']);
    }
    $status = oci_execute($this->result);
    if (!$status) {
      $e = oci_error($this->result);
      throw new Exception($e['message'] . "\n" . $e['sqltext']);
    }
    migrate_instrument_stop('oracle_query');
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * Returns the next row of the result set as an object, making sure NULLs are
   * represented as PHP NULLs and that LOBs are returned directly without
   * special handling.
   */
  public function getNextRow() {
    $row = oci_fetch_array($this->result, OCI_ASSOC | OCI_RETURN_NULLS | OCI_RETURN_LOBS);
    if (!empty($row)) {
      return (object) $row;
    }
    else {
      return FALSE;
    }
  }
}
