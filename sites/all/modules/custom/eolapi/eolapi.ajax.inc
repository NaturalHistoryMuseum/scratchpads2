<?php

/**
 * Callback function.  This will help to populate an empty view.
 */
function eolapi_callback(){
  module_load_include('functions.inc', 'eolapi');
  eolapi_search($_POST['tid']);
  $view = views_get_view($_POST['view-name']);
  $view->args[] = $_POST['tid'];
  $output = $view->render('block');
  if(count($view->result)){
    echo $output;
  } else {
    echo "";
  }
}