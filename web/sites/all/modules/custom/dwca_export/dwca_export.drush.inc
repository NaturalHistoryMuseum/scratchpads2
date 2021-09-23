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
  if(_dwca_export_cron(FALSE)){
    _dwca_export_cron_create_zip();
    drush_log(dt('The ZIP file has been created'), 'success');
  }else{
    drush_log(dt('Please wait for approximately five minutes (or check \'ps\' to see that the commands have finished) and re-run this function to build the ZIP file'), 'success');
  }
}