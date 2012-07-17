<?php

class DrupalFieldGroupMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'group_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationFieldGroup::getKeySchema());
    $this->destination = new MigrateDestinationFieldGroup();
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
  protected function sourceQuery(){
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
    $query->condition('g.type_name', DrupalFieldMigration::excludedNodeTypes(), 'NOT IN');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    $row->children = explode(',', $row->children);
    // We only want to migrate groups of content types created by migration
    // Existing types have changed so much the field groups are not useful
    $destid = Migration::getInstance('ContentType')->getMap()->lookupDestinationID(array(
      array(
        $row->type_name
      )
    ));
    if($destid){
      return TRUE;
    }else{
      return false;
    }
  }
}