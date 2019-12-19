<?php

/**
 * @file
 * Theme settings file for the Ohm theme.
 */

require_once dirname(__FILE__) . '/template.php';

/**
 * Implements hook_form_FORM_alter().
 */
function ohm_form_system_theme_settings_alter(&$form, $form_state) {
  $form['ohm_enable_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a warning when this theme is used'),
    '#description' => t("You can permanently hide this message, but please be aware that Ohm is a demonstration subtheme and will therefore be constantly evolving with latest best practices. Explore, break it and learn but don't use it in production directly, or as a base theme."),
    '#default_value' => omega_theme_get_setting('ohm_enable_warning', TRUE),
    '#weight' => -20,
  );
}
