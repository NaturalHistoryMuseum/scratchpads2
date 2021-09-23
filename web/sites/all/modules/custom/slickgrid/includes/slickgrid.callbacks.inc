<?php

/**
 * Alters a JSON array to add the messages.
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
 * Callback function - update a node
 * This is the slickgrid.js update function.
 */
function slickgrid_callback_update(){
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
  drupal_json_output($json);
}

/**
 * Callback function - add an entity
 */
function slickgrid_callback_add($entity_type, $bundle_type){
  global $user;
  $entity_info = entity_get_info($entity_type);
  $args = array_slice(func_get_args(), 2);
  ctools_include('modal');
  ctools_include('ajax');
  $form_callback_func = $entity_info['form callback'];
  $entity = NULL;
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
    $result = module_invoke_all('slickgrid_add_entity', $entity_type, $bundle_type, $args, $entity);
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