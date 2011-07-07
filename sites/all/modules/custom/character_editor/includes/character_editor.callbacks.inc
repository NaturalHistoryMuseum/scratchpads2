<?php

/** 
 * hook_menu callback; 
 * Universal json callback - builds function name and returns error = true 
 */
function character_editor_callback($op){
  $func = 'character_editor_callback_' . $op;
  // Basic required json 
  $json = array(
    'status' => TRUE,
    'op' => $op
  );
  // Return the json
  drupal_json_output($json);
}

function character_editor_callback_get_character_fields(){
	
}

