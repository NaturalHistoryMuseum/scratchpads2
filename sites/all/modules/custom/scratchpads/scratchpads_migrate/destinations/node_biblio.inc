<?php

class ScratchpadsMigrateDestinationNodeBiblio extends ScratchpadsMigrateDestinationNode{

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    $biblio_fields = biblio_get_db_fields();
    foreach($biblio_fields as $biblio_field){
    	$fields[$biblio_field] = $biblio_field;
    }
    return $fields;
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