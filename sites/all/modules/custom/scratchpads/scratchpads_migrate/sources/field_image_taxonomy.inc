<?php

class DrupalFieldImageTaxonomyMigration extends DrupalFieldTaxonomyMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
  }

  protected function sourceQuery(){
    $query = $this->baseQuery();
    $query->condition('vnt.type', 'image');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    $row->entity_type = 'file';
    $row->bundle = 'image';
    if(parent::prepareRow($row) === FALSE){return FALSE;}
  }
}