<?php

/**
 * Menu callback. 
 */
function oboe_add_job(){
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  // Bypass the node/add listing if only one content type is available.
  if(count($content) == 1){
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('node_add_list', array(
    'content' => $content
  ));
}

/**
 * Returns an OBOE submission form.
 */
function oboe_add($type){
  global $user;
  $oboe = (object)array(
    'uid' => $user->uid,
    'type' => $type
  );
  drupal_set_title(t('Create @name', array(
    '@name' => $types[$type]->name
  )), PASS_THROUGH);
  $output = drupal_get_form($type . '_oboe_form', $oboe);
  return $output;
}

/**
 * Callback defined in hook_forms().
 */
function oboe_job_form($form, &$form_state, $oboe){
  // Firstly, we need to get the parameters for this oboe type.
  $parameters = OBOEService::get_parameters_by_type(FALSE, $oboe->type);
  return array(
    'something' => array(
      '#markup' => print_r($parameters, 1)
    )
  );
}