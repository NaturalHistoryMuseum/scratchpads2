<?php

/**
 * Copy of ajax_form_callback
 */
function tui_ajax_form_callback(){
  // The only addition over the standard callback, is this include!
  module_load_include('admin.inc', 'taxonomy');
  list($form, $form_state) = ajax_get_form();
  drupal_process_form($form['#form_id'], $form, $form_state);
  // We need to return the part of the form (or some other content) that needs
  // to be re-rendered so the browser can update the page with changed content.
  // Since this is the generic menu callback used by many Ajax elements, it is
  // up to the #ajax['callback'] function of the element (may or may not be a
  // button) that triggered the Ajax request to determine what needs to be
  // rendered.
  if(!empty($form_state['triggering_element'])){
    $callback = $form_state['triggering_element']['#ajax']['callback'];
  }
  if(!empty($callback) && function_exists($callback)){return $callback($form, $form_state);}
}

/**
 * Callback function to return HTML
 */
function tui_ajax_form_callback_callback(&$form, &$form_state){
  return drupal_render($form);
}

/**
 * Callback to simply hide the form.
 */
function tui_ajax_blank(&$form, &$form_state){
  if(isset($form_state['tui-ajax'])){
    $ajax = $form_state['tui-ajax'];
  }else{
    $ajax = array();
  }
  $ajax[] = ajax_command_invoke('#tui-tree-form', 'slideUp', array(
    200
  ));
  print ajax_render($ajax);
  exit();
}