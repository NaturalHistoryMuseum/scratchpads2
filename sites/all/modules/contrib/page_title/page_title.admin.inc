<?php

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function page_title_admin_settings() {
  // Set the page title - the page is a local task now.
  drupal_set_title(t('Page titles'));


  // Set the theme callback for the patterns section
  $form['patterns'] = array(
    '#theme' => 'page_title_admin_settings',
  );

  $all_settings = page_title_get_settings();

  foreach ($all_settings as $key => $settings) {
    $form['patterns']['pattern'][$key] = array(
      '#title' => t($settings['label'], $settings['label arguments']),
      '#default_value' => variable_get($key, $settings['default']),
      '#required' => $settings['required'],
      '#description' => t($settings['description'], $settings['description arguments']),
      '#weight' => $settings['weight'],
      '#token_types' => $settings['scopes'],
      '#element_validate' => array('token_element_validate'),
      '#type' => 'textfield',
      '#size' => 30,
    );

    $form['patterns']['scope'][$key] = array(
      '#markup' => implode('<br />', array_map('_page_title_scope_t', $settings['scopes'])),
    );

    if ($settings['show field']) {
      $form['patterns']['showfield'][$key . '_showfield'] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get($key . '_showfield', 0),
      );
    }
  }


  // Add the token help to a collapsed fieldset at the end of the configuration page.
  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Tokens List'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['content'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node', 'comment', 'term', 'vocabulary', 'user'),
  );

  $form = system_settings_form($form);
  return $form;
}



function _page_title_scope_t($item) {
  return t(ucfirst($item));
}
