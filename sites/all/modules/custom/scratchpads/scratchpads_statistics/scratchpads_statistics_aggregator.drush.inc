<?php

function scratchpads_statistics_aggregator_drush_command(){
  return array(
    'stats-aggregate' => array(
      'callback' => 'scratchpads_statistics_aggregator_drush_stats',
      'description' => 'Harvest stats from other sites if aggregator is installed.'
    )
  );
}

function scratchpads_statistics_aggregator_drush_stats(){
  if(module_exists('scratchpads_statistics_aggregator')){
    module_load_include('cron.inc', 'scratchpads_statistics_aggregator');
    _scratchpads_statistics_aggregator_cron(300);
  }
}