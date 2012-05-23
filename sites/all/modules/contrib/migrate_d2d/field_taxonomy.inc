<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalFieldTaxonomyMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->fieldQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function fieldQuery();
}

class MigrateDestinationFieldTaxonomy extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'vid' => array(
        'type' => 'int',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create taxonomy fields');
  }

  public function import(stdClass $object, stdClass $row){
    dd($object);
  }

  public function fields(){
    return array(
      'vid' => t('Vocabulary ID'),
      'type' => t('Node type')
    );
  }

  public function rollback(array $ids){}
}