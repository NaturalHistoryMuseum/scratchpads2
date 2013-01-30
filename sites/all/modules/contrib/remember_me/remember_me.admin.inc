<?php

/**
 * @file
 * Menu callback include file for admin/settings pages.
 */

/**
 * Implementation of menu callback.
 */
function remember_me_admin_settings($form, &$form_state) {
  global $user;
  $options = drupal_map_assoc(
    array(3600, 10800, 21600, 43200, 86400, 172800, 259200, 604800, 1209600, 2592000, 5184000, 7776000),
    'format_interval'
  );

  $vars = array(
    'remember' => array(
      '#type' => 'item',
      '#title' => t('Remember me'),
      '#value' => isset($user->data['remember_me']) ? t('Yes') : t('No'),
      '#description' => t('Current user chose at log in.'),
    ),
    'session' => array(
      '#type' => 'item',
      '#title' => t('Session lifetime'),
      '#value' => format_interval(ini_get('session.cookie_lifetime')),
      '#description' => t('Currently configured session cookie lifetime.'),
    ),
  );
  $form['legend'] = array(
    '#type' => 'markup',
    '#markup' => theme('remember_me_settings_display', array('vars' => $vars)),
  );

  $form['remember_me_managed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Manage session lifetime'),
    '#default_value' => variable_get('remember_me_managed', 0),
    '#description' => t('Choose to manually overwrite the configuration value from settings.php.'),
  );
  $form['remember_me_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Lifetime'),
    '#default_value' => variable_get('remember_me_lifetime', 604800),
    '#options' => $options,
    '#description' => t('Duration a user will be remembered for. This setting is ignored if Manage session lifetime (above) is disabled.'),
  );
  $form['remember_me_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember me field'),
    '#default_value' => variable_get('remember_me_checkbox', 1),
    '#description' => t('Default state of the "Remember me" field on the login forms.'),
  );
  $form['remember_me_checkbox_visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember me field visible'),
    '#default_value' => variable_get('remember_me_checkbox_visible', 1),
    '#description' => t('Should the "Remember me" field be visible on the login forms.'),
  );

  return system_settings_form($form);
}

/**
 * Theme callback function to dispaly status information.
 */
function theme_remember_me_settings_display($variables) {
  $rows = array(array());
  
  foreach ($variables['vars'] as $var) {
    $element = array(
      'element' => array(
          '#type' => 'markup',
          '#children' => $var['#value'],
          '#title' => $var['#title'],
          '#title_display' => 'before',
      )
    );

    $output = '<div class="container-inline">' . theme('form_element', $element) . '</div>';
    $output .= '<div class="description">' . $var['#description'] . '</div>';
    
    $rows[0][] = array( 'data' => $output);
  }
  return theme('table', array('rows' => $rows));
}
