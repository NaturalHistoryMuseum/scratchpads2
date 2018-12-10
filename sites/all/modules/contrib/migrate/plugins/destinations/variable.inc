<?php

/**
 * @file
 * Support for variable destinations.
 */

/**
 * Destination class implementing migration into {variable}.
 */
class MigrateDestinationVariable extends MigrateDestination {

  static public function getKeySchema() {
    return array(
      'name' => array(
        'description' => 'The name of the variable.',
        'type' => 'varchar',
        'length' => 128,
        'not null' => TRUE,
        'default' => '',
      ),
    );
  }

  public function __construct() {
    parent::__construct();
  }

  public function __toString() {
    $output = t('Variable');
    return $output;
  }

  /**
   * Returns a list of fields available to be mapped for variables.
   *
   * @param Migration $migration
   *   Optionally, the migration containing this destination.
   *
   * @return array
   *   Keys: machine names of the fields (to be passed to addFieldMapping)
   *   Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array(
      'name' => t('The name of the variable.'),
      'value' => t('The value of the variable.'),
    );
    return $fields;
  }

  /**
   * Import a single row.
   *
   * @param $variable
   *   Variable object to build. Prefilled with any fields mapped in the
   *   Migration.
   * @param $row
   *   Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *   Array of key fields of the object that was saved if
   *   successful. FALSE on failure.
   */
  public function import(stdClass $variable, stdClass $row) {
    // Invoke migration prepare handlers
    $this->prepare($variable, $row);

    // Check to see if this is a new variable.
    $update = FALSE;
    // We cannot just check against NULL because a variable might actually be
    // set to NULL. Attempt to use a unique variable default value that nothing
    // else would use.
    $default = 'migrate:' . REQUEST_TIME . ':' . drupal_random_key();
    if (variable_get($variable->name, $default) !== $default) {
      $update = TRUE;
    }

    // variable_set() provides no return callback, so we can't really test this
    // without running a variable_get() check.
    migrate_instrument_start('variable_set');
    variable_set($variable->name, $variable->value);
    migrate_instrument_stop('variable_set');

    // Return the new id or FALSE on failure.
    if (variable_get($variable->name, $default) === $variable->value) {
      // Increment the count if the save succeeded.
      if ($update) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      // Return the primary key to the mapping table.
      $return = array($variable->name);
    }
    else {
      $return = FALSE;
    }

    // Invoke migration complete handlers.
    $this->complete($variable, $row);

    return $return;
  }

  /**
   * Implementation of MigrateDestination::prepare().
   */
  public function prepare($variable, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $variable->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any general handlers.
    migrate_handler_invoke_all('variable', 'prepare', $variable, $row);
    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($variable, $row);
    }
  }

  /**
   * Implementation of MigrateDestination::complete().
   */
  public function complete($variable, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $variable->migrate = array(
      'machineName' => $migration->getMachineName(),
    );
    // Call any general handlers.
    migrate_handler_invoke_all('variable', 'complete', $variable, $row);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($variable, $row);
    }
  }

  /**
   * Delete a single variable.
   *
   * @param $id
   *   Array of fields representing the key (in this case, just variable name).
   */
  public function rollback(array $id) {
    $name = reset($id);
    migrate_instrument_start('variable_delete');
    $this->prepareRollback($name);
    variable_del($name);
    $this->completeRollback($name);
    migrate_instrument_stop('variable_delete');
  }

  /**
   * Give handlers a shot at cleaning up before a variable has been rolled back.
   *
   * @param $name
   *   The name of the variable about to be deleted.
   */
  public function prepareRollback($name) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('variable', 'prepareRollback', $name);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'prepareRollback')) {
      $migration->prepareRollback($name);
    }
  }

  /**
   * Give handlers a shot at cleaning up after a variable has been rolled back.
   *
   * @param $name
   *   The name of the variable which has been deleted.
   */
  public function completeRollback($name) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('variable', 'completeRollback', $name);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'completeRollback')) {
      $migration->completeRollback($name);
    }
  }
}
