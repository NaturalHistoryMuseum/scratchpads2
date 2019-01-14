<?php
/**
 * @file
 * Examples and test fodder for migration from Oracle sources. To use this
 *   example
 * (and to run the corresponding tests) you must define a connection to an
 *   Oracle database in your settings.php. E.g.,
 *
 * $conf['oracle_db'] = array(
 *   'username' => 'DRUPAL',
 *   'password' => 'DRUPAL',
 *   'connection_string' => '//oracledb/orcl',
 * );
 *
 * The username must have the CREATE TABLE privilege, so test data can be
 *   stored for the example to import.
 *
 * See http://us.php.net/manual/en/function.oci-connect.php for more
 *   information on connection_string.
 */

/**
 * Migration class to test importing from Oracle into nodes.
 */
class MigrateExampleOracleNode extends Migration {

  public function __construct() {
    parent::__construct();
    $this->description = t('Example migration from Oracle into nodes.');

    // Note that Oracle by default upper-cases all identifiers, so use upper-case
    // for the key name and for source fields below.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'OID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Content ID',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Source fields available from the Oracle table.
    $fields = array(
      'OID' => t('Source id'),
      'TITLE' => t('Title'),
      'BODY' => t('Description'),
      'MAINIMAGE' => t('Main image'),
      'CREATED' => t('Creation date'),
      'UPDATED' => t('Updated date'),
    );

    // Oracle will usually (depending on server configuration) return only a
    // date (such as 01-MAY-11) for datetime fields - you need to use TO_CHAR()
    // to extract time information as well.
    $query = "SELECT OID, TITLE, BODY, MAINIMAGE, TO_CHAR(CREATED, 'yyyy/mm/dd hh24:mi:ss') CREATED,
                TO_CHAR(UPDATED, 'yyyy/mm/dd hh24:mi:ss') UPDATED
              FROM ORACLE_CONTENT";
    $count_query = "SELECT COUNT(*) FROM ORACLE_CONTENT";

    // Per above, the connection info should be defined in settings.php.
    global $conf;
    $this->source = new MigrateSourceOracle($conf['oracle_db'], $query,
      $count_query, $fields);
    $this->destination = new MigrateDestinationNode('migrate_example_oracle');

    // Basic fields
    $this->addFieldMapping('title', 'TITLE');
    $this->addFieldMapping('uid')
      ->defaultValue(1);
    $this->addFieldMapping('body', 'BODY');
    $this->addFieldMapping('field_mainimage', 'MAINIMAGE')
      ->description('An image blob in the DB')
      ->arguments(array(
        'file_function' => 'file_blob',
        // Alternatively, specify a column here for dynamic file name.
        'source_path' => 'druplicon.png',

      ));
    $this->addFieldMapping('created', 'CREATED');
    $this->addFieldMapping('changed', 'UPDATED');


    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'is_new',
      'status',
      'promote',
      'revision',
      'language',
      'sticky',
      'revision_uid',
      'path',
    ));
  }
}

/*
 * Implementation of hook_migrate_api().
 */
function migrate_example_oracle_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'MigrateExampleOracle' => array('class_name' => 'MigrateExampleOracleNode'),
    ),
  );
  return $api;
}
