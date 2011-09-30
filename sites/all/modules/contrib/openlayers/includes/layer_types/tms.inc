<?php

/**
 * @file
 * TMS Layer Type
 */

/**
 * OpenLayers TMS Layer Type class
 */
class openlayers_layer_type_tms extends openlayers_layer_type {

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
      'serverResolutions' => openlayers_get_resolutions('900913'),
      'maxExtent' => openlayers_get_extent('900913'),
      'projection' => array('900913'),
      'baselayer' => TRUE,
      'layer_handler' => 'tms',
      'type' => 'png',
      'wrapDateLine' => false,
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
          $this->data['base_url'] : '',
        '#description' => t('This is the base URL of the TMS service.  For example, if this was your tile scheme URL: http://example.com/tms/1.0.0/layer_name/{z}/{y}/{x}.png, then <em>http://example.com/tms/</em> would be the Base URL.'),
      ),
      'layername' => array(
        '#type' => 'textfield',
        '#title' => t('Layer Name'),
        '#default_value' => isset($this->data['layername']) ?
          $this->data['layername'] : '',
        '#description' => t('This is the base URL of the TMS service.  For example, if this was your tile scheme URL: http://example.com/tms/1.0.0/layer_name/{z}/{y}/{x}.png, then <em>layer_name</em> would be the Layer Name.'),
      ),
      'baselayer' => array(
        '#type' => 'checkbox',
        '#default_value' => $this->data['baselayer'],
        '#title' => t('BaseLayer'),
        '#description' => t('Uncheck to make this map an overlay.')
      ),
      'type' => array(
        '#type' => 'select',
        '#title' => t('File Format'),
        '#options' => array(
          'png' => 'png',
          'jpg' => 'jpg'),
        '#default_value' => isset($this->data['type']) ?
          $this->data['type'] : 'png'
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
          array_map('strval', openlayers_get_resolutions('900913')),
        '#description' => t('The available zoom levels for the tiles.'),
      ),
      'wrapDateLine' => array(
        '#type' => 'checkbox',
        '#title' => t('Wrap Dateline'),
        '#default_value' => isset($this->data['type']) ?
          $this->data['type'] : False,
        '#description' => t('Wrapping the dateline will cause the map to appear to repeat itself when going east or west.  This may not be supported by all tilesets.'),
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_tms'
      ),
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers')
      . '/includes/layer_types/js/tms.js');
    return $this->options;
  }
}
