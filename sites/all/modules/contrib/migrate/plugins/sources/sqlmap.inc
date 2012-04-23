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
   * Qualifying the map table name with the database name and any prefix makes
   * cross-db joins possible. Note this will not work with prefixes - we will
   * omit prepending the database name if we're in the default connection, just
   * so we don't break simpletest.
   *
   * @return string
   */
  public function getQualifiedMapTable() {
    if ($this->connection->getKey() != 'default') {
      $options = $this->connection->getConnectionOptions();
      return $options['database'] . '.' . $this->mapTable;
    }
    else {
      return $this->mapTable;
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

  public function __construct($machine_name, array $source_key,
      array $destination_key, $connection_key = 'default') {
    // Default generated table names, limited to 63 characters
    $this->mapTable = 'migrate_map_' . drupal_strtolower($machine_name);
    $this->mapTable = substr($this->mapTable, 0, 63);
    $this->messageTable = 'migrate_message_' . drupal_strtolower($machine_name);
    $this->messageTable = substr($this->messageTable, 0, 63);
    $this->sourceKey = $source_key;
    $this->destinationKey = $destination_key;
    $this->connection = Database::getConnection('default', $connection_key);
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
          $pks[] = $mapkey;
        }

        $fields = $source_key_schema;

        // Add destination keys to map table
        // TODO: How do we discover the destination schema?
        $count = 1;
        foreach ($this->destinationKey as $field_schema) {
          // Allow dest key fields to be NULL (for IGNORED/FAILED cases)
          $field_schema['not null'] = FALSE;
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
        $fields['last_imported'] = array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'description' => 'UNIX timestamp of the last time this row was imported',
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
    $fields = $this->destinationKeyMap;
    $fields[] = 'needs_update';
    $fields[] = 'last_imported';
    $query = $this->connection->select($this->mapTable, 'map')
              ->fields('map', $fields);
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
    migrate_instrument_start('mapRowByDestination');
    $fields = $this->sourceKeyMap;
    $fields[] = 'needs_update';
    $fields[] = 'last_imported';
    $query = $this->connection->select($this->mapTable, 'map')
              ->fields('map', $fields);
    foreach ($this->destinationKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($destination_id), '=');
    }
    $result = $query->execute();
    migrate_instrument_stop('mapRowByDestination');
    return $result->fetchAssoc();
  }

  /**
   * Retrieve an array of map rows marked as needing update.
   *
   * @param int $count
   *  Maximum rows to return; defaults to 10,000
   * @return array
   *  Array of map row objects with needs_update==1.
   */
  public function getRowsNeedingUpdate($count) {
    $rows = array();
    $result = db_select($this->mapTable, 'map')
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
   * Given a (possibly multi-field) destination key, return the (possibly multi-field)
   * source key mapped to it.
   *
   * @param array $destination_id
   *  Array of destination key values.
   * @return array
   *  Array of source key values, or NULL on failure.
   */
  public function lookupSourceID(array $destination_id) {
    migrate_instrument_start('lookupSourceID');
    $query = $this->connection->select($this->mapTable, 'map')
              ->fields('map', $this->sourceKeyMap);
    foreach ($this->destinationKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($destination_id), '=');
    }
    $result = $query->execute();
    $source_id = $result->fetchAssoc();
    migrate_instrument_stop('lookupSourceID');
    return $source_id;
  }

  /**
   * Given a (possibly multi-field) source key, return the (possibly multi-field)
   * destination key it is mapped to.
   *
   * @param array $source_id
   *  Array of source key values.
   * @param Migration $destination_migration
   *  Unused and deprecated. To be removed at next API update.
   * @return array
   *  Array of destination key values, or NULL on failure.
   */
  public function lookupDestinationID(array $source_id, Migration $destination_migration = NULL) {
    migrate_instrument_start('lookupDestinationID');
    $query = $this->connection->select($this->mapTable, 'map')
              ->fields('map', $this->destinationKeyMap);
    foreach ($this->sourceKeyMap as $key_name) {
      $query = $query->condition("map.$key_name", array_shift($source_id), '=');
    }
    $result = $query->execute();
    $destination_id = $result->fetchAssoc();
    migrate_instrument_stop('lookupDestinationID');
    return $destination_id;
  }

  /**
   * Called upon successful import of one record, we record a mapping from
   * the source key to the destination key. Also may be called, setting the
   * third parameter to NEEDS_UPDATE, to signal an existing record should be remigrated.
   *
   * @param stdClass $source_row
   *  The raw source data. We use the key map derived from the source object
   *  to get the source key values.
   * @param array $dest_ids
   *  The destination key values.
   * @param int $needs_update
   *  Status of the source row in the map. Defaults to STATUS_IMPORTED.
   */
  public function saveIDMapping(stdClass $source_row, array $dest_ids, $needs_update = MigrateMap::STATUS_IMPORTED) {
    migrate_instrument_start('saveIDMapping');
    // Construct the source key
    $keys = array();
    foreach ($this->sourceKeyMap as $field_name => $key_name) {
      // A NULL key value will fail.
      if (is_null($source_row->$field_name)) {
        Migration::displayMessage(t(
          'Could not save to map table due to NULL value for key field !field',
          array('!field' => $field_name)));
        migrate_instrument_stop('saveIDMapping');
        return;
      }
      $keys[$key_name] = $source_row->$field_name;
    }

    $fields = array('needs_update' => (int)$needs_update);
    $count = 1;
    foreach ($dest_ids as $dest_id) {
      $fields['destid' . $count++] = $dest_id;
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
        // If any key value is empty, we can't save - print out and abort
        if (empty($key_value)) {
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
    $query->condition('needs_update', array(MigrateMap::STATUS_IMPORTED, MigrateMap::STATUS_NEEDS_UPDATE), 'IN');
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
   * Delete the map entry and any message table entries for the specified source row.
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
   * Delete the map entry and any message table entries for the specified destination row.
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
   * Implementation of Iterator::rewind() - called before beginning a foreach loop.
   * TODO: Support idlist, itemlimit
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
   * Implementation of Iterator::key - called when entering a loop iteration, returning
   * the key of the current row. It must be a scalar - we will serialize
   * to fulfill the requirement, but using getCurrentKey() is preferable.
   */
  public function key() {
    return serialize($this->currentKey);
  }

  /**
   * Implementation of Iterator::next() - called at the bottom of the loop implicitly,
   * as well as explicitly from rewind().
   */
  public function next() {
    $this->currentRow = $this->result->fetchObject();
    $this->currentKey = array();
    if (!is_object($this->currentRow)) {
      $this->currentRow = NULL;
    }
    else {
      foreach ($this->sourceKeyMap as $map_field) {
        $this->currentKey[$map_field] = $this->currentRow->$map_field;
        // Leave only destination fields
        unset($this->currentRow->$map_field);
      }
    }
  }

  /**
   * Implementation of Iterator::valid() - called at the top of the loop, returning
   * TRUE to process the loop and FALSE to terminate it
   */
  public function valid() {
    // TODO: Check numProcessed against itemlimit
    return !is_null($this->currentRow);
  }
}
