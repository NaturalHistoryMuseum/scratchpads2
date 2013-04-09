<?php

/** 
 * hook_menu callback; 
 * Universal json callback - builds function name and returns error = true 
 */
function slickgrid_callback($op){
  $func = 'slickgrid_callback_' . $op;
  // Get any additonal arguments passed to the callback function & shift off the first one (the $op)  
  $args = func_get_args();
  array_shift($args);
  // Basic required json 
  $json = array(
    'status' => TRUE,
    'op' => $op
  );
  // The callback can append extra json
  if(is_array($additonal_json = call_user_func_array($func, $args))){
    $json += $additonal_json;
  }
  slickgrid_callback_add_messages($json);
  // Return the json
  drupal_json_output($json);
}

/**
 * Alters a JSON array to add the messages.
 * 
 * Note, although this function is named slickgrid_callback_*, it is NOT a 
 * function to be called from "slickgrid_callback".
 */
function slickgrid_callback_add_messages(&$json){
  // Add any drupal messages that have been created
  foreach(drupal_get_messages() as $type => $messages){
    foreach($messages as $message){
      if($type == 'error'){
        $json['error'] = true;
      }
      $json['messages'][] = array(
        'type' => $type,
        'message' => $message
      );
    }
  }
}

/**
 * 
 * Callback function - update a view's settings
 */
function slickgrid_callback_settings(){
  // Set the settings.
  global $user;
  // do not save settings for anonymous users
  if($user->uid){
    // FIXME - This does not add any checks!
    slickgrid_set_settings($user->uid, $_POST['view'], $_POST['display_id'], array(
      $_POST['setting'] => $_POST['value']
    ));
  }
  // Basic output.
  $json = array(
    'status' => TRUE,
    'op' => 'settings'
  );
  slickgrid_callback_add_messages($json);
  // Return the json
  drupal_json_output($json);
}

/**
 * Callback function - update a node
 * This is the slickgrid.js update function. 
 */
function slickgrid_callback_update(){
  // Basic required json
  $json = array(
    'status' => TRUE,
    'op' => 'update'
  );
  ctools_include('plugins');
  $plugin_definition = ctools_get_plugins('slickgrid', 'editor', $_POST['plugin']);
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', $plugin_definition['plugin module']) . "/plugins/editors/handler.class.php";
  $class = ctools_plugin_get_class($plugin_definition, 'handler');
  if($class){
    $editor_plugin = new $class($plugin_definition);
    $extra_json = $editor_plugin->update();
    if(is_array($extra_json)){
      $json += $extra_json;
    }
  }
  slickgrid_callback_add_messages($json);
  // Return the json
  drupal_json_output($json);
}

/**
 * Callback function - add an entity
 */
function slickgrid_callback_add($entity_type, $bundle_type){
  global $user;
  $entity_info = entity_get_info($entity_type);
  $args = array_slice(func_get_args(), 4);
  ctools_include('modal');
  ctools_include('ajax');
  $form_callback_func = $entity_info['form callback'];
  if(function_exists($entity_info['creation callback'])){
    $entity = call_user_func($entity_info['creation callback'], array(
      'type' => $bundle_type,
      'name' => $user->name
    ), $entity_type);
    $form = $form_callback_func($entity);
  }else{
    $form = $form_callback_func();
  }
  $form_state = $_POST + array(
    'title' => t('Add @bundle_label', array(
      '@bundle_label' => strtolower($entity_info['bundles'][$bundle_type]['label'])
    ))
  );
  if(isset($form['#validated']) && !form_get_errors()){
    $output = array();
    $output[] = ctools_modal_command_dismiss();
    $result = array();
    slickgrid_callback_add_messages($result);
    $output[] = array(
      'command' => 'slickgrid',
      'response' => array(
        'result' => $result
      )
    );
  }else{
    $output = ctools_modal_form_render($form_state, $form);
  }
  print ajax_render($output);
  exit();
}