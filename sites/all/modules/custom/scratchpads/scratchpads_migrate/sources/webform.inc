<?php

class ScratchpadsWebformMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'nid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Webform node nid'
      )
    ), MigrateDestinationWebform::getKeySchema());
    $this->destination = new MigrateDestinationWebform();
  }
  
  /**
   * sourceQuery
   */
  protected function sourceQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('webform', 'w')->fields('w', array('nid'));
  }
}