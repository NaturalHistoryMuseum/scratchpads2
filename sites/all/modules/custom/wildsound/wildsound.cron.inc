<?php

function _wildsound_export_cron($break = TRUE){
  // Ensure the directory has been created.
  @drupal_mkdir('public://R');
  // This only works from Drush, so return if we're not using Drush.
  if(!function_exists('drush_get_context')){
    return;
  }
  
  global $base_url;
  $output_filename = drupal_realpath('public://R') . '/' . 'bioacoustic_traits.txt';
  drush_log(dt("Forking: drush vde r_acoustic_traits views_data_export_2 {$output_filename}"), 'success');
  exec("nohup drush @" . parse_url($base_url, PHP_URL_HOST) . " vde r_acoustic_traits views_data_export_2 {$output_filename}");
  
  $output_filename = drupal_realpath('public://R') . '/' . 'taxa.txt';
  drush_log(dt("Forking: drush vde r_taxa views_data_export_2 {$output_filename}"), 'success');
  exec("nohup drush @" . parse_url($base_url, PHP_URL_HOST) . " vde r_taxa views_data_export_1 {$output_filename}");
}
