<?php

/**
 * @file
 * Support for tables defined through the Schema API.
 */

/**
 * Destination class implementing migration into a single table defined through
 * the Schema API.
 */
class MigrateDestinationTable extends MigrateDestination {
  /**
   * The schema of the current table.
   *
   * @var array
   */
  protected $schema = NULL;

  /**
   * The name of the current table.
   *
   * @var string
   */
  protected $tableName = NULL;

  public function __construct($table_name) {
    $this->schema = drupal_get_schema($table_name);
    $this->tableName = $table_name;
  }

  static public function getKeySchema($table_name = NULL) {
    if (empty($table_name)) {
      return array();
    }
    $schema = drupal_get_schema($table_name);
    $keys = array();
    foreach ($schema['primary key'] as $primary_key) {
      // We can't have any form of serial fields here, since the mapping table
      // already has it's own.
      $schema['fields'][$primary_key]['auto_increment'] = FALSE;
      if ($schema['fields'][$primary_key]['type'] == 'serial') {
        $schema['fields'][$primary_key]['type'] = 'int';
      }

      $keys[$primary_key] = $schema['fields'][$primary_key];
    }

    return $keys;
  }

  public function __toString() {
    $output = t('Table !name', array('!name' => $this->tableName));
    return $output;
  }

  /**
   * Delete a single row.
   *
   * @param $id
   *  Primary key values.
   */
  public function rollback(array $id) {
    migrate_instrument_start('table rollback');
    $delete = db_delete($this->tableName);
    $keys = array_keys(self::getKeySchema($this->tableName));
    $i = 0;
    foreach ($id as $value) {
      $key = $keys[$i++];
      $delete->condition($key, $value);
    }
    $delete->execute();
    migrate_instrument_stop('table rollback');
  }

  /**
   * Import a single row.
   *
   * @param $entity
   *  Object object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    if (empty($this->schema['primary key'])) {
      throw new MigrateException(t("The destination table has no primary key defined."));
    }

    // Only filled when doing an update.
    $primary_key = array();

    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      $i = 1;
      foreach ($this->schema['primary key'] as $key) {
        $primary_key[] = $key;
        $destination_id = $row->{'migrate_map_destid' . $i};
        if (isset($entity->{$key})) {
          if ($entity->{$key} != $destination_id) {
            throw new MigrateException(t("Incoming id !id and map destination id !destid don't match",
              array('!id' => $entity->{$key}, '!destid' => $destination_id)));
          }
        }
        else {
          $entity->{$key} = $destination_id;
        }
        $i++;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      foreach ($this->schema['primary key'] as $key) {
        $primary_key[] = $key;
        if (!isset($entity->{$key})) {
          throw new MigrateException(t('System-of-record is DESTINATION, but no destination id provided'));
        }
      }

      $select = db_select($this->tableName)
                ->fields($this->tableName);
      foreach ($this->schema['primary key'] as $key) {
        $select->condition($key, $entity->{$key});
      }
      $old_entity = $select->execute()->fetchObject();
      if (empty($old_entity)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but the destination entity does not exist'));
      }
      foreach ($entity as $field => $value) {
        $old_entity->$field = $entity->$field;
      }
      $entity = $old_entity;
    }

    $this->prepare($entity, $row);
    $status = drupal_write_record($this->tableName, $entity, $primary_key);
    $this->complete($entity, $row);

    if ($status) {
      $id = array();
      foreach ($this->schema['primary key'] as $key) {
        $id[] = $entity->{$key};
      }

      // Increment the number of updated or inserted records by checking the
      // result of drupal_write_record.
      ($status == SAVED_NEW) ? $this->numCreated++ : $this->numUpdated++;

      return $id;
    }
  }

  /**
   * Returns a list of fields available to be mapped.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    foreach ($this->schema['fields'] as $column => $schema) {
      $fields[$column] = t('Type: !type', array('!type' => $schema['type']));
    }
    return $fields;
  }

  /**
   * Give handlers a shot at modifying the object before saving it.
   *
   * @param $entity
   *  Entity object to build. Prefilled with any fields mapped in the Migration.
   * @param $source_row
   *  Raw source data object - passed through to prepare handlers.
   */
  public function prepare($entity, stdClass $source_row) {
    $migration = Migration::currentMigration();
    $entity->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($entity, $source_row);
    }
  }

  /**
   * Give handlers a shot at modifying the object (or taking additional action)
   * after saving it.
   *
   * @param $object
   *  Entity object to build. This is the complete object after saving.
   * @param $source_row
   *  Raw source data object - passed through to complete handlers.
   */
  public function complete($entity, stdClass $source_row) {
    $migration = Migration::currentMigration();

    // Call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($entity, $source_row);
    }
  }
}
