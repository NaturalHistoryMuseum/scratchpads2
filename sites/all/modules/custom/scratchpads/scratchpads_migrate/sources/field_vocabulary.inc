<?php

class DrupalFieldVocabularyMigration extends DrupalFieldMigration{

	protected $vocabulary;
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->removeFieldMapping('entity_type');
    $this->addFieldMapping('entity_type')->defaultValue('taxonomy_term');
    // What bundle was created for this vocabulary 
    $this->removeFieldMapping('bundle');
    // Load the vocabulary to get the classifcation name to use as bundle
    $this->vocabulary = taxonomy_vocabulary_load($arguments['vid']);
    $this->addFieldMapping('bundle')->defaultValue($this->vocabulary->machine_name);
  }

  /**
   * Query for the basic source query.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
    $query->fields('i');
    $query->innerJoin('content_node_field', 'f', 'f.field_name = i.field_name');
    $query->fields('f');
    $query->condition('i.widget_active', 1);
    $query->condition('i.type_name', $this->arguments['content_type']);
    return $query;
  }
}