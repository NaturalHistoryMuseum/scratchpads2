<?php

class ScratchpadsMigrateDestinationNodeBiblio extends ScratchpadsMigrateDestinationNode{

  public function fields($migration = NULL){
    $migration = Migration::currentMigration();
    $fields = parent::fields($migration);
    // Add any extra fields which won't be picked up by the scan of field elements
    $fields['path'] = t('Term path');
    // Add a biblio_contributors field to add authors
    // It's not in the list of biblio fields, and you can't use any of the biblio_authors fields. Fucking crap biblo
    $fields['biblio_contributors'] = t('Biblio contributors');
    $biblio_fields = biblio_get_db_fields();
    foreach($biblio_fields as $biblio_field){
      $fields[$biblio_field] = $biblio_field;
    }
    return $fields;
  }
}