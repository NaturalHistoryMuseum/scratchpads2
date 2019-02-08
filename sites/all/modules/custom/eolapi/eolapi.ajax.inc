<?php

/**
 * Callback function.  This will help to populate an empty view.
 */
function eolapi_callback()
{

  if (variable_get('eolapi_is_disabled', false)) {
    return false;
  }

  $callbacks = module_invoke_all('eolapi_callbacks');

  $view_name = $_POST['viewName'];
  $tid = $_POST['tid'];

  if (!empty($callbacks[$view_name])) {
    call_user_func($callbacks[$view_name], $tid);
  } elseif (!empty($tid)) {
    module_load_include('functions.inc', 'eolapi');
    eolapi_search($tid);
  }

  if (!empty($tid)) {
    $view = views_get_view($view_name);
    $view->args[] = $tid;
    if (method_exists($view, 'render')) {
      $output = $view->render('block');
      if (count($view->result)) {
        echo $output;
        return;
      }
    } else {
      return _eolapi_callback_error();
    }
  } else {
    return _eolapi_callback_error();
  }
  echo "";
}

function _eolapi_callback_error()
{
  return watchdog('eolapi', 'There is an issue with your view @view_name', array(
    '@view_name' => $_POST['view-name']
  ));
}
