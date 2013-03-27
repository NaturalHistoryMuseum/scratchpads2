<?php

/**
 * Class for migrating backreferences
 */
class DrupalFieldBackreferenceMigration extends DrupalMigration{

  /**
   * Constructor
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'field_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationFieldBackreference::getKeySchema());
    $this->destination = new MigrateDestinationFieldBackreference($this->sourceConnection);
    $this->addSimpleMappings(array(
      'field_name'
    ));
  }

  /**
   * Query for the basic source query.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
    $query->fields('i', array(
      'weight',
      'label',
      'display_settings',
      'type_name'
    ));
    $query->innerJoin('content_node_field', 'f', 'f.field_name = i.field_name');
    $query->fields('f', array(
      'field_name',
      'global_settings',
      'type'
    ));
    $query->condition('f.type', array(
      'noderelationships_backref',
      'nodereferrer'
    ), 'IN');
    return $query;
  }
}

