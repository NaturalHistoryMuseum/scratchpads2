<?php

/**
 * @file
 * Defines a Drupal db-based implementation of MigrateMap.
 */

class MigrateSQLMap extends MigrateMap {

  /**
   * Names of tables created for tracking the migration.
   *
   * @var string
   */
  protected $mapTable, $messageTable;

  public function getMapTable() {
    return $this->mapTable;
  }

  public function getMessageTable() {
    return $this->messageTable;
  }

  /**
   * Qualifying the map table name with the database name makes cross-db joins
   * possible. Note that, because prefixes are applied after we do this (i.e.,
   * it will prefix the string we return), we do not qualify the table if it has
   * a prefix. This will work fine when the source data is in the default
   * (prefixed) database (in particular, for simpletest), but not if the primary
   * query is in an external database.
   *
   * @return string
   */
  public function getQualifiedMapTable() {
    $options = $this->connection->getConnectionOptions();
    $prefix = $this->connection->tablePrefix($this->mapTable);
    if ($prefix) {
      return $this->mapTable;
    }
    else {
      $dbname = $options['database'];

      if (!empty($options['driver']) && $options['driver'] == 'mysql') {
        // Backtick the db name on mysql to ensure dbs with hyphens work.
        $dbname = "`{$dbname}`";
      }
      return $dbname . '.' . $this->mapTable;
    }
  }

  /**
   * sourceKey and destinationKey arrays are keyed by the field names; values
   * are the Drupal schema definition for the field.
   *
   * @var array
   */
  public function getSourceKey() {
    return $this->sourceKey;
  }

  public function getDestinationKey() {
    return $this->destinationKey;
  }

  /**
   * Drupal connection object on which to create the map/message tables
   *
   * @var DatabaseConnection
   */
  protected $connection;

  public function getConnection() {
    return $this->connection;
  }

  /**
   * We don't need to check the tables more than once per request.
   *
   * @var boolean
   */
  protected $ensured;

  /**
   * Provide caching for Source or Desination Map Lookups.
   */
  protected $cacheMapLookups;

  /**
   * Constructor.
   *
   * @param string $machine_name
   *   The unique reference to the migration that we are mapping.
   * @param array $source_key
   *   The database schema for the source key.
   * @param array $destination_key
   *   The database schema for the destination key.
   * @param string $connection_key
   *   Optional - The connection used to create the mapping tables. By default
   *   this is the destination (Drupal). If it's not possible to make joins
   *   between the destination database and your source database you can specify
   *   a different connection to create the mapping tables on.
   * @param array $options
   *   Optional - Options applied to this source.
   */
  public function __construct($machine_name, array $source_key,
                              array $destination_key, $connection_key = 'default', $options = array()) {
    if (isset($options['track_last_imported'])) {
      $this->trackLastImported = TRUE;
    }

    if (isset($options['cache_map_lookups'])) {
      $this->cacheMapLookups = $options['cache_map_lookups'];
    }
    else {
      $this->cacheMapLookups = FALSE;
    }

    $this->connection = Database::getConnection('default', $connection_key);

    // Default generated table names, limited to 63 characters
    $prefixLength = strlen($this->connection->tablePrefix());
    $this->mapTable = 'migrate_map_' . drupal_strtolower($machine_name);
    $this->mapTable = drupal_substr($this->mapTable, 0, 63 - $prefixLength);
    $this->messageTable = 'migrate_message_' . drupal_strtolower($machine_name);
    $this->messageTable = drupal_substr($this->messageTable, 0, 63 - $prefixLength);
    $this->sourceKey = $source_key;
    $this->destinationKey = $destination_key;

    // Build the source and destination key maps
    $this->sourceKeyMap = array();
    $count = 1;
    foreach ($source_key as $field => $schema) {
      $this->sourceKeyMap[$field] = 'sourceid' . $count++;
    }
    $this->destinationKeyMap = array();
    $count = 1;
    foreach ($destination_key as $field => $schema) {
      $this->destinationKeyMap[$field] = 'destid' . $count++;
    }
    $this->ensureTables();
  }

