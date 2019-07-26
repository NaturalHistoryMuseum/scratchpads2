<?php

/**
 * Submit function
 */
function gm3_filter_helper_form_submit(){
  dpm("Submitted");
}

/**
 * Function to return the form!
 */
function gm3_filter_helper_form($form, $form_state){
  $form = array();
  if(isset($form_state['input']) && count($form_state['input'])){
    $form['google_map_json'] = array(
      '#type' => 'textarea',
      '#title' => 'Google Map JSON',
      '#description' => 'Copy and paste this into a suitable text field',
      '#default_value' => 'Something'
    );
  }
  // Firstly we need to build a map array.
  $map = array(
    'id' => 'gm3-generator',
    'libraries' => array(
      'filter_helper' => array(
        'module' => 'gm3_filter'
      )
    ),
    'tools' => array()
  );
  $libraries_to_use = array(
    'point',
    'polygon',
    'polyline',
    'rectangle',
    'region'
  );
  $form = array_merge($form, array(
    'generate' => array(
      '#type' => 'submit',
      '#value' => t('Generate'),
      '#weight' => -5
    ),
    'zoom' => array(
      '#type' => 'hidden',
      '#title' => 'Zoom',
      '#attributes' => array(
        'class' => array(
          'gm3-generator-zoom'
        )
      )
    ),
    'center' => array(
      '#type' => 'hidden',
      '#title' => 'Center',
      '#attributes' => array(
        'class' => array(
          'gm3-generator-center'
        )
      )
    ),
    'maptypeid' => array(
      '#type' => 'hidden',
      '#title' => 'Map Type ID',
      '#attributes' => array(
        'class' => array(
          'gm3-generator-maptypeid'
        )
      )
    ),
    '#attached' => array(
      'library' => array(
        array(
          'gm3_filter',
          'filter_helper'
        )
      )
    )
  ));
  foreach($libraries_to_use as $library){
    $map['libraries'][$library] = array();
    $map['libraries']["field_$library"] = array(
      'module' => $library == 'region' ? 'gm3_region_field' : 'gm3_field'
    );
    $map['tools'][] = theme("gm3_{$library}_button");
    $form['generator_' . $library] = array(
      '#title' => ucfirst($library),
      '#type' => 'hidden',
      '#attributes' => array(
        'class' => array(
          'gm3-generator-' . $library
        )
      )
    );
  }
  $form['map'] = array(
    '#weight' => -10,
    '#markup' => drupal_render(theme('gm3_map', $map))
  );
  return $form;
}