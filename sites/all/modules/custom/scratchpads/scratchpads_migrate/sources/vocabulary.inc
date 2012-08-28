<?php

class DrupalVocabularyMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->vocabularyQuery(), $this->sourceFields, NULL, $this->sourceOptions);
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
    $this->addFieldMapping('biological_classification', 'classification_type');
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

  public function beginProcess($newStatus){
    parent::beginProcess($newStatus);
    if($newStatus == Migration::STATUS_IMPORTING){
      // Here we move asside the VIDs of existing vocabularies (that haven't been
      // imported by this module).
      $current_migration = Migration::currentMigration();
      $machine_name = $current_migration->getMachineName();
      $results = db_select('migrate_map_' . drupal_strtolower($machine_name), 'm')->fields('m')->execute();
      $vids = array(
        -1
      );
      foreach($results as $row){
        // We need destid1 to ensure we don't change that.
        $vids[] = $row->destid1;
      }
      db_update('taxonomy_vocabulary')->expression('vid', 'vid + 10000000')->condition('vid', $vids, 'NOT IN')->execute();
      db_update('taxonomy_term_data')->expression('vid', 'vid + 10000000')->condition('vid', $vids, 'NOT IN')->execute();
    }
  }

  protected function postImport(){
    drupal_flush_all_caches();
  }
}