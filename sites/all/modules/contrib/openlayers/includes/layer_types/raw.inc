<?php

/**
 * @file
 * OpenLayers Raw Layer Type
 */
class openlayers_layer_type_raw extends openlayers_layer_type {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layer_type' => 'openlayers_layer_type_raw',
      'layer_handler' => 'openlayers_raw',
      'vector' => TRUE,
      'baselayer' => FALSE,
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    return array(
      array(
        '#value' => t('Raw layers should not be added
        through the UI.')
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') .
      '/includes/layer_types/js/raw.js');
    // $features = ;
    // $this->data['features'] = $features;
    return $this->options;
  }
}

