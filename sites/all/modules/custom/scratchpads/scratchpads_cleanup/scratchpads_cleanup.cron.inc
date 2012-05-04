<?php

/**
 * Helper function that does all the work for this module's hook_cron.
 */
function _scratchpads_cleanup_cron(){
  // Cleanup any terms that should have been deleted.
  // Max 1000 at a time.
  $results = db_query('SELECT tid FROM {taxonomy_term_data} WHERE vid NOT IN (SELECT vid FROM {taxonomy_vocabulary} LIMIT 1000');
  foreach($results as $result){
    taxonomy_term_delete($result->tid);
  }
}