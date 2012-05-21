<?php

/**
 * Does all the heavy lifting for this module's hook_cron().
 */
function _scratchpads_registry_stats_cron(){
  if(variable_get('scratchpads_registry_stats_cron_weekly', 0) < (time() - 604800)){
    scratchpads_registry_stats_cron_weekly();
  }
  if(variable_get('scratchpads_registry_stats_cron_daily', 0) < (time() - 86400)){
    scratchpads_registry_stats_cron_daily();
  }
  if(variable_set('cron_last', 0) < (time() - 3600)){
    scratchpads_registry_stats_cron_daily();
  }
}

/**
 * Weekly cron
 *
 * Performs functions once a week. Non-essential tasks, like updating nodes/
 * organizations/scratchpads should be placed here.
 */
function scratchpads_registry_stats_cron_weekly(){
  $gbif_registry = new GBIFRegistry();
  foreach($gbif_registry->getTechnicalInstallations() as $technical_installation){
    
  }
  variable_set('scratchpads_registry_stats_cron_weekly', time());
}

/**
 * Daily cron
 *
 * Perform daily updates, such as querying a Scratchpad for metadata.
 */
function scratchpads_registry_stats_cron_daily(){
  variable_set('scratchpads_registry_stats_cron_daily', time());
}

/**
 * Hourly cron
 */
function scratchpads_registry_stats_cron_hourly(){}