<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Map Form Values Behavior
 */
class openlayers_behavior_geofield extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'centerpoint' => array(
        'lat' => '#lat',
        'lon' => '#lon',
      ),
      'bounds' => array(
        '#left',
        '#bottom',
        '#right',
        '#top'
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'geofield') .'/includes/behaviors/js/openlayers_behavior_geofield.js');
    return $this->options;
  }
}
