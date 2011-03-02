<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Fullscreen Behavior
 */
class openlayers_behavior_fullscreen extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'activated' => FALSE
    );
  }

  /**
   * Provide form for configurations per map.
   */
  function options_form($defaults) {
    return array(
      'activated' => array(
        '#title' => t('Initially activated'),
        '#type' => 'checkbox',
        '#description' => t('Select to be in fullscreen by default.'),
        '#default_value' => isset($defaults['activated']) ?  $defaults['activated'] : FALSE
      )
    );
  }

  function js_dependency() {
    return array(
      'OpenLayers.Control.Button'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_css(drupal_get_path('module', 'openlayers') .
      '/includes/behaviors/js/openlayers_behavior_fullscreen.css');
    drupal_add_js(drupal_get_path('module', 'openlayers') .
      '/includes/behaviors/js/openlayers_behavior_fullscreen.js');
    return $this->options;
  }
}

