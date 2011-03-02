<?php

/**
 * @file
 * TileWarper Layer Type
 */

/**
 * OpenLayers MapTiler Layer Type class
 */
class openlayers_layer_type_maptiler extends openlayers_layer_type {

  function __construct($layer = array(), $map = array()) {
    parent::__construct($layer, $map);
    if (isset($this->data)) {
      $this->data += $this->options_init();
      $this->data['baselayer'] = $this->data['isBaseLayer'];
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
      'serverResolutions' => openlayers_get_resolutions('900913'),
      'maxExtent' => openlayers_get_extent('900913'),
      'projection' => array('900913'),
      'baselayer' => TRUE,
      'type' => 'png',
      'layer_handler' => 'maptiler',
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form() {
    return array(
      'base_url' => array(
        '#type' => 'textfield',
        '#title' => t('Base URL'),
        '#default_value' => isset($this->data['base_url']) ? 
          $this->data['base_url'] : ''
      ),
      'isBaseLayer' => array(
        '#type' => 'checkbox',
        '#default_value' => isset($this->data['isBaseLayer']) ?
          $this->data['isBaseLayer'] : TRUE,
        '#title' => t('BaseLayer'),
        '#description' => t('Uncheck to make this map an overlay')
      ),
      'resolutions' => array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => array_combine(
          array_map('strval', openlayers_get_resolutions('900913')),
          range(0, 18)
        ),
        '#title' => t('Zoom Level Range'),
        '#default_value' => isset($this->data['resolutions']) ?
          $this->data['resolutions'] :
          array_map('strval', openlayers_get_resolutions('900913'))
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_maptiler'
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers') 
      . '/includes/layer_types/js/maptiler.js');
    return $this->options;
  }
}
