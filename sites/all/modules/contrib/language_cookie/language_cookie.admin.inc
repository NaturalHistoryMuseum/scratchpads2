<?php
/**
 * @file
 * Language Cookie admin settings form.
 */

/**
 * The URL language provider configuration form.
 */
function language_cookie_admin_form($form, &$form_state) {
  $form['language_cookie_param'] = array(
    '#title' => t('Cookie parameter'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_param', 'language'),
    '#description' => t('Name of the cookie parameter used to determine the desired language.'),
  );

  $form['language_cookie_time'] = array(
    '#title' => t('Cookie duration'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_time', 31536000),
    '#description' => t('The time the cookie expires. This is the number of seconds from the current time.'),
  );

  $form['language_cookie_path'] = array(
    '#title' => t('Cookie path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_path', '/'),
    '#description' => t('The cookie available server path'),
  );

  $form['language_cookie_domain'] = array(
    '#title' => t('Cookie domain scope'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_cookie_domain', ''),
    '#description' => t('The cookie domain scope'),
  );

  $form_state['redirect'] = 'admin/config/regional/language/configure';
  return system_settings_form($form);
}