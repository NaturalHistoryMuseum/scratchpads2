<?php

class ScratchpadsNodeCountriesmapMigration extends ScratchpadsNodeMigration{

  public function __construct(array $arguments){
    $this->manualFieldMapping = array(
      'country' => 'field_spm_map'
    );
    parent::__construct($arguments);
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $fields['country'] = 'array';
    return $fields;
  }

  protected function nodeQuery(){
    // TODO - Must have TID
    $query = parent::nodeQuery();
    return $query;
  }

  public function prepareRow($row){
    parent::prepareRow($row);

    // Get the TDGW region data
    // Add the location data
    $query = Database::getConnection('default', $this->sourceConnection)->select('countriesmap_tdwg', 't');
    $query->fields('t', array(
      'level4code'
    ));
    $query->innerJoin('countriesmap_nodes', 'cmn', 'cmn.cmid = t.cmid');
    $query->condition('cmn.vid', $row->vid);
    $result = $query->execute();
    foreach($result as $location){
      $level_3 = substr($location->level4code, 0, 3);
      $level_4 = substr($location->level4code, 3, 2);
      // Get the level 1 & level 2 codes
      $top_levels = db_select('gm3_region_data', 'rd')->fields('rd', array(
        'level_1_code',
        'level_2_code'
      ))->condition('level_3_code', $level_3)->condition('level_4_code', $level_4)->execute()->fetchObject();
      $row->country[] = $top_levels->level_1_code . ':' . $top_levels->level_2_code . ':' . $level_3;
    }
  }
}
