<?php

/** 
 * hook_menu callback; 
 * Universal json callback - builds function name and returns error = true 
 */
function slickgrid_callback($op){
  $func = 'slickgrid_callback_' . $op;
  // Basic required json 
  $json = array(
    'status' => TRUE,
    'op' => $op
  );
  // The callback can append extra json
  if(is_array($additonal_json = $func())){
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
  // TODO - work with all entities
  //  $entities = entity_load($_POST['entity_type'], $_POST['entity_ids']);
  //	
  //  $controller = entity_get_controller($_POST['entity_type']);
  // There doesn't seem to be a generic entity_delete function???
  entity_delete_multiple($_POST['entity_type'], $_POST['entity_ids']);
  drupal_set_message(format_plural(count($_POST['entity_ids']), '1 item was deleted.', '@count items were deleted.'));
  return array(
    'deleted' => $_POST['entity_ids']
  );
}

/**
 * 
 * Get a views filtered by NIDs 
 * @param string $view_name
 * @param string $display_id
 * @param array $nids
 */
function slickgrid_callback_get_view($view_name, $display_id, $nids){
  $view = views_get_view($view_name);
  $view->set_display($display_id);
  // Remove all existing arguments - we'll limit result set by NID only
  foreach($view->get_items('argument') as $id => $arg){
    $view->set_item($display_id, 'argument', $id, NULL);
  }
  // Add an argument to limit the view to only nids being updated	
  $options = array(
    'table' => 'node',
    'field' => nid,
    'break_phrase' => 1, // Allow multiple nids,
    'validate_argument_nid_type' => 'nid'
  );
  $view->add_item($display_id, 'argument', 'node', 'nid', $options);
  $view->set_arguments(array(
    implode('+', $nids)
  ));
  $view->pre_execute();
  $view->execute();
  $view->render();
  return $view;
}

/**
 * 
 * Callback function - update a view's settings
 */
function slickgrid_callback_settings(){
  global $user;
  slickgrid_set_settings($user->uid, $_POST['view'], array(
    $_POST['setting'] => $_POST['value']
  ));
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
  require_once DRUPAL_ROOT . '/' . $plugin_definition['path'] . "/handler.class.php";
  $class = ctools_plugin_get_class($plugin_definition, 'handler');
  if($class){
    $editor_plugin = new $class($plugin_definition);
    return $editor_plugin->update();
  }
}

/**
 * Callback function - log an message 
 */
function slickgrid_callback_undo(){
  // TODO - change this to use entities
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
    $view = slickgrid_callback_get_view($_POST['view'], $_POST['display_id'], array_keys($updated_nodes));
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



