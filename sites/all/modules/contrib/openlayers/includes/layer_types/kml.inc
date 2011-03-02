<?php

/**
 * @file
 * KML Layer Type
 */

/**
 * OpenLayers KML Layer Type class
 */
class openlayers_layer_type_kml extends openlayers_layer_type {
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
      'layer_type' => 'kml',
      'layer_handler' => 'kml',
      'projection' => array('4326'),
      'baselayer' => FALSE,
      'vector' => TRUE,
      'formatOptions' => array(
        'extractStyles' => TRUE,
        'extractAttributes' => TRUE,
      ),
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    return array(
      'url' => array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#description' => t('The URL of the KML file.'),
        '#maxlength' => 1024,
        '#default_value' => isset($this->data['url']) ?
          $this->data['url'] : ''
      ),
      'formatOptions' => array(
        'extractStyles' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Styles'),
          '#description' => t('Extract styles from KML.'),
          '#default_value' => isset(
            $this->data['formatOptions']['extractStyles']) ?
            $this->data['formatOptions']['extractStyles'] : TRUE,
        ),
        'extractAttributes' => array(
          '#type' => 'checkbox',
          '#title' => t('Extract Attributes'),
          '#description' => t('Extract attributes from KML.'),
          '#default_value' => isset(
            $this->data['formatOptions']['extractAttributes']) ?
            $this->data['formatOptions']['extractAttributes'] : TRUE,
        ),
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_kml'
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') 
      . '/includes/layer_types/js/kml.js');
    return $this->options;
  }
}