  /**
   * Create the map and message tables if they don't already exist.
   */
  protected function ensureTables() {
    if (!$this->ensured) {
      if (!$this->connection->schema()->tableExists($this->mapTable)) {
        // Generate appropriate schema info for the map and message tables,
        // and map from the source field names to the map/msg field names
        $count = 1;
        $source_key_schema = array();
        $pks = array();
        foreach ($this->sourceKey as $field_schema) {
          $mapkey = 'sourceid' . $count++;
          $source_key_schema[$mapkey] = $field_schema;
          $source_key_schema[$mapkey]['not null'] = TRUE;
          $pks[] = $mapkey;
        }

        $fields = $source_key_schema;

        // Add destination keys to map table
        // TODO: How do we discover the destination schema?
        $count = 1;
        foreach ($this->destinationKey as $field_schema) {
          // Allow dest key fields to be NULL (for IGNORED/FAILED cases)
          $field_schema['not null'] = FALSE;
          $field_schema['default'] = NULL;
          $mapkey = 'destid' . $count++;
          $fields[$mapkey] = $field_schema;
        }
        $fields['needs_update'] = array(
          'type' => 'int',
          'size' => 'tiny',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => MigrateMap::STATUS_IMPORTED,
          'description' => 'Indicates current status of the source row',
        );
        $fields['rollback_action'] = array(
          'type' => 'int',
          'size' => 'tiny',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => MigrateMap::ROLLBACK_DELETE,
          'description' => 'Flag indicating what to do for this item on rollback',
        );
        $fields['last_imported'] = array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'description' => 'UNIX timestamp of the last time this row was imported',
        );
        $fields['hash'] = array(
          'type' => 'varchar',
          'length' => '32',
          'not null' => FALSE,
          'description' => 'Hash of source row data, for detecting changes',
        );
        $schema = array(
          'description' => t('Mappings from source key to destination key'),
          'fields' => $fields,
          'primary key' => $pks,
        );
        $this->connection->schema()->createTable($this->mapTable, $schema);

        // Now for the message table
        $fields = array();
        $fields['msgid'] = array(
          'type' => 'serial',
          'unsigned' => TRUE,
          'not null' => TRUE,
        );
        $fields += $source_key_schema;

        $fields['level'] = array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 1,
        );
        $fields['message'] = array(
          'type' => 'text',
          'size' => 'medium',
          'not null' => TRUE,
        );
        $schema = array(
          'description' => t('Messages generated during a migration process'),
          'fields' => $fields,
          'primary key' => array('msgid'),
          'indexes' => array('sourcekey' => $pks),
        );
        $this->connection->schema()->createTable($this->messageTable, $schema);
      }
      else {
        // Add any missing columns to the map table
        if (!$this->connection->schema()->fieldExists($this->mapTable,
          'rollback_action')) {
          $this->connection->schema()->addField($this->mapTable,
            'rollback_action', array(
              'type' => 'int',
              'size' => 'tiny',
              'unsigned' => TRUE,
              'not null' => TRUE,
              'default' => 0,
              'description' => 'Flag indicating what to do for this item on rollback',
            ));
        }
        if (!$this->connection->schema()
          ->fieldExists($this->mapTable, 'hash')) {
          $this->connection->schema()->addField($this->mapTable, 'hash', array(
            'type' => 'varchar',
            'length' => '32',
            'not null' => FALSE,
            'description' => 'Hash of source row data, for detecting changes',
          ));
        }
      }
      $this->ensured = TRUE;
    }
  }

  /**
   * Retrieve a row from the map table, given a source ID
   *
   * @param array $source_id
   */
  public function getRowBySource(array $source_id) {
    migrate_instrument_start('mapRowBySource');
    $query = $this->connection->select($this->mapTable, 'map')
      ->fields('map');
    foreach ($this->sourceKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($source_id), '=');
    }
    $result = $query->execute();
    migrate_instrument_stop('mapRowBySource');
    return $result->fetchAssoc();
  }

  /**
   * Retrieve a row from the map table, given a destination ID
   *
   * @param array $source_id
   */
  public function getRowByDestination(array $destination_id) {
    migrate_instrument_start('getRowByDestination');
    $query = $this->connection->select($this->mapTable, 'map')
      ->fields('map');
    foreach ($this->destinationKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($destination_id), '=');
    }
    $result = $query->execute();
    migrate_instrument_stop('getRowByDestination');
    return $result->fetchAssoc();
  }

  /**
   * Retrieve an array of map rows marked as needing update.
   *
   * @param int $count
   *  Maximum rows to return; defaults to 10,000
   *
   * @return array
   *  Array of map row objects with needs_update==1.
   */
  public function getRowsNeedingUpdate($count) {
    $rows = array();
    $result = $this->connection->select($this->mapTable, 'map')
      ->fields('map')
      ->condition('needs_update', MigrateMap::STATUS_NEEDS_UPDATE)
      ->range(0, $count)
      ->execute();
    foreach ($result as $row) {
      $rows[] = $row;
    }
    return $rows;
  }

  /**
   * Given a (possibly multi-field) destination key, return the (possibly
   * multi-field) source key mapped to it.
   *
   * @param array $destination_id
   *  Array of destination key values.
   *
   * @return array
   *  Array of source key values, or NULL on failure.
   */
  public function lookupSourceID(array $destination_id) {
    migrate_instrument_start('lookupSourceID');
    // Try a cache lookup if enabled.
    if ($this->cacheMapLookups) {
      $cache = &drupal_static($this->mapTable . '_sourceIDCache');
      $serialized = json_encode($destination_id);
      if (isset($cache[$serialized])) {
        migrate_instrument_stop('lookupSourceID');
        return $cache[$serialized];
      }
    }

    $query = $this->connection->select($this->mapTable, 'map')
      ->fields('map', $this->sourceKeyMap);
    foreach ($this->destinationKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($destination_id), '=');
    }
    $result = $query->execute();
    $source_id = $result->fetchAssoc();

    // Store the id in a cache if enabled.
    if ($this->cacheMapLookups) {
      $cache[$serialized] = $destination_id;
    }

    migrate_instrument_stop('lookupSourceID');
    return $source_id;
  }

  /**
   * Given a (possibly multi-field) source key, return the (possibly
   * multi-field) destination key it is mapped to.
   *
   * @param array $source_id
   *  Array of source key values.
   *
   * @return array
   *  Array of destination key values, or NULL on failure.
   */
  public function lookupDestinationID(array $source_id) {
    migrate_instrument_start('lookupDestinationID');
    // Try a cache lookup if enabled.
    if ($this->cacheMapLookups) {
      $cache = &drupal_static($this->mapTable . '_destinationIDCache');
      $serialized = json_encode($source_id);
      if (isset($cache[$serialized])) {
        migrate_instrument_stop('lookupDestinationID');
        return $cache[$serialized];
      }
    }

    $query = $this->connection->select($this->mapTable, 'map')
      ->fields('map', $this->destinationKeyMap);
    foreach ($this->sourceKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($source_id), '=');
    }
    $result = $query->execute();
    $destination_id = $result->fetchAssoc();

    // Store the id in a cache if enabled.
    if ($this->cacheMapLookups) {
      $cache[$serialized] = $destination_id;
    }

    migrate_instrument_stop('lookupDestinationID');
    return $destination_id;
  }

  /**
   * Called upon import of one record, we record a mapping from the source key
   * to the destination key. Also may be called, setting the third parameter to
   * NEEDS_UPDATE, to signal an existing record should be remigrated.
   *
   * @param stdClass $source_row
   *  The raw source data. We use the key map derived from the source object
   *  to get the source key values.
   * @param array $dest_ids
   *  The destination key values.
   * @param int $needs_update
   *  Status of the source row in the map. Defaults to STATUS_IMPORTED.
   * @param int $rollback_action
   *  How to handle the destination object on rollback. Defaults to
   *  ROLLBACK_DELETE.
   * $param string $hash
   *  If hashing is enabled, the hash of the raw source row.
   */
  public function saveIDMapping(stdClass $source_row, array $dest_ids,
                                $needs_update = MigrateMap::STATUS_IMPORTED,
                                $rollback_action = MigrateMap::ROLLBACK_DELETE, $hash = NULL) {
    migrate_instrument_start('saveIDMapping');
    // Construct the source key
    $keys = array();
    foreach ($this->sourceKeyMap as $field_name => $key_name) {
      // A NULL key value will fail.
      if (is_null($source_row->{$field_name})) {
        Migration::displayMessage(t(
          'Could not save to map table due to NULL value for key field !field',
          array('!field' => $field_name)));
        migrate_instrument_stop('saveIDMapping');
        return;
      }
      $keys[$key_name] = $source_row->{$field_name};
    }

    $fields = array(
      'needs_update' => (int) $needs_update,
      'rollback_action' => (int) $rollback_action,
      'hash' => $hash,
    );
    $count = 1;
    if (!empty($dest_ids)) {
      foreach ($dest_ids as $dest_id) {
        $fields['destid' . $count++] = $dest_id;
      }
    }
    if ($this->trackLastImported) {
      $fields['last_imported'] = time();
    }
    $this->connection->merge($this->mapTable)
      ->key($keys)
      ->fields($fields)
      ->execute();
    migrate_instrument_stop('saveIDMapping');
  }

  /**
   * Record a message in the migration's message table.
   *
   * @param array $source_key
   *  Source ID of the record in error
   * @param string $message
   *  The message to record.
   * @param int $level
   *  Optional message severity (defaults to MESSAGE_ERROR).
   */
  public function saveMessage($source_key, $message, $level = Migration::MESSAGE_ERROR) {
    // Source IDs as arguments
    $count = 1;
    if (is_array($source_key)) {
      foreach ($source_key as $key_value) {
        $fields['sourceid' . $count++] = $key_value;
        // If any key value is not set, we can't save - print out and abort
        if (!isset($key_value)) {
          print($message);
          return;
        }
      }
      $fields['level'] = $level;
      $fields['message'] = $message;
      $this->connection->insert($this->messageTable)
        ->fields($fields)
        ->execute();
    }
    else {
      // TODO: What else can we do?
      Migration::displayMessage($message);
    }
  }

  /**
   * Prepares this migration to run as an update - that is, in addition to
   * unmigrated content (source records not in the map table) being imported,
   * previously-migrated content will also be updated in place.
   */
  public function prepareUpdate() {
    $this->connection->update($this->mapTable)
      ->fields(array('needs_update' => MigrateMap::STATUS_NEEDS_UPDATE))
      ->execute();
  }

  /**
   * Returns a count of records in the map table (i.e., the number of
   * source records which have been processed for this migration).
   *
   * @return int
   */
  public function processedCount() {
    $query = $this->connection->select($this->mapTable);
    $query->addExpression('COUNT(*)', 'count');
    $count = $query->execute()->fetchField();
    return $count;
  }

  /**
   * Returns a count of imported records in the map table.
   *
   * @return int
   */
  public function importedCount() {
    $query = $this->connection->select($this->mapTable);
    $query->addExpression('COUNT(*)', 'count');
    $query->condition('needs_update', array(
      MigrateMap::STATUS_IMPORTED,
      MigrateMap::STATUS_NEEDS_UPDATE,
    ), 'IN');
    $count = $query->execute()->fetchField();
    return $count;
  }

  /**
   * Returns a count of records which are marked as needing update.
   *
   * @return int
   */
  public function updateCount() {
    $query = $this->connection->select($this->mapTable);
    $query->addExpression('COUNT(*)', 'count');
    $query->condition('needs_update', MigrateMap::STATUS_NEEDS_UPDATE);
    $count = $query->execute()->fetchField();
    return $count;
  }

  /**
   * Get the number of source records which failed to import.
   *
   * @return int
   *  Number of records errored out.
   */
  public function errorCount() {
    $query = $this->connection->select($this->mapTable);
    $query->addExpression('COUNT(*)', 'count');
    $query->condition('needs_update', MigrateMap::STATUS_FAILED);
    $count = $query->execute()->fetchField();
    return $count;
  }

  /**
   * Get the number of messages saved.
   *
   * @return int
   *  Number of messages.
   */
  public function messageCount() {
    $query = $this->connection->select($this->messageTable);
    $query->addExpression('COUNT(*)', 'count');
    $count = $query->execute()->fetchField();
    return $count;
  }

  /**
   * Delete the map entry and any message table entries for the specified
   * source row.
   *
   * @param array $source_key
   */
  public function delete(array $source_key, $messages_only = FALSE) {
    if (!$messages_only) {
      $map_query = $this->connection->delete($this->mapTable);
    }
    $message_query = $this->connection->delete($this->messageTable);
    $count = 1;
    foreach ($source_key as $key_value) {
      if (!$messages_only) {
        $map_query->condition('sourceid' . $count, $key_value);
      }
      $message_query->condition('sourceid' . $count, $key_value);
      $count++;
    }

    if (!$messages_only) {
      $map_query->execute();
    }
    $message_query->execute();
  }

  /**
   * Delete the map entry and any message table entries for the specified
   * destination row.
   *
   * @param array $destination_key
   */
  public function deleteDestination(array $destination_key) {
    $map_query = $this->connection->delete($this->mapTable);
    $message_query = $this->connection->delete($this->messageTable);
    $source_key = $this->lookupSourceID($destination_key);
    if (!empty($source_key)) {
      $count = 1;
      foreach ($destination_key as $key_value) {
        $map_query->condition('destid' . $count, $key_value);
        $count++;
      }
      $map_query->execute();
      $count = 1;
      foreach ($source_key as $key_value) {
        $message_query->condition('sourceid' . $count, $key_value);
        $count++;
      }
      $message_query->execute();
    }
  }

  /**
   * Set the specified row to be updated, if it exists.
   */
  public function setUpdate(array $source_key) {
    $query = $this->connection->update($this->mapTable)
      ->fields(array('needs_update' => MigrateMap::STATUS_NEEDS_UPDATE));
    $count = 1;
    foreach ($source_key as $key_value) {
      $query->condition('sourceid' . $count++, $key_value);
    }
    $query->execute();
  }

  /**
   * Delete all map and message table entries specified.
   *
   * @param array $source_keys
   *  Each array member is an array of key fields for one source row.
   */
  public function deleteBulk(array $source_keys) {
    // If we have a single-column key, we can shortcut it
    if (count($this->sourceKey) == 1) {
      $sourceids = array();
      foreach ($source_keys as $source_key) {
        $sourceids[] = $source_key;
      }
      $this->connection->delete($this->mapTable)
        ->condition('sourceid1', $sourceids, 'IN')
        ->execute();
      $this->connection->delete($this->messageTable)
        ->condition('sourceid1', $sourceids, 'IN')
        ->execute();
    }
    else {
      foreach ($source_keys as $source_key) {
        $map_query = $this->connection->delete($this->mapTable);
        $message_query = $this->connection->delete($this->messageTable);
        $count = 1;
        foreach ($source_key as $key_value) {
          $map_query->condition('sourceid' . $count, $key_value);
          $message_query->condition('sourceid' . $count++, $key_value);
        }
        $map_query->execute();
        $message_query->execute();
      }
    }
  }

  /**
   * Clear all messages from the message table.
   */
  public function clearMessages() {
    $this->connection->truncate($this->messageTable)
      ->execute();
  }

  /**
   * Remove the associated map and message tables.
   */
  public function destroy() {
    $this->connection->schema()->dropTable($this->mapTable);
    $this->connection->schema()->dropTable($this->messageTable);
  }

  protected $result = NULL;

  protected $currentRow = NULL;

  protected $currentKey = array();

  public function getCurrentKey() {
    return $this->currentKey;
  }

  /**
   * Implementation of Iterator::rewind() - called before beginning a foreach
   * loop. TODO: Support idlist, itemlimit
   */
  public function rewind() {
    $this->currentRow = NULL;
    $fields = array();
    foreach ($this->sourceKeyMap as $field) {
      $fields[] = $field;
    }
    foreach ($this->destinationKeyMap as $field) {
      $fields[] = $field;
    }

    /* TODO
    if (isset($this->options['itemlimit'])) {
      $query = $query->range(0, $this->options['itemlimit']);
    }
    */
    $this->result = $this->connection->select($this->mapTable, 'map')
      ->fields('map', $fields)
      ->execute();
    $this->next();
  }

  /**
   * Implementation of Iterator::current() - called when entering a loop
   * iteration, returning the current row
   */
  public function current() {
    return $this->currentRow;
  }

  /**
   * Implementation of Iterator::key - called when entering a loop iteration,
   * returning the key of the current row. It must be a scalar - we will
   * serialize to fulfill the requirement, but using getCurrentKey() is
   * preferable.
   */
  public function key() {
    return serialize($this->currentKey);
  }

  /**
   * Implementation of Iterator::next() - called at the bottom of the loop
   * implicitly, as well as explicitly from rewind().
   */
  public function next() {
    $this->currentRow = $this->result->fetchObject();
    $this->currentKey = array();
    if (!is_object($this->currentRow)) {
      $this->currentRow = NULL;
    }
    else {
      foreach ($this->sourceKeyMap as $map_field) {
        $this->currentKey[$map_field] = $this->currentRow->{$map_field};
        // Leave only destination fields
        unset($this->currentRow->{$map_field});
      }
    }
  }

  /**
   * Implementation of Iterator::valid() - called at the top of the loop,
   * returning TRUE to process the loop and FALSE to terminate it
   */
  public function valid() {
    // TODO: Check numProcessed against itemlimit
    return !is_null($this->currentRow);
  }
}
