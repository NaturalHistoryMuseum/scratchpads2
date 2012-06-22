<?php

class DrupalFieldTaxonomy6Migration extends DrupalFieldTaxonomyMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      ),
      'vid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE
      )
    ), MigrateDestinationFieldTaxonomy::getKeySchema());
    $this->destination = new MigrateDestinationFieldTaxonomy();
    $this->addSimpleMappings(array(
      'name',
      'multiple',
      'required',
      'tags'
    ));
    $this->addFieldMapping('bundle', 'type');
    $this->addFieldMapping('vid', 'vid')->sourceMigration('Vocabulary');
    $this->addFieldMapping(NULL, 'classification_type');
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function fieldQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('vocabulary_node_types', 'vnt');
    $query->fields('vnt');
    $query->innerJoin('vocabulary', 'v', 'v.vid = vnt.vid');
    $query->fields('v', array(
      'name',
      'multiple',
      'required',
      'tags'
    ));
    $query->leftJoin('scratchpads_migrate_biological_classification', 'mbc', 'v.vid = mbc.vid');
    $query->addField('mbc', 'type', 'classification_type');
    $query->condition('vnt.type', array(
      DrupalField6Migration::excludedNodeTypes()
    ), 'NOT IN');
    return $query;
  }
}