<?php

/**
 * Callback function.  This will help to populate an empty view.
 */
function eolapi_callback(){  
  module_load_include('functions.inc', 'eolapi');
  $search_results = eolapi_search($_POST['tid']);
  $view = views_get_view($_POST['view-name']);
  $view->args[] = $_POST['tid'];
  $output = $view->render('block');
  echo $output;  
}