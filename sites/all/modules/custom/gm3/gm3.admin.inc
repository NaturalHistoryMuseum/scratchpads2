<?php

/**
 * Admin settings
 */
function gm3_admin_settings_form(){
  return array(
    'gm3_settings' => array(
      '#title' => t('GM3 Settings'),
      '#type' => 'textarea',
      '#default_value' => serialize(gm3_settings()),
      '#rows' => 10
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save')
    ),
    '#submit' => array(
      'gm3_admin_settings_form_submit'
    )
  );
}

/**
 * Submit function for the admin settings.
 */
function gm3_admin_settings_form_submit(&$form, &$form_state){
  $data = unserialize($form_state['values']['gm3_settings']);
  if($data){
    variable_set('gm3_default_settings', $data);
  }
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