<?php

/**
 * @file
 * Contains the theme settings form elements for the layouts extension.
 */

/**
 * Implements hook_extension_EXTENSION_theme_settings_form_alter().
 */
function omega_extension_layouts_settings_form($element, &$form, $form_state) {
  if ($layouts = omega_layouts_info()) {
    $element['omega_layout'] = array(
      '#type' => 'radios',
      '#options' => array(),
      '#title' => t('Choose a layout'),
      '#default_value' => omega_theme_get_setting('omega_layout', 'simple'),
    );

    foreach ($layouts as $layout => $info) {
      $element['omega_layout']['#options'][$layout] = $info['info']['name'];
      $element['omega_layout'][$layout]['#description'] = $info['info']['description'];

      if (isset($info['info']['preview'])) {
        $icon = file_create_url($info['path'] . '/' . $info['info']['preview']);
        $element['omega_layout'][$layout]['#prefix'] = '<div class="omega-layout-selection-wrapper">';
        $element['omega_layout'][$layout]['#suffix'] = '<img class="omega-layout-icon" src="' . $icon . '" /></div>';
      }
    }
  }

  return $element;
}
