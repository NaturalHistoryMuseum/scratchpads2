<?php

/**
 * @file
 * Copies data_row into a table using drupal_write_record()
 */

/**
 * Destination class implementing migration into a single table.
 */
class MigrateDestinationTableCopy extends MigrateDestination {

  public function __construct($tableName, $keySchema) {
    parent::__construct();
    $this->tableName = $tableName;
    $this->keySchema = $keySchema;
  }

  public function __toString() {
    $output = t('Table copy');
    return $output;
  }

  /**
   * Delete a batch of rows at once.
   *
   * @param $ids
   *  Array of IDs to be deleted.
   */
  public function bulkRollback(array $ids) {
    migrate_instrument_start('table_copy bulkRollback');
    db_delete($this->tableName)
      ->condition(key($this->keySchema), $ids, 'IN')
      ->execute();
    migrate_instrument_stop('table_copy bulkRollback');
  }

  /**
   * Import a single row.
   *
   * @param $entity
   *  Object object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    $migration = MigrationBase::currentMigration();

    $fields = clone $row;
    // Remove all map data, otherwise we'll try to write it to the destination
    // table.
    foreach ($fields as $field => $data) {
      if (strpos($field, 'migrate_map_') === 0) {
        unset($fields->{$field});
      }
    }
    $keys = array_keys($this->keySchema);
    $values = array();
    foreach ($keys as $key) {
      $values[] = $row->{$key};
    }
    $query = db_merge($this->tableName)
      ->key($keys, $values)
      ->fields((array) $fields);
    try {
      $status = $query->execute();
      if ($status == MergeQuery::STATUS_INSERT) {
        $this->numCreated++;
      }
      else {
        $this->numUpdated++;
      }
      return $values;
    } catch (MigrateException $e) {
      $migration->saveMessage($e->getMessage(), $e->getLevel());
      Migration::displayMessage($e->getMessage());
    } catch (Exception $e) {
      $migration->handleException($e);
    }
  }

  public function fields($migration = NULL) {
    return array();
  }

}
