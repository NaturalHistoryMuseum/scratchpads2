<?php

function _scratchpads_user_register_field_sort($a, $b){
  return $a['widget']['weight'] >= $b['widget']['weight'];
}

function theme_scratchpads_user_register_form($variables){
  // Add CSS
  drupal_add_css(drupal_get_path('module', 'scratchpads_user') . '/css/scratchpads_user.css');
  
  $output = '
<div class="clearfix">
  <div class="scratchpads-user-left">
    <div>';
  $fields = $variables['form']['#fields'];
  uasort($fields, '_scratchpads_user_register_field_sort');
  $field_names = array_keys($fields);
  // Add each field
  foreach($field_names as $field_name){
    $output .= drupal_render($variables['form'][$field_name]);
  }
  $output .= '
    </div>
  </div>
  <div class="scratchpads-user-right">
    <div>';
  // Render "create user" checkbox.
  $output .= drupal_render($variables['form']['create_user_account']);
  // Move the actions element outside of the columns
  $actions = $variables['form']['actions'];
  unset($variables['form']['actions']);
  $output .= drupal_render_children($variables['form']);
  $output .= drupal_render($variables['form']['account']['mail']);
  $output .= '
    </div>
  </div>
</div>';
  $output .= drupal_render($actions);
  return $output;
}