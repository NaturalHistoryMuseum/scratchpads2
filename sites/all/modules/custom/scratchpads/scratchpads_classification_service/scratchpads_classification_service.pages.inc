<?php

/**
 * Handles the search callback
 */
function scratchpads_classification_service_search($search){
  module_load_all_includes('scs.inc');
  $details = array();
  foreach(module_implements('scratchpads_classification_service_info') as $module){
    $details[$module] = module_invoke($module, 'scratchpads_classification_service_info');
  }
  $results = array();
  foreach($details as $module => $detail){
    foreach($detail as $identifier => $service){
      if(function_exists($service['search'])){
        $this_results = $service['search']($search);
        foreach($this_results as $key => $value){
          $results[url('classification/download/0/' . $module . '/' . $identifier . '/' . $key, array(
            'absolute' => TRUE
          ))] = $value . ' (' . $service['name'] . ')';
        }
      }
    }
  }
  return $results;
}

/**
 * Handles the download of the CSV/Excel file
 */
function scratchpads_classification_service_download($page, $module, $service, $id){
  if(module_exists($module)){
    module_load_include('scs.inc', $module);
    $details = module_invoke($module, 'scratchpads_classification_service_info');
    if($details && isset($details[$service])){
      if(function_exists($details[$service]['download'])){
        $details[$service]['download']($id, $page, 100000);
        return TRUE;
      }
    }
    return FALSE;
  }
}

/**
 * Basic delivery function which does nothing (we've already delivered).
 */
function scratchpads_classification_service_do_nothing($result){
  if(!$result){
    // We should output an error here, although we could catch it above.
  }
}

/**
 * Basic function that seems to be missing to deliver JSON to the browser or to
 * another service.
 *
 * Copied from scratchpads_statistics.module
 *
 * Based on ajax_delver (see documentation there).
 */
function scratchpads_classification_service_json_deliver($json){
  if($json == MENU_ACCESS_DENIED){
    $json = array(
      'ACCESS DENIED'
    );
  }
  $iframe_upload = !empty($_POST['ajax_iframe_upload']);
  if(is_null(drupal_get_http_header('Content-Type'))){
    if(!$iframe_upload){
      drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
    }else{
      drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
    }
  }
  drupal_alter('json', $json);
  $json = drupal_json_encode($json);
  if(!$iframe_upload){
    print $json;
  }else{
    print '<textarea>' . $json . '</textarea>';
  }
  ajax_footer();
}