<?php

/**
 * @file
 * Admin part of CAPTCHA After module.
 */

/**
 * CAPTCHA after settings form.
 */
function captcha_after_settings() {

  $form['captcha_after_thresholds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global thresholds'),
    '#description' => t('Global CAPTCHA After thresholds. If some threshold is empty or 0 CAPTCHA After check will not be done at all for that threshold - this is usefull when you want to disable some checks. But be carefull with this - disabling all checks will disable CAPTCHA protection for selected forms.'),
    '#collapsible' => FALSE,
  );

  $form['captcha_after_thresholds']['captcha_after_submit_threshold'] = array(
    '#type' => 'textfield',
    '#title' => 'CAPTCHA After invalid submit threshold',
    '#description' => t('Number of times a user (based on Session ID) is permitted to submit non-valid data into the form in an hour before starting to protect form with CAPTCHA. Enter 0 to disable CAPTCHA After functionality.'),
    '#default_value' => variable_get('captcha_after_submit_threshold', 3),
  );

  $form['captcha_after_thresholds']['captcha_after_flooding_threshold'] = array(
    '#type' => 'textfield',
    '#title' => 'CAPTCHA flooding threshold',
    '#description' => t('Number of times a visitor (based on hostname/IP) is allowed to submit a protected form in an hour before starting to protect form with CAPTCHA. This is useful for protecting against repeated (but valid) submissions. Enter 0 to disable this behaviour.'),
    '#default_value' => variable_get('captcha_after_flooding_threshold', 3),
  );

  $form['captcha_after_thresholds']['captcha_after_global_flooding_threshold'] = array(
    '#type' => 'textfield',
    '#title' => 'CAPTCHA global flooding threshold',
    '#description' => t('Number of times <strong>ALL</strong> visitors are allowed to submit a protected form within an hour before starting to protect form with CAPTCHA. This is useful for protecting against flooding from multiple IPs. Enter 0 to disable this behaviour.'),
    '#default_value' => variable_get('captcha_after_global_flooding_threshold', 1000),
  );

  $captcha_forms = captcha_after_get_captcha_forms();
  $captcha_after_forms = captcha_after_db_get_forms();

  $form['captcha_after_forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Captcha protected forms'),
    '#description' => !empty($captcha_forms) ? t('Enable CAPTCHA After for following CAPTCHA protected forms. You can also overide global threshold values per form. If threshold value is empty then global configuration threshold value will be used.') : t('Configure CAPTCHA to protect at least one form in order to enable CAPTCHA After.'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );

  foreach ($captcha_forms as $form_id) {
    $form['captcha_after_forms'][$form_id] = array(
      '#type' => 'fieldset',
      '#title' => t('!form form', array('!form' => $form_id)),
      '#collapsible' => TRUE,
      '#collapsed' => isset($captcha_after_forms[$form_id]) ? !$captcha_after_forms[$form_id]['enable'] : TRUE,
    );
    
    $form['captcha_after_forms'][$form_id]['enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('enable captcha_after for <em>!form</em> form.', array('!form' => $form_id)),
      '#default_value' => isset($captcha_after_forms[$form_id]) ? $captcha_after_forms[$form_id]['enable'] : 0,
    );

    $form['captcha_after_forms'][$form_id]['submit_threshold'] = array(
      '#type' => 'textfield',
      '#title' => 'CAPTCHA After invalid submit threshold',
      '#default_value' => isset($captcha_after_forms[$form_id]) ? $captcha_after_forms[$form_id]['options']['submit_threshold'] : '',
    );

    $form['captcha_after_forms'][$form_id]['flooding_threshold'] = array(
      '#type' => 'textfield',
      '#title' => 'CAPTCHA flooding threshold',
      '#default_value' => isset($captcha_after_forms[$form_id]) ? $captcha_after_forms[$form_id]['options']['flooding_threshold'] : '',
    );

    $form['captcha_after_forms'][$form_id]['global_flooding_threshold'] = array(
      '#type' => 'textfield',
      '#title' => 'CAPTCHA global flooding threshold',
      '#default_value' => isset($captcha_after_forms[$form_id]) ? $captcha_after_forms[$form_id]['options']['global_flooding_threshold'] : '',
    );
  }

  $form['#submit'][] = 'captcha_after_settings_form_submit';
  
  return system_settings_form($form);

}

/**
 * CAPTCHA after settings form submit.
 */
function captcha_after_settings_form_submit($form, &$form_values) {  
  foreach ($form_values['values']['captcha_after_forms'] as $form_id  => $options) {
    captcha_after_db_set_form($form_id, $options);
  }
  
  // Delete captcha_after values so they are not saved in system_settings_form_submit.
  unset($form_values['values']['captcha_after_forms']);
}

/**
 * Returns array of all forms that are protected by CAPTCHA module.
 *
 * @return
 *   Array of forms ids.
 */
function captcha_after_get_captcha_forms() {
  $forms = array();

  $result = db_query('SELECT form_id FROM {captcha_points} WHERE module IS NOT NULL OR captcha_type IS NOT NULL ORDER BY form_id');
  foreach ($result as $form) {
    $forms[$form->form_id] = $form->form_id;
  }

  return $forms;
}
