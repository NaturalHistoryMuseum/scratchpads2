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
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Nothing to check (at the moment), so just return TRUE.
    return TRUE;
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
  public function rollback(array $key){}

  /**
   * Import a single vocabulary.
   */
  public function import(stdClass $vocabulary, stdClass $row){
    // Hacked a lot of code from this function that needs replacing.
    $this->prepare($vocabulary, $row);
    // Set the machine name for this entity.  I'm sure there is a more "migrate"
    // way of doing this, but it's not very apparent.
    $vocabulary->machine_name = preg_replace('/\ +/', '_', preg_replace('/[^0-9a-z]/', '', strtolower($vocabulary->name)));
    // We need to insert a dummy row in to the taxonomy_vocabulary table first
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