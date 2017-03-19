<?php

function _wildsound_export_cron($break = TRUE){
  // Ensure the directory has been created.
  @drupal_mkdir('public://traits');
  // This only works from Drush, so return if we're not using Drush.
  if(!function_exists('drush_get_context')){
    return;
  }
  // Get a list of views on which to operate
  module_load_include('views_default.inc', 'wildsound');
  $views = wildsound_views_default_views();
  $view = $views['r_acoustic_traits'];
  $filename = 'bioacoustic_traits.txt';
  $output_filename = drupal_realpath('public://traits') . '/' . $filename;
  global $base_url;
  drush_log(dt("Forking: drush vde {$view->name} views_data_export_1 $output_filename"), 'success');
  exec("nohup drush @" . parse_url($base_url, PHP_URL_HOST) . " vde {$view->name} views_data_export_1 {$output_filename}");
}