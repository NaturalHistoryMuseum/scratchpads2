<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * PermaLink Behavior
 */
class openlayers_behavior_permalink extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'permalink' => '',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.Permalink');
  }

  function options_form($defaults) {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') . 
      '/includes/behaviors/js/openlayers_behavior_permalink.js');
    return $this->options;
  }
}
