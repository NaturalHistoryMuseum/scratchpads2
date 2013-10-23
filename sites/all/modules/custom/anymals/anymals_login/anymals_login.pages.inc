<?php

/**
 * Callback to create the basic anymals login page.
 */
function anymals_login_page_form(){
  global $user;
  // FIXME - Remove the following user load line.
  $user = user_load(0);
  if($user->uid){
    // We are already logged-in, we should simply dislpay some information, and
    // ensure that the client_id is saved.
  }else{
    // We need to log the user in.
    // Display login form:
    $form = drupal_get_form('user_login');
    $form['#attached']['css'][] = drupal_get_path('module', 'anymals_login') . '/css/login_page.css';
    return $form;
  }
}

/**
 * Deliver a basic page without any additional theme shit.
 */
function anymals_login_basic_page($page_callback_result){
  // We render the output first so that "#attached" js and css are added to the
  // relevant arrays.
  $output = drupal_render($page_callback_result);
  print '<html>
  <head>
    <title>' . drupal_get_title() . '</title>
    ' . drupal_get_css() . '
    ' . drupal_get_js() . '
    <meta name="viewport" content="width=device-width, user-scalable=false;">
  </head>
  <body style="background-color: #' . variable_get('scratchpads_colour', SCRATCHPADS_DEFAULT_COLOUR) . '">
    <div id="anymals-login">' . $output . '</div>
  </body>
</html>';
}