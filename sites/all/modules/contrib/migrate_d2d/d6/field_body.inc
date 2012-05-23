<?php

class DrupalFieldBody6Migration extends DrupalFieldBodyMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationFieldBody6::getKeySchema());
    $this->destination = new MigrateDestinationFieldBody6();
    $this->addSimpleMappings(array(
      'body_label',
      'type'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function fieldQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_type', 'nt');
    $query->fields('nt', array(
      'type',
      'body_label'
    ));
    $query->condition('nt.type', DrupalField6Migration::excludedNodeTypes(), 'NOT IN');
    $query->condition('nt.has_body', 1);
    return $query;
  }
}

class MigrateDestinationFieldBody6 extends MigrateDestinationFieldBody{

  public function __construct(){
    parent::__construct();
  }

  public function import(stdClass $object, stdClass $row){
    return parent::import($object, $row);
  }
}

