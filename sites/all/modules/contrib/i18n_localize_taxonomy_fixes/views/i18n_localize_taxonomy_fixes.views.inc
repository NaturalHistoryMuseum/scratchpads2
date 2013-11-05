<?php
/**
 * Implementation of hook_views_data_alter().
 */
function i18n_localize_taxonomy_fixes_views_data_alter(&$data) {
	
 foreach ($data as $table => $config) {
	
    foreach ($config as $item => $item_config) {
    	if (strrpos($table,"field_data")!== false){
    	
    	if (isset($item_config['filter']['handler'])) {
      	  
      		if ($item_config['filter']['handler'] == 'views_handler_filter_term_node_tid') {
                $data[$table][$item]['filter']['handler'] = 'views_handler_i18n_localize_taxonomy_fixes_term_node_tid';
      }
      }
    	}
    }
  
  }
 
 
}
