<?php

/**
 * hashcash settings page
 */
function hashcash_admin_settings(){
  $form['hashcash_submit_disabled'] = array(
    '#type' => 'textarea',
    '#title' => t('Warning message for those with JavaScript disabled'),
    '#default_value' => t(variable_get('hashcash_submit_disabled', 'Submission of a form on this page has been disabled as you do not have JavaScript enabled in your browser.'))
  );
  $form['hashcash_addorignore'] = array(
    '#type' => 'radios',
    '#title' => t('Use hashcash on specific forms'),
    '#default_value' => variable_get('hashcash_addorignore', 0),
    '#options' => array(
      0 => t('Add to every form except the listed ones'),
      1 => t('Add to only the listed forms')
    )
  );
  $form['hashcash_form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Form IDs'),
    '#default_value' => variable_get('hashcash_form_ids', "user_login\nuser_login_block"),
    '#description' => t('Enter one form ID per line.')
  );
  $form['hashcash_add_warning'] = array(
    '#type' => 'radios',
    '#title' => t('Display warning if hashcash is required for a form, and user doesn\'t appear to have Javascript'),
    '#default_value' => variable_get('hashcash_add_warning', 0),
    '#options' => array(
      0 => t('Do not display warning'),
      1 => t('Display warning')
    )
  );
  $days = array(
    1 => 1,
    2 => 2,
    3 => 3,
    4 => 4,
    5 => 5,
    6 => 6,
    7 => 7
  );
  $form['hashcash_expire'] = array(
    '#type' => 'select',
    '#title' => t('Expire time (days)'),
    '#default_value' => variable_get('hashcash_expire', 2),
    '#description' => t('Time after which hashcash values can be repeated'),
    '#options' => $days
  );
  return system_settings_form($form);
}