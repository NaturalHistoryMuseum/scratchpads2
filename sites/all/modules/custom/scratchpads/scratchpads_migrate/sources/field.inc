<?php

class DrupalFieldMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    
    $this->sourceFields['entity_type'] = t('Entity type');
    
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'field_name' => array(
        'type' => 'varchar',
        'length' => 255
      ),
      'type_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationField::getKeySchema());
    $this->destination = new MigrateDestinationField();
    // Field mappings
    $this->addSimpleMappings(array(
      'label',
      'description',
      'required',
      'field_name'
    ));
    $this->addFieldMapping('bundle', 'type_name');
    // These are all node entity types
    $this->addFieldMapping('entity_type', 'entity_type')->defaultValue('node');
    $this->addFieldMapping('active', 'active'); // Used to create field
    $this->addFieldMapping('widget_settings', 'widget_settings'); // Merge into field_config_instance.data
    $this->addFieldMapping('global_settings', 'global_settings'); // Merge into field_config.data
    $this->addFieldMapping('display_settings', 'display_settings'); // Merge into field_config_instance.data
    $this->addFieldMapping('weight', 'weight'); // Now part of field_config_instance.data
    $this->addFieldMapping('widget_type', 'widget_type'); // Now part of field_config_instance.data
    $this->addFieldMapping('module', 'module'); // Now part of field_config_instance.data
    $this->addFieldMapping('type', 'type'); // Used to create field
    $this->addFieldMapping('widget_module', 'widget_module'); // Used to create field instance
    // Unmapped source fields
    $this->addUnmigratedSources(array(
      'db_columns',
      'db_storage',
      'locked',
      'multiple',
      'widget_active'
    ));
  }

  /**
   * Query for the basic source query.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
    $query->fields('i');
    $query->innerJoin('content_node_field', 'f', 'f.field_name = i.field_name');
    $query->fields('f');
    $query->condition('i.widget_active', 1);
    $query->condition('i.type_name', $this->excludedNodeTypes(), 'NOT IN');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Unserialize field & widget serttings
    $row->widget_settings = unserialize($row->widget_settings);
    $row->display_settings = unserialize($row->display_settings);
    $row->global_settings = unserialize($row->global_settings);
    // Check if this field has a different destination entity type / bundle
    $mapping = DrupalContentTypeMigration::getContentTypeMappings($row->type_name);
    if($mapping && is_array($mapping)){
      $row->entity_type = $mapping['entity_type'];
      $row->type_name = $mapping['bundle'];
      if(isset($mapping['fields']) && isset($mapping['fields'][$row->field_name])){
        $row->field_name = $mapping['fields'][$row->field_name];
      }
    }else{
      $row->entity_type = 'node';
    }
    // We want all image fields to use the media field
    if($row->type == 'image' || $row->type == 'filefield'){
      $row->field_name = 'field_media';
    }
    // Does an instance of this field already exists?
    if(field_read_instance($row->entity_type, $row->field_name, $row->type_name)){
      watchdog('scratchpads_migrate', t('Skipped field %name - already exists %entity_type:%bundle'), array(
        '%name' => $row->field_name,
        '%entity_type' => $row->entity_type,
        '%bundle' => $row->type_name
      ), WATCHDOG_NOTICE);
      return false; // Instance already exists, so skip it
    }
  }

  /**
   * Get a list of node types not to migrate fields for
   *
   */
  static public function excludedNodeTypes(){
    $node_types = array();
    foreach(DrupalContentTypeMigration::getContentTypeMappings() as $node_type => $mapping){
      if($mapping === MIGRATE_NONE || $mapping === MIGRATE_CONTENT){
        $node_types[] = $node_type;
      }
    }
    return $node_types;
  }
}
