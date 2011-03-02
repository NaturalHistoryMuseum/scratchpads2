<?php

/**
 * @file
 *
 * OpenLayers CloudMade layer type
 */

/**
 * OpenLayers CloudMade Layer Type class
 */
class openlayers_layer_type_cloudmade extends openlayers_layer_type {
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
      'layer_type' => 'cloudmade',
      'layer_handler' => 'cloudmade',
      'key' => variable_get('openlayers_layers_cloudmade_api', ''),
      'projection' => array('900913'),
      'baselayer' => TRUE,
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    $warning = (!variable_get('openlayers_layers_cloudmade_js', FALSE) ||
      !variable_get('openlayers_layers_cloudmade_api', FALSE)) ?
        array(
          '#value' => t('WARNING: Your CloudMade API key or
          Javascript location is not set. Map including CloudMade layers
          will break until they are set correctly.')
        )
      : NULL;
    return array(
      'styleId' => array(
        '#type' => 'textfield',
        '#title' => t('Style ID'),
        '#description' => t('Enter the numeric Style ID for a <a href="http://maps.cloudmade.com/editor">Map Style</a> from CloudMade. You can usually find the Style ID in the lower right hand corner of each Map Style box.'),
        '#default_value' => isset($this->data['styleId']) ?
          $this->data['styleId'] : ''
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_cloudmade'
      ),
      $warning
    );
  }

  /**
   * Layer-type-wide settings form
   */
  function settings_form() {
    return array(
      'openlayers_layers_cloudmade_api' => array(
        '#type' => 'textfield',
        '#title' => t('CloudMade API Key'),
        '#default_value' => variable_get('openlayers_layers_cloudmade_api', ''),
        '#description' => t('<a href="@cloudmade">Get a CloudMade account and API Key</a>',
          array('@cloudmade' => 'http://cloudmade.com/user/show'))
      ),
      'openlayers_layers_cloudmade_js' => array(
        '#type' => 'textfield',
        '#title' => t('CloudMade Javascript Location'),
        '#default_value' => variable_get('openlayers_layers_cloudmade_js', ''),
        '#description' => t('The Drupal path or full URL to the location of the ' .
          '<a href="@url">Cloudmade OpenLayers Javascript library</a>', 
          array('@url' => 'http://developers.cloudmade.com/projects/show/openlayers-api')),
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    static $cloudmade_maps_included;
    
    if (!isset($cloudmade_maps_included)) {
      drupal_add_js(drupal_get_path('module', 'openlayers') . '/includes/layer_types/js/cloudmade.js');
      $path = variable_get('openlayers_layers_cloudmade_js', '');
      if (valid_url($path, TRUE)) {
        // TODO: could be collapsed, removing the if-else
        drupal_add_js($path, 'external');
      }
      else {
        drupal_add_js($path);
      }
      $cloudmade_maps_included = TRUE;
    }
    return $this->options;
  }
}
