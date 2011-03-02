<?php

/**
 * @file
 * WMS Layer Type
 */

/**
 * OpenLayers WMS Layer Type class
 */
class openlayers_layer_type_wms extends openlayers_layer_type {
  function __construct($layer = array(), $map = array()) {
    parent::__construct($layer, $map);
    if (isset($this->data)) {
      if (isset($this->data['options']['srs'])) {
        $pts = explode(':', $this->data['options']['srs']);
        $this->data['projection'] = array($pts[1]);
        $this->data['params']['resolutions'] = openlayers_get_resolutions($pts[1]);
        $this->data['params']['projection'] = $this->data['options']['srs']; 
        $this->data['params']['maxextent'] =  openlayers_get_extent($pts[1]);
      }
      $this->data['baselayer'] = $this->data['params']['isBaseLayer'];
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
      'layer_handler' => 'wms',
      'baselayer' => FALSE
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
      
      // TODO: swap terms
      'params' => array(
        'isBaseLayer' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($this->data['params']['isBaseLayer']) ? 
            $this->data['params']['isBaseLayer'] : TRUE,
          '#return_value' => 'true',
          '#title' => t('BaseLayer'),
          '#description' => t('Uncheck to make this map an overlay')
        ),
        // TODO: validate the field, only positive integers shall be allowed
        'buffer' => array(
          '#type' => 'textfield',
          '#default_value' => isset($this->data['params']['buffer']) ?
            $this->data['params']['buffer'] : 2,
          '#title' => t('Buffer'),
          '#description' => t('Used only when not in single-tile mode, this specifies umber of extra rows and colums of tiles on each side which will surround the minimum grid tiles to cover the map')
        ),
        // TODO: validate the field, only positive numbers shall be allowed
        //       numbers below 1 might also not make much sense
        'ratio' => array(
          '#type' => 'textfield',
          '#default_value' => isset($this->data['params']['ratio']) ?
            $this->data['params']['ratio'] : 1.5,
          '#title' => t('Ratio'),
          '#description' => t('Used only when in single-tile mode, this specifies the ratio of the size of the single tile to the size of the map')
        ),
        'singleTile' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($this->data['params']['singleTile']) ?
            $this->data['params']['singleTile'] : FALSE,
          '#title' => t('Single tile'),
          '#description' => t('Check to make this layer untiled')
        )
      ),
      'options' => array(
        'srs' => array(
          '#type' => 'select',
          '#title' => t('Projection'),
          '#options' => array(
            'EPSG:900913' => '900913',
            'EPSG:4326' => '4326'),
          '#default_value' => isset($this->data['options']['srs']) ?
            $this->data['options']['srs'] : '900913'
        ),
        'TRANSPARENT' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($this->data['options']['TRANSPARENT']) ?
            $this->data['options']['TRANSPARENT'] : FALSE,
          '#return_value' => 'true',
          '#title' => t('Transparent'),
          '#description' => t('When a PNG, make the background color transparent')
        ),
         'exceptions' => array(
          '#type' => 'select',
          '#title' => t('Exceptions'),
          '#options' => array(
            'application/vnd.ogc.se_xml' => 'application/vnd.ogc.se_xml',
            'application/vnd.ogc.se_inimage' => 'application/vnd.ogc.se_inimage'),
          '#default_value' => isset($this->data['options']['exceptions']) ?
            $this->data['options']['exceptions'] : 'application/vnd.ogc.se_inimage',
          '#description' => t('Select the exception handler')
        ),
        'format' => array(
          '#type' => 'select',
          '#title' => t('File Format'),
          '#options' => array(
            'image/png' => 'image/png',
            'image/gif' => 'image/gif',
            'image/jpeg' => 'image/jpeg'),
          '#default_value' => isset($this->data['options']['format']) ?
            $this->data['options']['format'] : 'image/png'
        ),
        'layers' => array(
          '#type' => 'textfield',
          '#title' => t('Layers'),
          '#default_value' => isset($this->data['options']['layers']) ?
            $this->data['options']['layers'] : ''
        ),
        'styles' => array(
          '#type' => 'textfield',
          '#title' => t('Styles'),
          '#default_value' => isset($this->data['options']['styles']) ?
            $this->data['options']['styles'] : ''
        ),
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_layer_type_wms'
      )
    );
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers')
      . '/includes/layer_types/js/wms.js');
    return $this->options;
  }
}
