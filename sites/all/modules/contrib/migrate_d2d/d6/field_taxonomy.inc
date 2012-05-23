<?php

class DrupalFieldTaxonomy6Migration extends DrupalFieldTaxonomyMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'vid' => array(
        'type' => 'int',
        'length' => 255
      )
    ), MigrateDestinationFieldTaxonomy::getKeySchema());
    $this->destination = new MigrateDestinationFieldTaxonomy();
    $this->addSimpleMappings(array(
      'type'
    ));
    $this->addFieldMapping('vid', 'vid')->sourceMigration('Vocabulary');
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function fieldQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('vocabulary_node_types', 'v');
    $query->fields('v');
    $query->condition('v.type', DrupalField6Migration::excludedNodeTypes(), 'NOT IN');
    return $query;
  }
}