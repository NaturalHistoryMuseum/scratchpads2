<?php

/**
 * @file
 * Theme settings for the Omega theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function beta_form_system_theme_settings_alter(&$form, &$form_state) {
  // Include any changes to the theme settings here. 
  // unset the CSS options as changing them will break the design
  $form['omega_general']['optional_css']['#access'] = FALSE;
  
  // change the menu form to only allow either default or none menu placements
  $menu_type = omega_theme_get_setting('omega_menu_type') ? omega_theme_get_setting('omega_menu_type') : 'drupal';
  $form['omega_general']['menu']['omega_menu_type'] = array(
    '#type' => 'radios',
    '#description' => t('Select the type of menus to generate.'),
    '#title' => t('Menu Type'),
    '#default_value' => $menu_type,
    '#options' => array(
      'drupal' => t('Drupal Default Primary/Secondary Menus (default)'),
      'none' => t('No Menu (Rely on menu blocks and/or contributed modules (Recommended)'),
    ),
  );
}
