<?php

/**
 * hashcash settings page
 */
function hashcash_admin_settings(){
  return system_settings_form(array(
    'hashcash_log' => array(
      '#type' => 'checkbox',
      '#title' => t('Log failed hashcash'),
      '#description' => t('Report information about failed attempts to the !watchdoglog.', array('!watchdoglog' => l(t('log'), 'admin/reports/dblog'))),
      '#default_value' => variable_get('hashcash_log', 0)
    ),
    'hashcash_addorignore' => array(
      '#type' => 'radios',
      '#title' => t('Use hashcash on specific forms'),
      '#default_value' => variable_get('hashcash_addorignore', 0),
      '#options' => array(
        0 => t('Add to every form except the listed ones'),
        1 => t('Add to only the listed forms')
      )
    ),
    'hashcash_form_ids' => array(
      '#type' => 'textarea',
      '#title' => t('Form IDs'),
      '#default_value' => variable_get('hashcash_form_ids', "user_login\nuser_login_block"),
      '#description' => t('Enter one form ID per line.')
    ),
    'hashcash_expire' => array(
      '#type' => 'select',
      '#title' => t('Expire time (days)'),
      '#default_value' => variable_get('hashcash_expire', 2),
      '#description' => t('Time after which hashcash values can be repeated'),
      '#options' => array(
        1 => 1,
        2 => 2,
        3 => 3,
        4 => 4,
        5 => 5,
        6 => 6,
        7 => 7
      )
    )
  ));
}