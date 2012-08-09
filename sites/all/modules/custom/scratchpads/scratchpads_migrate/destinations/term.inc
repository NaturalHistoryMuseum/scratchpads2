<?php

class ScratchpadsMigrateDestinationTerm extends MigrateDestinationTerm{

  public function fields($migration = NULL){
    $fields = parent::fields($migration);
    // Add some extra fields
    $fields['path'] = t('Term path');
    return $fields;
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