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
  slickgrid_callback_get_messages($json);
  // Return the json
  drupal_json_output($json);
}

function slickgrid_callback_get_messages(&$json){
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
 * Callback function - delete a node
 */
function slickgrid_callback_delete(){
  foreach($_POST['entity_ids'] as $entity_id){
    entity_delete($_POST['entity_type'], $entity_id);
  }
  drupal_set_message(format_plural(count($_POST['entity_ids']), '1 item was deleted.', '@count items were deleted.'));
  return array(
    'deleted' => $_POST['entity_ids']
  );
}

/**
 * 
 * Callback function - update a view's settings
 */
function slickgrid_callback_settings(){
  global $user;
  if($user->uid){ // do not save settings for anonymous users
    slickgrid_set_settings($user->uid, $_POST['view'], $_POST['display_id'], array(
      $_POST['setting'] => $_POST['value']
    ));
  }
}

/**
 * Callback function - log an message 
 */
function slickgrid_callback_log(){
  watchdog('slickgrid', $_POST['error'], array(), WATCHDOG_ERROR);
}

/**
 * Callback function - update a node
 * This is the slickgrid.js update function.
 * 
 */
function slickgrid_callback_update(){
  ctools_include('plugins');
  $plugin_definition = ctools_get_plugins('slickgrid', 'editor', $_POST['plugin']);
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', $plugin_definition['plugin module']) . "/plugins/editors/handler.class.php";
  $class = ctools_plugin_get_class($plugin_definition, 'handler');
  if($class){
    $editor_plugin = new $class($plugin_definition);
    return $editor_plugin->update();
  }
}

/**
 * Callback function - clone an entity
 * 
 */
function slickgrid_callback_clone(){
  $entities = entity_load($_POST['entity_type'], $_POST['entity_ids']);
  $entity_keys = slickgrid_get_entity_keys($_POST['entity_type'], array(
    'bundle',
    'label'
  ));
  foreach($entities as $entity){
    $clone = clone $entity;
    // Unset all entity keys to create a new entity 
    foreach($entity_keys as $entity_key){
      if(property_exists($clone, $entity_key)){
        unset($clone->$entity_key);
      }
    }
    entity_save($_POST['entity_type'], $clone);
  }
  drupal_set_message(format_plural(count($_POST['entity_ids']), 'Cloned 1 item.', 'Cloned @count items.'));
  // Reload the view
  $view = slickgrid_get_view($_POST['view'], $_POST['display_id']);
  // Return the view data to be reloaded in the grid
  return array(
    'data' => $view->data
  );
}

/**
 * Callback function - add an entity
 * 
 */
function slickgrid_callback_add($entity_type, $bundle_type, $view_name, $display_id){
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
  if($form['#validated'] && !form_get_errors()){
    $output = array();
    $output[] = ctools_modal_command_dismiss();
    $result = array();
    slickgrid_callback_get_messages($result);
    // Reload the view
    $view = slickgrid_get_view($view_name, $display_id, array(), $args);
    $result['data'] = $view->data;
    $result['columns'] = slickgrid_encode_columns($view->columns);
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

/**
 * Callback function - undo (revert nodes)
 */
function slickgrid_callback_undo(){
  $field_name = $_POST['field_name'];
  $field_id = $_POST['field_id'];
  $updated_nodes = $_POST['updated'];
  if(is_array($updated_nodes)){
    foreach($updated_nodes as $nid => $node){
      $node_revision = node_load($nid, $node['vid']);
      $node_revision->revision = 1;
      $node_revision->log = t('Copy of the revision from %date.', array(
        '%date' => format_date($node_revision->revision_timestamp)
      ));
      node_save($node_revision);
    }
    // All nodes have been reverted - need to get the values from the view
    $view = slickgrid_get_view($_POST['view'], $_POST['display_id'], array_keys($updated_nodes));
    foreach($view->result as $count => $row){
      $updated_nodes[$row->nid]['value'] = $view->render_field($field_id, $count);
    }
  }
  drupal_set_message(format_plural(count($updated_nodes), 'Reset changes to 1 item.', 'Reset changes to @count items.'));
  return array(
    'updated' => $updated_nodes,
    'field_name' => $field_name,
    'field_id' => $field_id
  );
}



