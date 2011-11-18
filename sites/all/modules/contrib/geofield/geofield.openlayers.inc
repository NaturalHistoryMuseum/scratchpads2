<?php

/**
 * @file
 * Provides hooks for integration with OpenLayers (http://drupal.org/project/openlayers)
 */

/**
 * Implements hook_openlayers_maps().
 */
function geofield_openlayers_maps() {
  // Create full preset array
  $widget = new stdClass();
  $widget->api_version = 1;
  $widget->name = 'geofield_widget_map';
  $widget->title = t('Geofield Widget Map');
  $widget->description = t('A Map Used for Geofield Input');
  $widget->data = array(
    'projection' => '900913',
    'width' => '600px',
    'default_layer' => 'osm_mapnik',
    'height' => '400px',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2'
      )
    ),
    'options' => array(
      'maxExtent' => openlayers_get_extent('900913'),
    ),
    'behaviors' => array(
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_geofield' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_navigation' => array(
        'navigation' => '',
        'zoomWheelEnabled' => FALSE,
      ),
    ),
    'layers' => array(
      'osm_mapnik',
    )
  );
  
  $formatter = new stdClass();
  $formatter->api_version = 1;
  $formatter->name = 'geofield_formatter_map';
  $formatter->title = t('Geofield Formatter Map');
  $formatter->description = t('A Map Used for Geofield Output');
  $formatter->data = array(
    'projection' => '900913',
    'width' => '600px',
    'default_layer' => 'osm_mapnik',
    'height' => '400px',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2'
      )
    ),
    'options' => array(
      'maxExtent' => openlayers_get_extent('900913'),
    ),
    'behaviors' => array(
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => 'geofield_formatter',
        'point_zoom_level' => 5,
      ),
      'openlayers_behavior_navigation' => array(
        'navigation' => '',
        'zoomWheelEnabled' => FALSE,
      ),
    ),
    'layers' => array(
      'geofield_formatter',
      'osm_mapnik',
    )
  );
  
  return array(
    'geofield_widget_map' => $widget, 
    'geofield_formatter_map' => $formatter, 
  );
}

/**
 * Implements hook_openlayers_behaviors().
 */
function geofield_openlayers_behaviors() {
  return array(
    'openlayers_behavior_geofield' => array(
      'title' => t('Geofield'),
      'description' => t('Fuels the geofield map-input form.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'geofield') . '/includes/behaviors',
      'file' => 'openlayers_behavior_geofield.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_geofield',
        'parent' => 'openlayers_behavior',
      ),
    ),
  );
}

/**
 * Formatter layers
 */
function geofield_openlayers_layers() {
  $layers = array();
  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'geofield_formatter';
  $layer->title = 'Placeholder for Geofield Formatter';
  $layer->description = '';
  $layer->data = array(
    'layer_type' => 'openlayers_layer_type_raw',
    'projection' => array('900913'),
    'features' => array()
  );
  $layers[$layer->name] = $layer;
  return $layers;
}
