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
    //    $options = array(
    //      'copy_file' => TRUE
    //    );
    $this->destination = new MigrateDestinationFile();
    // Basic fields
    $this->addFieldMapping('uid', 'uid')->sourceMigration('User')->defaultValue(1);
    // Just map the incoming URL to the destination's 'uri'
    $this->addFieldMapping('value', 'filepath');
    $this->addFieldMapping('file_replace')->defaultValue(FILE_EXISTS_ERROR);
    $this->addFieldMapping('preserve_files')->defaultValue(true);
    $this->addSimpleMappings(array(
      'timestamp'
    ));
    $this->addUnmigratedDestinations(array(
      'destination_dir',
      'destination_file',
      'source_dir'
    ));
    $this->addUnmigratedSources(array(
      'filesize',
      'filename',
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
    return $query;
  }

  public function prepareRow($row){
    // Update to use remote file
    // FIXME - Change this for live migrations
    $row->filepath = str_replace('sites/', 'http://', $row->filepath);
  }
}
