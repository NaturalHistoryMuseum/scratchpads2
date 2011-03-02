<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Navigation Behavior
 */
class openlayers_behavior_navigation extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'navigation' => '',
      'zoomWheelEnabled' => TRUE
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.Navigation');
  }

  function options_form($defaults) {
    return array(
      'zoomWheelEnabled' => array(
        '#type' => 'checkbox',
        '#title' => t('Enable Zoom Wheel'),
        '#description' => t('Enable zooming via the mouse scroll wheel. This can 
          interfere with page scrolling.'),
        '#default_value' => isset($defaults['zoomWheelEnabled']) ? $defaults['zoomWheelEnabled'] : TRUE 
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') 
      . '/includes/behaviors/js/openlayers_behavior_navigation.js');
    return $this->options;
  }
}
