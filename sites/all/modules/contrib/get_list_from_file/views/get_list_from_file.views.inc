<?php
/**
 * Implementation of hook_views_data_alter().
 */
function get_list_from_file_views_data_alter(&$data) {
  foreach ($data as $table => $config) {
	
    foreach ($config as $item => $item_config) {
    	if (strrpos($table,"field_data")!== false){
    	if (strrpos($item,"get_list_from_file")>0) {
    		//if ($item=="field_ag_country") {
      if (isset($item_config['filter']['handler'])) {
      	  
      		if ($item_config['filter']['handler'] == 'views_handler_filter_string') {
        $data[$table][$item]['filter']['handler'] = 'views_handler_filter_get_list_from_file_in_operator';
      }
      }}
    	}
    }
  //}
  }
 
  return $data;
}
 
/**
 * Implementation of hook_views_handlers().


function get_list_from_file_views_handlers() {
  return array(
   
      'path' => drupal_get_path('module', 'get_list_from_file') . '/handlers',
	  'handlers' => array(
      'views_handler_filter_get_list_from_file_in_operator' => array(
      'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
 */