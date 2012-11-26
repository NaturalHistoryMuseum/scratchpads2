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
  drupal_flush_all_caches();
  node_access_rebuild();
}


  