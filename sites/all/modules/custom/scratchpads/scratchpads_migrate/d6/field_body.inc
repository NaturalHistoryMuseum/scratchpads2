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
    $excluded_node_types = DrupalField6Migration::excludedNodeTypes();
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
}

