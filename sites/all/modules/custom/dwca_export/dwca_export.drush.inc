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
	variable_set('dwca_export_rebuild', TRUE);
	variable_set('dwca_Export_zip_build_time', 0);
	module_load_include('cron.inc', 'dwca_export');
	_dwca_export_cron();
}