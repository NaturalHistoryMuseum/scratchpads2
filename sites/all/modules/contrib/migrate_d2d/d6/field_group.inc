<?php

class DrupalFieldGroup6Migration extends DrupalFieldGroupMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'group_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationFieldGroup6::getKeySchema());
    $this->destination = new MigrateDestinationFieldGroup6();
    // Field mappings
    $this->addSimpleMappings(array(
      'group_name',
      'label',
      'weight',
    	'children'
    ));
    $this->addFieldMapping('bundle', 'type_name');
    $this->addFieldMapping('entity_type')->defaultValue('node');
    // Unmapped source fields
    $this->addFieldMapping(NULL, 'settings'); // Settings are merged into the group object before saving
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function fieldQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('content_group', 'g');
    $query->fields('g', array(
      'type_name',
      'group_name',
      'label',
      'settings',
      'weight'
    ));   
    $query->innerJoin('content_group_fields', 'f', 'f.type_name = g.type_name AND f.group_name = g.group_name');
    $query->groupBy('g.group_name');
    $query->addExpression('GROUP_CONCAT(f.field_name)', 'children');
    $query->condition('g.type_name', scratchpads_migrate_node_types_to_migrate(), 'IN');
    return $query;
  }
}

class MigrateDestinationFieldGroup6 extends MigrateDestinationFieldGroup{

  public function __construct(){
    parent::__construct();
  }

  public function import(stdClass $object, stdClass $row){
    return parent::import($object, $row);
  }
}