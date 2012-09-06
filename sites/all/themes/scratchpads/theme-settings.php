<?php

/**
 * Implements hook_form_system_theme_settings_alter()
 */
function scratchpads_form_system_theme_settings_alter(&$form, &$form_state){
  $theme = alpha_get_theme();
  $form['logo']['resize_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Resize logo.'),
    '#default_value' => variable_get('resize_logo', 1)
  );
  $form['#submit'][] = 'scratchpads_form_system_theme_settings_submit';
}

function scratchpads_form_system_theme_settings_submit($form, &$form_state){
	variable_set('resize_logo', $form_state['values']['resize_logo']);
}