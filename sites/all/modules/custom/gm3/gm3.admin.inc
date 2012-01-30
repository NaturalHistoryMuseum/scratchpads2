<?php

/**
 * Admin settings
 */
function gm3_admin_settings_form(){
  return array(
    'gm3_settings' => array(
      '#title' => t('GM3 Settings'),
      '#type' => 'textarea',
      '#default_value' => serialize(gm3_settings()),
      '#rows' => 10
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save')
    ),
    '#submit' => array(
      'gm3_admin_settings_form_submit'
    )
  );
}

/**
 * Submit function for the admin settings.
 */
function gm3_admin_settings_form_submit(&$form, &$form_state){
  $data = unserialize($form_state['values']['gm3_settings']);
  if($data){
    variable_set('gm3_default_settings', $data);
  }
}