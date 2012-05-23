<?php

class DrupalField6Migration extends DrupalFieldMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'field_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationField6::getKeySchema());
    $this->destination = new MigrateDestinationField6();
    // Field mappings
    $this->addSimpleMappings(array(
      'label',
      'description',
      'required',
      'field_name'
    ));
    $this->addFieldMapping('bundle', 'type_name');
    // These are all node entity types
    $this->addFieldMapping('entity_type')->defaultValue('node');
    // Unmapped source fields
    // content_node_field
    $this->addFieldMapping(NULL, 'global_settings'); // Merge into field_config.data
    $this->addFieldMapping(NULL, 'db_columns'); // Merge into field_config.data
    $this->addFieldMapping(NULL, 'db_storage'); // Used to create field
    $this->addFieldMapping(NULL, 'module'); // Used to create field
    $this->addFieldMapping(NULL, 'locked'); // Used to create field
    $this->addFieldMapping(NULL, 'active'); // Used to create field
    $this->addFieldMapping(NULL, 'type'); // Used to create field
    $this->addFieldMapping(NULL, 'multiple'); // Used to create field
    // content_node_field_instance
    $this->addFieldMapping(NULL, 'widget_active'); // Merge into field_config_instance.data
    $this->addFieldMapping(NULL, 'display_settings'); // Merge into field_config_instance.data
    $this->addFieldMapping(NULL, 'widget_settings'); // Merge into field_config_instance.data
    $this->addFieldMapping(NULL, 'weight'); // Now part of field_config_instance.data
    $this->addFieldMapping(NULL, 'widget_module'); // Now part of field_config_instance.data
    $this->addFieldMapping(NULL, 'widget_type'); // Now part of field_config_instance.data
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function fieldQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
    $query->fields('i');
    $query->innerJoin('content_node_field', 'f', 'f.field_name = i.field_name');
    $query->fields('f');
    $query->condition('i.widget_active', 1);
    $query->condition('i.type_name', $this->excludedNodeTypes(), 'NOT IN');
    return $query;
  }

  /**
   * Get a list of node types not to migrate fields for
   *
   */
  public function excludedNodeTypes(){
    $node_types = array();
    foreach(scratchpads_migrate_type_mappings() as $node_type => $mapping){
      if($mapping === MIGRATE_NONE || $mapping === MIGRATE_CONTENT){
        $node_types[] = $node_type;
      }
    }
    return $node_types;
  }
}

class MigrateDestinationField6 extends MigrateDestinationField{

  public function __construct(){
    parent::__construct();
  }

  public function import(stdClass $object, stdClass $row){
    return parent::import($object, $row);
  }
}