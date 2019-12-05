<?php

/**
 * Admin settings
 */
function gm3_admin_settings_form(){
  $settings = gm3_settings();

  return [
    'dimensions' => [
      '#type' => 'fieldset',
      '#title' => t('Dimensions'),
      'width' => [
        '#title' => t('Width'),
        '#type' => 'textfield',
        '#default_value' => $settings['width'],
        '#description' => t('CSS width, e.g. 100%, 250px')
      ],
      'height' => [
        '#title' => t('Height'),
        '#type' => 'textfield',
        '#default_value' => $settings['height'],
        '#description' => t('CSS height, e.g. 500px')
      ]
    ],
    'centre' => [
      '#type' => 'fieldset',
      '#title' => t('Centre point'),
      'latitude' => [
        '#title' => t('Latitude'),
        '#type' => 'textfield',
        '#default_value' => $settings['center']['latitude']
      ],
      'longitude' => [
        '#title' => t('Longitude'),
        '#type' => 'textfield',
        '#default_value' => $settings['center']['longitude']
      ]
    ],
    'minZoom' => [
      '#type' => 'select',
      '#title' => t('Minimum zoom level'),
      '#default_value' => $settings['minZoom'],
      '#options' => drupal_map_assoc(range(2, 10))
    ],
    'enableClustering' => [
      '#type' => 'checkbox',
      '#title' => t('Enable clustering'),
      '#default_value' => $settings['enableClustering'],
      '#description' => t('For points that are close together at lower zoom levels, shows the number of points in that area instead of the individual points themselves.')
    ],
    'submit' => [
      '#type' => 'submit',
      '#value' => t('Save')
    ]
  ];
}

/**
 * Submit function for the admin settings.
 */
function gm3_admin_settings_form_submit(&$form, &$form_state){
  $settings = [
    'width' => $form_state['values']['width'],
    'height' => $form_state['values']['height'],
    'minZoom' => $form_state['values']['minZoom'],
    'zoom' => $form_state['values']['minZoom'],
    'center' => [
      'latitude' => $form_state['values']['latitude'],
      'longitude' => $form_state['values']['longitude']
    ],
    'enableClustering' => $form_state['values']['enableClustering']
  ];
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
