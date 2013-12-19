<?php

class DrupalFileMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->description = t('Migrate files.');
    $this->map = new MigrateSQLMap($this->machineName, array(
      'fid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Image ID.'
      )
    ), MigrateDestinationFile::getKeySchema());
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->destination = new MigrateDestinationFile('file', 'MigrateRemoteFileUri');
    // Basic fields
    $this->addFieldMapping('uid', 'uid')->sourceMigration('User')->defaultValue(1);
    // Just map the incoming URL to the destination's 'uri'
    $this->addFieldMapping('value', 'filepath');
    $this->addFieldMapping('file_replace')->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('preserve_files')->defaultValue(true);
    $this->addSimpleMappings(array(
      'timestamp',
      'filename'
    ));
    $this->addUnmigratedDestinations(array(
      'path',
      'destination_dir',
      'destination_file',
      'source_dir'
    ));
    $this->addUnmigratedSources(array(
      'filesize',
      'filemime',
      'status'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('files', 'f');
    $query->fields('f');
    $query->orderBy('fid');
    $query->condition('filemime', DrupalImageMigration::imageFileMime(), 'NOT IN');
    return $query;
  }
}
