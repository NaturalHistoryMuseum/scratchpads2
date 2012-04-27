<?php

/**
 * Implements hook_install_tasks_alter().
 */
function scratchpad_2_sandbox_install_tasks_alter(&$tasks, &$install_state){
  $tasks['install_select_locale'] = array(
    'function' => 'scratchpad_2_sandbox_select_locale'
  );
  $tasks['install_configure_form'] = array(
    'function' => 'scratchpad_2_sandbox_install_configure_form'
  );
}

/**
 * scratchpad_2_sandbox_install_task_one().
 */
function scratchpad_2_sandbox_install_configure_form(){}

function scratchpad_2_sandbox_select_locale(&$install_state){
  $install_state['parameters']['locale'] = 'en';
}