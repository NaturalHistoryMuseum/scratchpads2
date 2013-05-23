<?php

function dwca_export_drush_command(){
  return array(
    'dwca_rebuild' => array(
      'callback' => 'dwca_export_drush_rebuild',
      'description' => 'Rebuild the Darwin Core Archive'
    )
  );
}

function dwca_export_drush_rebuild(){
  if(!module_exists('dwca_export')){return;}
  module_load_include('cron.inc', 'dwca_export');
  if(_dwca_export_cron()){
    _dwca_export_cron_create_zip();
    drupal_set_message(t('The ZIP file has been created'));
  }else{
    drupal_set_message(t('Please re-run this function to build the ZIP file'));
  }
}