<?php

class DrupalFieldBodyMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationFieldBody::getKeySchema());
    $this->destination = new MigrateDestinationFieldBody();
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
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_type', 'nt');
    $query->fields('nt', array(
      'type',
      'body_label'
    ));
    // darwincore / location have body fields, but are hidden. Skip them 
    $excluded_node_types = DrupalFieldMigration::excludedNodeTypes();
    // Add a few more node types we know to exclude
    $excluded_node_types[] = 'darwincore';
    $excluded_node_types[] = 'darwincorelocation';
    $excluded_node_types[] = 'page';
    $excluded_node_types[] = 'forum';
    $excluded_node_types[] = 'links';
    $query->condition('nt.type', $excluded_node_types, 'NOT IN');
    $query->condition('nt.has_body', 1);
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Check if this field has a different destination entity type / bundle
    $mapping = DrupalContentTypeMigration::getContentTypeMappings($row->type);
    if($mapping && is_array($mapping) && isset($mapping['bundle'])){
      $row->type = $mapping['bundle'];
    }
    // Does an instance of this field already exists?
    if(field_read_instance('node', 'body', $row->type)){ // Instance already exists, so skip it
      watchdog('scratchpads_migrate', t('Body field already exists for %type'), array(
        '%type' => $row->type
      ), WATCHDOG_NOTICE);
      return false;
    }
  }
}

