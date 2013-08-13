<?php

/**
 * Admin settings
 */
function gm3_admin_settings_form(){
  $settings = gm3_settings();
  return array(
    'dimensions' => array(
      '#type' => 'fieldset',
      '#title' => t('Dimensions'),
      'width' => array(
        '#type' => 'textfield',
        '#default_value' => $settings['width'],
        '#description' => t('CSS width, e.g. 100%, 250px')
      ),
      'height' => array(
        '#type' => 'textfield',
        '#default_value' => $settings['height'],
        '#description' => t('CSS height, e.g. 500px')
      )
    ),
    'centre' => array(
      '#type' => 'fieldset',
      '#title' => t('Centre point'),
      'latitude' => array(
        '#title' => t('Latitude'),
        '#type' => 'textfield',
        '#default_value' => $settings['center']['latitude']
      ),
      'longitude' => array(
        '#title' => t('Longitude'),
        '#type' => 'textfield',
        '#default_value' => $settings['center']['longitude']
      )
    ),
    'minZoom' => array(
      '#type' => 'select',
      '#title' => t('Zoom'),
      '#default_value' => $settings['minZoom'],
      '#options' => drupal_map_assoc(range(2, 10))
    ),
    'advanced' => array(
      '#type' => 'fieldset',
      '#title' => t('Advanced'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      'gm3_settings' => array(
        '#title' => t('GM3 Settings'),
        '#type' => 'textarea',
        '#default_value' => serialize(gm3_settings()),
        '#rows' => 10
      ),
      'warning' => array(
        '#markup' => '<p style="color:red">' . t('Only change this if you know what you are doing.') . '</p>'
      )
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save')
    )
  );
}

/**
 * Submit function for the admin settings.
 */
function gm3_admin_settings_form_validate(&$form, &$form_state){
  if(!unserialize($form_state['values']['gm3_settings'])){
    form_set_error('gm3_settings', t('Failed to unserialize your GM3 settings'));
  }
}

/**
 * Submit function for the admin settings.
 */
function gm3_admin_settings_form_submit(&$form, &$form_state){
  $settings = unserialize($form_state['values']['gm3_settings']);
  $settings['width'] = $form_state['values']['width'];
  $settings['height'] = $form_state['values']['height'];
  $settings['minZoom'] = $form_state['values']['minZoom'];
  $settings['center']['latitude'] = $form_state['values']['latitude'];
  $settings['center']['longitude'] = $form_state['values']['longitude'];
  variable_set('gm3_default_settings', $settings);
}

/**
 * Test page
 */
function gm3_test_page(){
  $results = db_select('gm3_region_data', 'g')->fields('g', array(
    'level_1_code',
    'level_2_code',
    'level_3_code',
    'level_4_code'
  ))->condition('level_4_code', '', '!=')->condition('level_3_code', 'ANT', '!=')->execute();
  foreach($results as $row){
    $regions[] = "{$row->level_1_code}:{$row->level_2_code}:{$row->level_3_code}:{$row->level_3_code}-{$row->level_4_code}";
  }
  return array(
    'map' => array(
      '#theme' => 'gm3_map',
      '#map' => array(
        'libraries' => array(
          'region' => array(
            'module' => 'gm3_region',
            'regions' => $regions
          )
        )
      )
    )
  );
}