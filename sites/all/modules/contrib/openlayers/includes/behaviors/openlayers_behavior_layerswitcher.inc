<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Layer Switcher Behavior
 */
class openlayers_behavior_layerswitcher extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'layerswitcher' => '',
      'ascending' => TRUE,
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.LayerSwitcher');
  }

  function options_form($defaults) {
    return array(
      'ascending' => array(
        '#type' => 'checkbox',
        '#title' => t('Show layers in ascending order'),
        '#description' => t('Higher layers shown toward the bottom'),
        '#default_value' => isset($defaults['ascending']) ? $defaults['ascending'] : TRUE 
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') . 
      '/includes/behaviors/js/openlayers_behavior_layerswitcher.js');
    return $this->options;
  }
}
