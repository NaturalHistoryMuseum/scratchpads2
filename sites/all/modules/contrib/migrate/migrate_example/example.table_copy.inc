<?php

/**
 * @file
 * Make a copy of the role table. To use this you must create a table named
 * role_copy with the same structure as role.
 */

class RoleTableMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array();
    $this->description = 'Copy the role table as an example of table_copy plugin.';
    $destination_key = array(
      'rid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
    $query = db_select('role', 'r')->fields('r');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTableCopy('role_copy', $destination_key);

    $this->map = new MigrateSQLMap($this->machineName,
    array('rid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'alias' => 'r',
      )
    ),
    $destination_key
    );
  }
}
