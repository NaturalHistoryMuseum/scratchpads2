<?php

class DrupalViewsMigration extends DrupalMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'vid' => array(
        'type' => 'int',
        'description' => 'ID of destination view'
      )
    ), MigrateDestinationViews::getKeySchema());
    $this->destination = new MigrateDestinationViews();
    // Unmapped destination - need to get human name from the view title
    $this->addFieldMapping('human_name');
    $this->addSimpleMappings(array(
      'name',
      'description',
      'tag',
      'base_table',
      'core'
    ));
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('views_view', 'vv');
    $query->fields('vv');
    return $query;
  }
}