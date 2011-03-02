<?php

/**
 * @file
 * Yahoo Layer Type
 */

/**
 * OpenLayers Yahoo Layer Type class
 */
class openlayers_layer_type_yahoo extends openlayers_layer_type {
  function __construct($layer = array(), $map = array()) {
    parent::__construct($layer, $map);
    if (isset($this->data)) {
      $this->data += $this->options_init();
    }
    else {
      $this->data = $this->options_init();
    }
  }
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'projection' => array('900913'),
      'layer_type' => 'yahoo',
      'layer_handler' => 'yahoo',
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    $warning = (!variable_get('openlayers_layers_yahoo_api', FALSE)) ?
        array(
          '#value' => t('WARNING: Your Yahoo API key is not set.
          Map including Yahoo layers
          will break until it is set correctly.')
        )
      : NULL;

    $yahoo_layer_types = array(
      'hybrid' => 'Hybrid',
      'street' => 'Street',
      'satellite' => 'Satellite');
    return array(
      'type' => array(
        '#title' => t('Yahoo Layer Type'),
        '#type' => 'select',
        '#default_value' => isset($this->data['type']) ?
          $this->data['type'] : 'normal',
        '#options' => $yahoo_layer_types
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_yahoo'
      ),
      $warning
    );
  }

  function settings_form() {
    return array(
      'openlayers_layers_yahoo_api' => array(
        '#type' => 'textfield',
        '#title' => t('Yahoo API Key'),
        '#default_value' => variable_get('openlayers_layers_yahoo_api', ''),
      )
    );
  }


  /**
   * Render.
   */
  function render(&$map) {
    static $yahoo_maps_included;
    
    if (!isset($yahoo_maps_included)) {
      drupal_add_js(drupal_get_path('module', 'openlayers')
        . '/includes/layer_types/js/yahoo.js');
      drupal_add_js("http://api.maps.yahoo.com/ajaxymap?v=3.0&appid=" .
        variable_get('openlayers_layers_yahoo_api', ''),
        'external');
      $yahoo_maps_included = TRUE;
    }
    return $this->options;
  }
}
