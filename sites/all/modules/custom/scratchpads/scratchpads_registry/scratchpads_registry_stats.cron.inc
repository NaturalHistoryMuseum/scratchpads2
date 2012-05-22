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
 *
 * FIXME - Not sure what the "tag" is going to be for a "scratchpad", although
 * it is likely to be "scratchpad". This needs checking.
 */
function scratchpads_registry_stats_cron_weekly(){
  $gbif_registry = GBIFRegistry::singleton();
  // foreach($gbif_registry->getTechnicalInstallations('scratchpad') as
  // $technical_installation){
  foreach($gbif_registry->getTechnicalInstallations() as $technical_installation){
    echo "HERE";exit;
    // Check that the organization that this installation is linked to exists.
    // If it doesn't, we need to create it.
    print_r($technical_installation);
    exit();
  }
  // FIXME - For debugging - remove the comments on the next line
  // variable_set('scratchpads_registry_stats_cron_weekly', time());
}

/**
 * Daily cron
 *
 * Perform daily updates, such as querying a Scratchpad for metadata.
 */
function scratchpads_registry_stats_cron_daily(){
  // FIXME - For debugging - remove the comments on the next line
  // variable_set('scratchpads_registry_stats_cron_daily', time());
}

/**
 * Hourly cron
 */
function scratchpads_registry_stats_cron_hourly(){}