<?php

class ScratchpadsMigrateDestinationNode extends MigrateDestinationNode{

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    return $fields;
  }

  public function import(stdClass $node, stdClass $row){
    $migration = Migration::currentMigration();
    // Loop through and unset NULL fields, otherwise they are populated with empty values
    $properties = array_keys((array)$node);
    foreach($properties as $property){
      if((is_array($node->{$property}) && empty($node->{$property}[0])) || empty($node->{$property})){
        unset($node->{$property});
      }
    }
    $status = parent::import($node, $row);
    // If the term has a path, save it
    if(isset($node->path) && !empty($node->path)){
      $path = array(
        'source' => 'node/' . $node->nid,
        'alias' => $node->path
      );
      path_save($path);
    }
    // Extract any files
    foreach($migration->getLongTextFields() as $longTextField){
      if(isset($row->{$longTextField})){
        // TODO - file extraction
      //    		dd($row);
      }
    }
    return $status;
  }

  public function rollback(array $key){
    $status = parent::rollback($key);
    // Delete any paths
    path_delete(array(
      'source' => 'node/' . $key['destid1']
    ));
    return $status;
  }
}