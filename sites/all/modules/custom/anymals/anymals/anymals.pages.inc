<?php

/**
 * The settings form for the anymals module.
 */
function anymals_configuration_form(){
  $options = array();
  foreach(node_type_get_types() as $key => $type){
    $options[$key] = $type->name;
  }
  $form = array(
    'anymals_key' => array(
      '#type' => 'textfield',
      '#title' => t('anymals+plants Key'),
      '#default_value' => variable_get('anymals_key', ''),
      '#description' => t('Your anymals+plants key, as found in the anymals+plants interface.')
    ),
    'anymals_token' => array(
      '#type' => 'textfield',
      '#title' => t('anymals+plants Token'),
      '#default_value' => variable_get('anymals_token', ''),
      '#description' => t('Your anymals+plants token, as found in the anymals+plants interface.')
    ),
    'advanced' => array(
      '#type' => 'fieldset',
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      '#title' => t('Advanced'),
      'anymals_forms' => array(
        '#type' => 'checkboxes',
        '#title' => t('Forms/Content types'),
        '#description' => t('Select which content types you would like to use with anymals+plants.  We suggest just using one - Specimen/Observation'),
        '#options' => $options,
        '#default_value' => variable_get('anymals_forms', array(
          'specimen_observation'
        ))
      )
    ),
    '#submit' => array(
      'anymals_configuration_form_submit'
    )
  );
  return system_settings_form($form);
}

/**
 * Submit function for the above form.  This simply sends a quick ping to the
 * anymals interface to let them know they can download updated forms.
 */
function anymals_configuration_form_submit($form, $form_state){
  // FIXME - Check the response of the XML to ensure anymals+plants is happy.
  $response = drupal_http_request('https://www.anymals.org/intercom/?request=addSightingFormByURL&token=' . $form_state['values']['anymals_token'] . '&key=' . $form_state['values']['anymals_key'] . '&xmlurl=' . url('ecml/forms/node/' . implode(',', array_filter($form_state['values']['anymals_forms'])), array(
    'absolute' => TRUE
  )), array(
    'method' => 'POST'
  ));
  watchdog('anymals', 'Response from anymals+plants server: !response', array(
    '!response' => print_r($response, 1)
  ));
}