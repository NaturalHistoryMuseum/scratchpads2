<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Mouse Potision Behavior
 */
class openlayers_behavior_overviewmap extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'overviewmap' => '',
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    // drupal_add_js(drupal_get_path('module', 'openlayers') .'/includes/behaviors/js/openlayers_behavior_mouseposition.js');
    return $this->options;
  }
}
