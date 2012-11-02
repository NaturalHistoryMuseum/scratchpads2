<?php

class ScratchpadsMigrateDestinationTerm extends MigrateDestinationTerm{

  public function fields($migration = NULL){
    $fields = parent::fields($migration);
    // Add some extra fields
    $fields['path'] = t('Term path');
    return $fields;
  }
  
  /**
   * Import a single term.
   *
   * @param $term
   *  Term object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (tid only in this case) of the term that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $term, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      $term->tid = $row->migrate_map_destid1;
      if (isset($term->tid)) {
        if ($term->tid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming tid !tid and map destination nid !destid1 don't match",
              array('!tid' => $term->tid, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $term->tid = $row->migrate_map_destid1;
      }
    }
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($term->tid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination tid provided'));
      }
      $rawterm = $term;
      $this->prepare($term, $row);
      $old_term = taxonomy_term_load($term->tid);
      if (empty($old_term)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but term !tid does not exist',
            array('!tid' => $term->tid)));
      }
      foreach ($rawterm as $field => $value) {
        $old_term->$field = $term->$field;
      }
      $term = $old_term;
    }
    else {
      // Default to bundle if no vocabulary machine name provided
      if (!isset($term->vocabulary_machine_name)) {
        $term->vocabulary_machine_name = $this->bundle;
      }
      // vid is required
      if (empty($term->vid)) {
        static $vocab_map = array();
        if (!isset($vocab_map[$term->vocabulary_machine_name])) {
          // The keys of the returned array are vids
          $vocabs = taxonomy_vocabulary_load_multiple(array(),
              array('machine_name' => $term->vocabulary_machine_name));
          $vids = array_keys($vocabs);
          if (isset($vids[0])) {
            $vocab_map[$term->vocabulary_machine_name] = $vids[0];
          }
          else {
            $migration->saveMessage(t('No vocabulary found with machine_name !name',
                array('!name' => $term->vocabulary_machine_name)));
            return FALSE;
          }
        }
        $term->vid = $vocab_map[$term->vocabulary_machine_name];
      }
  
      // Look up parent name if provided
      if (isset($term->parent_name) && trim($term->parent_name)) {
        // Look for the name in the same vocabulary.
        // Note that hierarchies may have multiples of the same name...
        $terms = taxonomy_term_load_multiple(array(),
            array('name' => trim($term->parent_name), 'vid' => $term->vid));
        $tids = array_keys($terms);
        $term->parent = array($tids[0]);
        unset($term->parent_name);
      }
      if (empty($term->parent)) {
        $term->parent = array(0);
      }
      if (is_array($term->parent) && isset($term->parent['arguments'])) {
        // Unset arguments here to avoid duplicate entries in the
        // term_hierarchy table.
        unset($term->parent['arguments']);
      }
      if (!isset($term->format)) {
        $term->format = $this->textFormat;
      }
      $this->prepare($term, $row);
      // REMOVED: See if the term, with the same parentage, already exists - if so,
      // load it
      // Checking parentage is removed as the source data can return terms with same name and parent
    }
  
    // Trying to update an existing term
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      $existing_term = taxonomy_term_load($term->tid);
      if ($existing_term) {
        // Incoming data overrides existing data, so only copy non-existent fields
        foreach ($existing_term as $field => $value) {
          if (!isset($term->$field)) {
            $term->$field = $existing_term->$field;
          }
        }
      }
    }
  
    if (isset($term->tid)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }
  
    migrate_instrument_start('taxonomy_term_save');
    $status = taxonomy_term_save($term);
    migrate_instrument_stop('taxonomy_term_save');
    $this->complete($term, $row);
    if (isset($term->tid)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      $return = array($term->tid);
    }
    else {
      $return = FALSE;
    }
    return $return;
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