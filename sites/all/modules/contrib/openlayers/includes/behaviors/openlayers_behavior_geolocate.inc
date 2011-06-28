<?php

/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Zoom Panel Behavior
 * http://dev.openlayers.org/docs/files/OpenLayers/Control/Geolocate-js.html
 */
class openlayers_behavior_geolocate extends openlayers_behavior {
  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'bind' => TRUE,
      'watch' => FALSE,
      'geolocationOptions' => array(
        'enableHighAccuracy' => FALSE,
        'maximumAge' => 0,
        'timeout' => 7000,
      ),
      'zoom_level' => 12,
    );
  }

  function options_form($defaults) {
    $intials = $this->options_init();
  
    return array(
      'bind' => array(
        '#title' => t('Center when located'),
        '#type' => 'select',
        '#options' => array(
          TRUE => t('Yes'),
          FALSE => t('No'),
        ),
        '#description' => t('When enabled, if the geolocation control finds a location, it will set the center of the map at this point.'),
        '#default_value' => isset($defaults['bind']) ? $defaults['bind'] : $intials['bind'],
      ),
      'zoom_level' => array(
        '#title' => t('Zoom level'),
        '#type' => 'textfield',
        '#description' => t('An integer zoom level for when a location is found.  0 is the most zoomed out and higher numbers mean more zoomed in (the number of zoom levels depends on your map).'),
        '#default_value' => isset($defaults['zoom_level']) ? $defaults['zoom_level'] : $intials['zoom_level'],
      ),
      'watch' => array(
        '#title' => t('Watch'),
        '#type' => 'select',
        '#options' => array(
          TRUE => t('Yes'),
          FALSE => t('No'),
        ),
        '#description' => t('When enabled, the map will continue to try to update the location of the device.'),
        '#default_value' => isset($defaults['watch']) ? $defaults['watch'] : $intials['watch'],
      ),
    );
  }

  function js_dependency() {
    return array(
      'OpenLayers.Control.Geolocate'
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') . 
      '/includes/behaviors/js/openlayers_behavior_geolocate.js');
    return $this->options;
  }
}
