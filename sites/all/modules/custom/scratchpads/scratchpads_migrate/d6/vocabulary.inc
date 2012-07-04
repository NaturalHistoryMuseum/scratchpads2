<?php

class DrupalVocabulary6Migration extends DrupalVocabularyMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'vid' => array(
        'type' => 'int',
        'length' => 255
      )
    ), MigrateDestinationVocabulary::getKeySchema());
    $this->destination = new MigrateDestinationVocabulary();
    $this->addSimpleMappings(array(
      'vid',
      'name',
      'description',
      'hierarchy',
      'weight'
    ));
    $this->addFieldMapping(NULL, 'classification_type');
    $this->addFieldMapping(NULL, 'module');
  }

  protected function vocabularyQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('vocabulary', 'v');
    $query->fields('v', array(
      'vid',
      'name',
      'description',
      'hierarchy',
      'weight',
      'module'
    ));
    $query->condition('module', array(
      'forum',
      'scratchpadify',
    	'taxonomy'
    ), 'IN');
    // We're not migrating image galleries vocabulary
    $query->condition('name', 'Image Galleries', '!=');
    $query->leftJoin('scratchpads_migrate_biological_classification', 'mbc', 'v.vid = mbc.vid');
    $query->addField('mbc', 'type', 'classification_type');
    return $query;
  }
}