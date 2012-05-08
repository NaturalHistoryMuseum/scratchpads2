<?php

/**
 * @file
 * Administrative functions / callbacks.
 */

/**
 * Menu callback for generating a countries settings admin form.
 */
function countries_configuration_options_form($form, &$form_state, $country, $name) {
  $config_options = countries_configuration_options();
  if (!isset($config_options[$name])) {
    drupal_access_denied();
    return;
  }

  $info = $config_options[$name];

  $form_state['is_update'] = db_select('countries_data', 'cd')
        ->fields('cd', array('iso2'))
        ->condition('iso2', $country->iso2)
        ->condition('name', $name)
        ->execute()
        ->fetchCol();

  if (!empty($info['file'])) {
    $file_path = empty($info['file path']) ? drupal_get_path('module', $info['module']) : $info['file path'];
    require_once DRUPAL_ROOT . '/' . $file_path . '/' . $info['file'];
  }

  if (isset($info['title callback']) && $title = $info['title callback']($country)) {
    drupal_set_title($title);
  }

  $form = array('#tree' => TRUE);
  if (!empty($info['help'])) {
    $form['help'] = array(
      '#markup' => '<p>' . $info['help'] . '</p>',
    );
  }
  $form['iso2'] = array(
    '#type' => 'value',
    '#value' => $country->iso2,
  );
  $form['module'] = array(
    '#type' => 'value',
    '#value' => $info['module'],
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );

  $values = countries_configuration($country, $name);
  $form['data'] = $info['form callback']($country, $values, $form);

  // Allow other modules to hook directly to the form data.
  $addtional = array(
    'form_state' => &$form_state,
    'complete_form' => &$form,
    'country' => $country,
  );
  drupal_alter('countries_configuration_form_' . $name, $form['data'], $values, $addtional);

  // Allow these hooks to define their own submit handlers.
  $reset_handlers = empty($form['reset']['#submit']) ? array() : $form['reset']['#submit'];
  $reset_handlers[] = 'countries_configuration_options_form_reset_submit';
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => $reset_handlers,
    '#weight' => 98,
  );
  // Move specified submit handlers onto the base form.
  $handlers = empty($form['submit']['#submit']) ? array() : $form['submit']['#submit'];
  $handlers[] = 'countries_configuration_options_form_save_submit';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $handlers,
    '#weight' => 99,
  );

  return $form;
}

/**
 * Reset submit handler for countries_configuration_options_form().
 *
 * This saves an empty array() to the database, resulting in the default values
 * being loaded when the record is loaded again.
 */
function countries_configuration_options_form_reset_submit($form, &$form_state) {
  // This will reset to the default settings on load.
  db_delete('countries_data')->condition('iso2', $form_state['values']['iso2'])
      ->condition('name', $form_state['values']['name'])->execute();
}

/**
 * Standard save submit handler for countries_configuration_options_form().
 */
function countries_configuration_options_form_save_submit($form, &$form_state) {
  drupal_write_record('countries_data', $form_state['values'], $form_state['is_update'] ? array('iso2', 'name') : array());
}
