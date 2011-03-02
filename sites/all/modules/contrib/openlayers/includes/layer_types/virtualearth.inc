<?php

/**
 * @file
 *
 * OpenLayers VirtualEarth layer type
 */

/**
 * OpenLayers VirtualEarth Layer Type class
 */
class openlayers_layer_type_virtualearth extends openlayers_layer_type {
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
      'layer_type' => 'virtualearth',
      'layer_handler' => 'virtualearth',
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    return array(
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_virtualearth'
      ),
    );
  }

  /**
   * Layer-type-wide settings form
   */
  function settings_form() {
    return array(
      'openlayers_layers_virtualearth_api' => array(
        '#type' => 'textfield',
        '#title' => t('VirtualEarth API Key'),
        '#default_value' => variable_get('openlayers_layers_virtualearth_api', ''),
        '#description' => t('<a href="@microsoft">Get a Virtual Earth API Key</a>',
          array('@microsoft' => 'http://www.microsoft.com/maps/developers/'))
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    static $virtualearth_maps_included;
    
    if (!isset($virtualearth_maps_included)) {
      drupal_add_js(drupal_get_path('module', 'openlayers')
        . '/includes/layer_types/js/virtualearth.js');
      drupal_add_js('http://dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=6.1',
        'external');
      $virtualearth_maps_included = TRUE;
    }
    return $this->options;
  }
}
