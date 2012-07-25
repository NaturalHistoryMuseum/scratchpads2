<?php

class ScratchpadsMigrateDestinationTerm extends MigrateDestinationTerm{

  public function fields($migration = NULL){
    $fields = parent::fields($migration);
    // Add some extra fields
    $fields['path'] = t('Term path');
    return $fields;
  }

  public function import(stdClass $term, stdClass $row){
    $status = parent::import($term, $row);
    // If the term has a path, save it
    if(isset($term->path)){
      $path = array(
        'source' => 'taxonomy/term/' . $term->tid,
        'alias' => $term->path
      );
      path_save($path);
    }
    return $status;
  }

  public function rollback(array $key){
    $status = parent::rollback($key);
    // Delete any paths
    path_delete(array(
      'source' => 'taxonomy/term/' . $key['destid1']
    ));
    return $status;
  }
}