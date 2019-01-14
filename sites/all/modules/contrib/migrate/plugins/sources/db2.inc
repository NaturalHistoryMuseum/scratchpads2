<?php

/**
 * @file
 * Define a MigrateSource class for importing from IBM DB2 databases.
 */

/**
 * Implementation of MigrateSource, to handle imports from remote DB2 servers.
 */
class MigrateSourceDB2 extends MigrateSource {

  /**
   * Array containing information for connecting to DB2:
   *  database - Database to connect to
   *  username - Username to connect as
   *  password - Password for authentication
   *
   * @var array
   */
  protected $configuration;

  /**
   * The active DB2 connection for this source.
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
   * The statement resource from executing the query - traversed to process the
   * incoming data.
   */
  protected $stmt;

  /**
   * Return an options array for DB2 sources.
   *
   * @param boolean $cache_counts
   *  Indicates whether to cache counts of source records.
   */
  static public function options($cache_counts = FALSE) {
    return compact('cache_counts');
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
      // Check for the ibm_db2 extension before attempting to connect with it.
      if (!extension_loaded('ibm_db2')) {
        throw new Exception(t('You must configure the ibm_db2 extension in PHP.'));
      }
      // Connect to db2.
      $this->connection = db2_connect($this->configuration['database'],
        $this->configuration['username'], $this->configuration['password']);
    }
    if ($this->connection) {
      return TRUE;
    }
    // If we failed to connect, throw an exception with the connection error
    // message.
    else {
      $e = db2_conn_errormsg();
      throw new Exception($e);
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
    migrate_instrument_start('MigrateSourceDB2 count');
    // Make sure we're connected.
    if ($this->connect()) {
      // Execute the count query.
      $stmt = db2_exec($this->connection, $this->countQuery);
      // If something went wrong, throw an exception with the error message.
      if (!$stmt) {
        $e = db2_stmt_errormsg($stmt);
        throw new Exception($e);
      }
      // Grab the first row as an array.
      $count_array = db2_fetch_array($stmt);
      // The first item in this array will be our count.
      $count = reset($count_array);
    }
    else {
      // Connection failed.
      $count = FALSE;
    }
    migrate_instrument_stop('MigrateSourceDB2 count');
    return $count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   */
  public function performRewind() {
    migrate_instrument_start('db2_query');
    // Ensure we're connected to the database.
    $this->connect();
    // Execute the query.
    $this->stmt = db2_exec($this->connection, $this->query);
    // Throw an exception with the error message if something went wrong.
    if (!$this->stmt) {
      $e = db2_stmt_errormsg($this->stmt);
      throw new Exception($e);
    }
    migrate_instrument_stop('db2_query');
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * @return object
   */
  public function getNextRow() {
    return db2_fetch_object($this->stmt);
  }
}
