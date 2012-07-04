<?php

/**
 * @file
 * Support for menu destinations.
 */

/**
 * Destination class implementing migration into {menu_custom}.
 */
class MigrateDestinationMenu extends MigrateDestination {
  static public function getKeySchema() {
    return array(
      'menu_name' => array(
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
        'description' => 'Primary Key: Unique key for menu. This is used as a block delta so length is 32.',
      ),
    );
  }

  public function __construct() {
    parent::__construct();
  }

  public function __toString() {
    $output = t('Menu');
    return $output;
  }

  /**
   * Returns a list of fields available to be mapped for menus.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array(
      'menu_name' => t('The menu name. Primary key.'),
      'title' => t('The human-readable name of the menu.'),
      'description' => t('A description of the menu'),
    );
    return $fields;
  }

  /**
   * Import a single row.
   *
   * @param $menu
   *  Menu object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $menu, stdClass $row) {
    // Invoke migration prepare handlers
    $this->prepare($menu, $row);

    // Menus are handled as arrays, so clone the object to an array.
    $menu = clone $menu;
    $menu = (array) $menu;

    // Check to see if this is a new menu.
    $update = FALSE;
    if ($data = menu_load($menu['menu_name'])) {
      $update = TRUE;
    }

    // menu_save() provides no return callback, so we can't really test this
    // without running a menu_load() check.
    migrate_instrument_start('menu_save');
    menu_save($menu);
    migrate_instrument_stop('menu_save');

    // Return the new id or FALSE on failure.
    if ($data = menu_load($menu['menu_name'])) {
      // Increment the count if the save succeeded.
      if ($update) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      // Return the primary key to the mapping table.
      $return = array($data['menu_name']);
    }
    else {
      $return = FALSE;
    }

    // Invoke migration complete handlers.
    $menu = (object) $data;
    $this->complete($menu, $row);

    return $return;
  }

  /**
   * Implementation of MigrateDestination::prepare().
   */
  public function prepare($menu, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $menu->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any general handlers.
    migrate_handler_invoke_all('menu', 'prepare', $menu, $row);
    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($menu, $row);
    }
  }

  public function complete($menu, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $menu->migrate = array(
      'machineName' => $migration->getMachineName(),
    );
    // Call any general handlers.
    migrate_handler_invoke_all('menu', 'complete', $menu, $row);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($menu, $row);
    }
  }

  /**
   * Delete a single menu.
   *
   * @param $id
   *  Array of fields representing the key (in this case, just menu_name).
   */
  public function rollback(array $id) {
    $menu_name = reset($id);

    migrate_instrument_start('menu_delete');
    $this->prepareRollback($menu_name);
    if ($menu = menu_load($menu_name)) {
      menu_delete($menu);
    }
    $this->completeRollback($menu_name);
    migrate_instrument_stop('menu_delete');
  }

  /**
   * Give handlers a shot at cleaning up before a menu has been rolled back.
   *
   * @param $menu_name
   *  ID of the menu about to be deleted.
   */
  public function prepareRollback($menu_name) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('menu', 'prepareRollback', $menu_name);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'prepareRollback')) {
      $migration->prepareRollback($menu_name);
    }
  }

  /**
   * Give handlers a shot at cleaning up after a menu has been rolled back.
   *
   * @param $menu_name
   *  ID of the menu which has been deleted.
   */
  public function completeRollback($menu_name) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('menu', 'completeRollback', $menu_name);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'completeRollback')) {
      $migration->completeRollback($menu_name);
    }
  }
}
