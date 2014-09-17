<?php

/**
 * Callback function.  This will help to populate an empty view.
 */
function eolapi_callback(){
  $callbacks = module_invoke_all('eolapi_callbacks');
  if(isset($callbacks[$_POST['view-name']])){
    call_user_func($callbacks[$_POST['view-name']], $_POST['tid']);
  }else{
    module_load_include('functions.inc', 'eolapi');
    eolapi_search($_POST['tid']);
  }
  $view = views_get_view($_POST['view-name']);
  $view->args[] = $_POST['tid'];
  if(method_exists($view, 'render')){
    $output = $view->render('block');
    if(count($view->result)){
      echo $output;
      return;
    }
  }else{
    watchdog('eolapi', 'There is an issue with your view @view_name', array(
      '@view_name' => $_POST['view-name']
    ));
  }
  echo "";
}