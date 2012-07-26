<?php

class ScratchpadsNodeCountriesmapMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
  }
  /*
  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $fields['location_country'] = 'text';
    $fields['location_latlong'] = 'integer';
    return $fields;
  }
  */
  protected function nodeQuery(){
    $query = parent::nodeQuery();
    $query->innerJoin('countriesmap_nodes', 'cmn', 'cmn.vid = n.vid');
    $query->innerJoin('countriesmap_tdwg', 'cmt', 'cmt.cmid = cmn.cmid');
    $query->fields('cmt', array(
      'level4code'
    ));
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);
  }
}
