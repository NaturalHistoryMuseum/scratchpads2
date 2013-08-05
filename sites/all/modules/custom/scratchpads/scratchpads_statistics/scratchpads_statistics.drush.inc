<?php

function scratchpads_statistics_drush_command(){
  return array(
    'stats' => array(
      'callback' => 'scratchpads_statistics_drush_stats',
      'description' => 'Rebuild the stats.'
    )
  );
}

function scratchpads_statistics_drush_stats(){
  if(module_exists('scratchpads_statistics')){
    module_load_include('cron.inc', 'scratchpads_statistics');
    _scratchpads_statistics_cron();
  }
}