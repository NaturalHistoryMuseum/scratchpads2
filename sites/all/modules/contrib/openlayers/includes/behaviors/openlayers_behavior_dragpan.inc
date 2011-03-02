<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * DragPan Behavior
 */
class openlayers_behavior_dragpan extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'navigation' => '',
    );
  }

  function js_dependency() {
    return array('OpenLayers.Control.DragPan');
  }

  function options_form($defaults) {
    return array();
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') . 
      '/includes/behaviors/js/openlayers_behavior_dragpan.js');
    return $this->options;
  }
}
