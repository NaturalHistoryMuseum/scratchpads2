<?php

abstract class DrupalVocabularyMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->vocabularyQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($vocabulary){
    if(parent::prepareRow($vocabulary) === FALSE){return FALSE;}
    // Check for collisions of the machine name.  If there is a collision, we
    // don't bother importing the vocabulary (terms from the old site should 
    // probably be imported into the new vocabulary anyway).    
    $machine_name = preg_replace('/[^0-9a-z]/', '', preg_replace('/\ +/', '_', strtolower($vocabulary->name)));
    $result = db_query('SELECT COUNT(*) AS machine_name_exists FROM {taxonomy_vocabulary} WHERE machine_name = :machine_name', array(
      ':machine_name' => $machine_name
    ))->fetch();
    if($result->machine_name_exists){return FALSE;}
    return TRUE;
  }

  /**
   * Implementation of Migration::beginProcess().
   */
  public function beginProcess($newStatus){
    parent::beginProcess($newStatus);
    // Here we move asside the VIDs of existing vocabularies (that haven't been
    // imported by this module).
    //Migration::get
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

  /**
   * Implementation of Migration::endProcess().
   */
  public function endProcess(){
    parent::endProcess();
    // We get the maximum vid below 10000000
    $query = db_select('taxonomy_vocabulary', 't');
    $query->addExpression('MAX(t.vid)', 'max_vid');
    $query->condition('vid', 10000000, '<');
    $max_vid = $query->execute()->fetch();
    $max_vid = $max_vid->max_vid;
    // And then reduce all the VIDs greater than 10000000 making sure there are
    // no collisions.
    db_update('taxonomy_vocabulary')->expression('vid', 'vid - :value_to_reduce', array(
      ':value_to_reduce' => 10000000 - $max_vid
    ))->condition('vid', 10000000, '>')->execute();
    db_update('taxonomy_term_data')->expression('vid', 'vid - :value_to_reduce', array(
      ':value_to_reduce' => 10000000 - $max_vid
    ))->condition('vid', 10000000, '>')->execute();
  }

  /**
   * @abstract
   * Version-specific classes must implement versionQuery, returning a query
   * that provides all the basic fields for each vocabulary.
   */
  abstract protected function vocabularyQuery();
}

class DrupalVocabulary6Migration extends DrupalVocabularyMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'vid' => array(
        'type' => 'int',
        'length' => 255
      )
    ), MigrateDestinationVocabulary6::getKeySchema());
    $this->destination = new MigrateDestinationVocabulary6();
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

/**
 * Destination class implementing migration into vocabularies.
 */
abstract class MigrateDestinationVocabulary extends MigrateDestinationEntity{

  static public function getKeySchema(){
    return array(
      'vid' => array(
        'type' => 'int',
        'description' => 'ID of destination vocabulary'
      )
    );
  }

  public function __construct(array $options = array()){
    parent::__construct('taxonomy_vocabulary', 'taxonomy_vocabulary', $options);
  }

  /**
   * Returns a list of fields available to be mapped for this vocabulary (bundle)
   */
  public function fields(){
    return array(
      'vid' => t('Vocabulary: Existing vocabulary ID'),
      'name' => t('Vocabulary: Name'),
      'description' => t('Vocabulary: Description'),
      'hierarchy' => t('Vocabulary: Hierarchy level'),
      'format' => t('Term: Format'),
      'weight' => t('Term: Weight')
    );
  }

  /**
   * Delete a migrated vocabulary.
   * 
   * FIXME
   */
  public function rollback(array $key){
    taxonomy_vocabulary_delete($key['destid1']);
  }

  /**
   * Import a single vocabulary.
   */
  public function import(stdClass $vocabulary, stdClass $row){
    // Hacked a lot of code from this function that needs replacing.
    $this->prepare($vocabulary, $row);
    // Set the machine name for this entity.  I'm sure there is a more "migrate"
    // way of doing this, but it's not very apparent.
    $vocabulary->machine_name = preg_replace('/[^0-9a-z_]/', '', preg_replace('/\ +/', '_', strtolower($vocabulary->name)));
    // We need to insert a dummy row in to the taxonomy_vocabulary table first
    // as we are specifying the VID of the taxonomy.
    $vocabulary_array = (array)$vocabulary;
    drupal_write_record('taxonomy_vocabulary', $vocabulary_array);
    taxonomy_vocabulary_save($vocabulary);
    return array(
      $vocabulary->vid
    );
  }
}

class MigrateDestinationVocabulary6 extends MigrateDestinationVocabulary{

  public function __construct(array $options = array()){
    parent::__construct($options);
  }
}