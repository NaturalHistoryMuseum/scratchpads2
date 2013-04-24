<?php

/**
 * Implements hook_install_tasks_alter().
 */
function scratchpad_2_training_install_tasks_alter(&$tasks, &$install_state){
  $tasks['install_select_locale'] = array(
    'function' => 'scratchpad_2_training_select_locale'
  );
  $tasks['install_configure_form'] = array(
    'function' => 'scratchpad_2_training_install_configure_form'
  );
}

/**
 * scratchpad_2_training_install_task_one().
 */
function scratchpad_2_training_install_configure_form(){
  variable_set('site_name', 'Scratchpad training site');
  variable_set('site_mail', 'scratchpad@nhm.ac.uk');
  variable_set('date_default_timezone', 'UTC');
  variable_set('site_default_country', 'GB');
  variable_set('clean_url', 1);
  variable_set('install_time', $_SERVER['REQUEST_TIME']);
  $account = user_load(1);
  $edit = array(
    'name' => 'Scratchpad Team',
    'pass' => uniqid() . uniqid() . uniqid(),
    'mail' => 'scratchpads@nhm.ac.uk',
    'status' => 1,
    'init' => '',
    'roles' => array(),
    'timezone' => 'UTC',
    'legal_accept' => FALSE
  );
  user_save($account, $edit);
  // Ensure this user 1 can not login directly.
  db_update('users')->fields(array(
    'pass' => ''
  ))->condition('uid', 1)->execute();
  // Create a new account for the trainee.
  $edit['name'] = 'username';
  $edit['mail'] = 'scratchpad@nhm.ac.uk';
  $edit['roles'] = array(
    5 => TRUE
  );
  user_save(NULL, $edit);
  // No idea why, but having issues with setting the password for this user, so
  // instead we'll hard code it!
  db_update('users')->fields(array(
    'pass' => user_hash_password('password')
  ))->condition('uid', 2)->execute();
}

function scratchpad_2_training_select_locale(&$install_state){
  $install_state['parameters']['locale'] = 'en';
}