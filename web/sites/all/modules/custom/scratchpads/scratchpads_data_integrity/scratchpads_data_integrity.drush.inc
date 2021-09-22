<?php

function scratchpads_data_integrity_drush_command(){
  return array(
    'datacheck' => array(
      'callback' => 'scratchpads_data_integrity_drush_check',
      'description' => 'Run the data integrity check'
    )
  );
}

function scratchpads_data_integrity_drush_check(){
  if(module_exists('scratchpads_data_integrity')){
    foreach(scratchpads_data_integrity_get_messages(TRUE, TRUE) as $message){
      drush_log($message, 'warning');
    }
  }
}