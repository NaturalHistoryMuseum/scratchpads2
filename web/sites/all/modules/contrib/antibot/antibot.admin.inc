<?php

/**
 * @file
 * Admin callbacks and functions.
 */

/**
 * Admin settings form.
 */
function antibot_admin_settings($form, &$form_state) {
  $form['message'] = array(
    '#type' => 'html_tag',
    '#tag' => 'h3',
    '#value' => t('Antibot requires that a user has JavaScript enabled in order to use and submit a given form.'),
  );
  $form['antibot_form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Form IDs'),
    '#default_value' => antibot_active_form_ids(),
    '#description' => t('Specify the form IDs that should be protected by Antibot. Each form ID should be on a separate line. Wildcard (*) characters can be used.'),
  );
  $form['antibot_show_form_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display form IDs'),
    '#default_value' => variable_get('antibot_show_form_ids', 0),
    '#description' => t('When enabled, the form IDs of all forms on every page will be displayed to any user with permission to access these settings. Also displayed will be whether or not Antibot is enabled for each form. This should only be turned on temporarily in order to easily determine the form IDs to use.'),
  );

  return system_settings_form($form);
}
