<?php
/**
 * @file
 * Examples and test fodder for migration into profile2 entities.
 */

/**
 * Migration class to test import of various date fields.
 */
class MigrateExampleProfile2Migration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration into profile2 entities');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationProfile2::getKeySchema()
    );

    // Our test data is in a CSV file
    $this->source = new MigrateSourceCSV(__DIR__ . '/migrate_extras_profile2.csv', $this->csvcolumns(), array(), $this->fields());
    $this->destination = new MigrateDestinationProfile2('migrate_extras_profile2');

    $this->addFieldMapping('uid')
         ->defaultValue(1);

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('id'));
  }

  function csvcolumns() {
    $columns[0] = array('id', 'Source ID');
    $columns[1] = array('uid', 'User ID');
    return $columns;
  }

  function fields() {
    return array(
      'id' => 'Source ID',
      'uid' => 'User ID',
    );
  }
}
