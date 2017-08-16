<?php

function _wildsound_export_cron($break = TRUE){
  // Ensure the directory has been created.
  @drupal_mkdir('public://R');
  // This only works from Drush, so return if we're not using Drush.
  if(!function_exists('drush_get_context')){
    return;
  }
  
  $filename = 'bioacoustic_traits.txt';
  $output_filename = drupal_realpath('public://R') . '/' . $filename;
  global $base_url;
  drush_log(dt("Forking: drush vde r_acoustic_traits views_data_export_2 {$output_filename}"), 'success');
  exec("nohup drush @" . parse_url($base_url, PHP_URL_HOST) . " vde r_acoustic_traits views_data_export_2 {$output_filename}");
}
