<?php

/**
 * Helper function that does all the work for this module's hook_cron.
 */
function _scratchpads_cleanup_cron(){
  scratchpads_cleanup_cleanup_taxonomy();
}

/**
 * Cleanup taxonomy
 */
function scratchpads_cleanup_cleanup_taxonomy(){
  // Just in case a previous vocabulary delete failed, we try again.
  $vocabulary = taxonomy_vocabulary_machine_name_load('__DELETE__');
  if($vocabulary){
    taxonomy_vocabulary_delete($vocabulary->vid);
  }
  $results = db_query('SELECT COUNT(*) AS count FROM {taxonomy_term_data} WHERE vid NOT IN (SELECT vid FROM {taxonomy_vocabulary})')->fetch();
  if($results->count){
    // We have some terms that need deleting.  We need to create a vocabulary
    // for them to be added to, which we then delete.
    $vocabulary = new stdClass();
    $vocabulary->name = 'DELETE';
    $vocabulary->machine_name = '__DELETE__';
    taxonomy_vocabulary_save($vocabulary);
    db_query('UPDATE {taxonomy_term_data} SET vid = :vid WHERE vid NOT IN (SELECT vid FROM {taxonomy_vocabulary})', array(
      ':vid' => $vocabulary->vid
    ));
    // Set the parent as 0 for all the terms in the vocabulary.  This ensures
    // that they all get deleted.
    db_query('UPDATE {taxonomy_term_hierarchy} SET parent = 0 WHERE tid IN (SELECT tid FROM {taxonomy_term_data} WHERE vid = :vid)', array(
      ':vid' => $vocabulary->vid
    ));
    taxonomy_vocabulary_delete($vocabulary->vid);
  }
  // Ensure no terms are set as being a child of themselves.
  db_query('UPDATE {taxonomy_term_hierarchy} SET parent = 0 WHERE parent = tid');
}