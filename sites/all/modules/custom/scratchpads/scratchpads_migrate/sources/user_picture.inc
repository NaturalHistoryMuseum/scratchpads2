<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class ScratchpadsUserPictureMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->description = t('Migrate files.');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'picture' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => TRUE,
        'description' => 'Image URI.'
      )
    ), MigrateDestinationFile::getKeySchema());
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->destination = new MigrateDestinationFile('file', 'MigrateRemoteFileUri');
    $this->addUnmigratedDestinations(array(
      'fid',
      'path',
      'destination_dir',
      'destination_file',
      'source_dir',
      'filename',
      'uid',
      'timestamp'
    ));
    $this->addFieldMapping('value', 'picture');
    $this->addFieldMapping('file_replace')->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('preserve_files')->defaultValue(true);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('users', 'u');
    $query->fields('u', array(
      'picture'
    ));
    $query->condition('picture', '', '!=');
    return $query;
  }
}