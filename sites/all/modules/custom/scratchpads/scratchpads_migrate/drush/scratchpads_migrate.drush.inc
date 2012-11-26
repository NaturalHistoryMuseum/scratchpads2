<?php

function scratchpads_migrate_drush_command(){
  $items = array();
  // The 'make-me-a-sandwich' command
  $items['scratchpads-migrate-tidy-up'] = array(
    'description' => "Final tidy up commands for scratchpads migrate.",
    'aliases' => array(
      'spm'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL, // No bootstrap at all.
    'callback' => 'scratchpads_migrate_drush_tidy_up'
  );
  return $items;
}

function scratchpads_migrate_drush_tidy_up(){
  if(!drush_confirm(dt('Are you sure you want to run the scratchpad tidy up migration functions?'))){return drush_user_abort();}
  db_update('apachesolr_environment')->fields(array(
    'name' => 'Scratchpad solr server',
    'url' => 'http://web-scratchpad-solr.nhm.ac.uk/solr/scratchpads2'
  ))->condition('env_id', 'solr')->execute();
  drupal_flush_all_caches();
  node_access_rebuild();
}


  