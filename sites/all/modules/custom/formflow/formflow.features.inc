<?php

/**
 * Implementation of hook_features_export_options().
 */
function formflow_features_export_options(){
  $options = array();
  foreach(formflow_get_flows() as $name => $flow){
    $options[$name] = $flow['title'];
  }
  return $options;
}

/**
 * Implementation of hook_features_export().
 */
function formflow_features_export($data, &$export, $module_name = ''){
	
	$export['dependencies']['formflow'] = 'formflow';
  // Load the flow into the features export array
  foreach($data as $name){
    $export['features']['formflow'][$name] = formflow_get_flows($name);
  }
  $pipe = array();
  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function formflow_features_export_render($module, $data, $export = NULL){
  $code = array();
  $code[] = '  $flows = array();';
  $code[] = '';
  foreach($data as $flow){
    $flow_export = features_var_export($flow, '  ');
    $flow_identifier = features_var_export($flow['name']);
    $code[] = "  // Exported flow: {$flow['title']}";
    $code[] = "  \$flows[{$flow_identifier}] = {$flow_export};";
    $code[] = "";
  }
  $code[] = '  return $flows;';
  $code = implode("\n", $code);
  return array(
    'default_flows' => $code
  );
}