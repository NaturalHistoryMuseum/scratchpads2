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
  }

  protected function vocabularyQuery(){
    return Database::getConnection('default', $this->sourceConnection)->select('vocabulary', 'v')->fields('v', array(
      'vid',
      'name',
      'description',
      'hierarchy',
      'weight'
    ))->condition('module', 'taxonomy');
  }
}