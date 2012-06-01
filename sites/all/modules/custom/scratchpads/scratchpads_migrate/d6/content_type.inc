<?php

class DrupalContentType6Migration extends DrupalContentTypeMigration{

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
    ), MigrateDestinationContentType6::getKeySchema());
    $this->destination = new MigrateDestinationContentType6();
    $this->addSimpleMappings(array(
      'type',
      'name',
      'description',
      'title_label',
      'help'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function contentTypeQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_type', 'nt');
    $query->fields('nt', array(
      'type',
      'name',
      'description',
      'title_label',
      'help'
    ));
    $query->innerJoin('node', 'n', 'n.type = nt.type');
    $query->groupBy('nt.type');
    $query->condition('nt.module', 'node');
    $query->condition('nt.type', array_keys(scratchpads_migrate_type_mappings()), 'NOT IN');
    return $query;
  }
}

class MigrateDestinationContentType6 extends MigrateDestinationContentType{

  public function __construct(){
    parent::__construct();
  }

  public function import(stdClass $object, stdClass $row){
    if(!parent::import($object, $row)){return FALSE;}
    // As we're importing from Drupal 6, we always had a body field, so should
    // create one.
    // FIXME - Add body field.
    return array(
      $object->type
    );
  }
}