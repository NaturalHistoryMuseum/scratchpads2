<?php

/**
 * @file
 * This file contains layer types implementations
 *
 * @ingroup openlayers
 */

/**
 * Layer Type Implementation
 *
 * Internal callback for openlayers layer types implementation.
 *
 * @return
 *   Array of layer types
 */
function _openlayers_openlayers_layer_types() {
  return array(
    'openlayers_layer_type_google' => array(
      'title' => t('Google'),
      'description' => t('Google Maps API Map'),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'google.inc',
        'class' => 'openlayers_layer_type_google',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_yahoo' => array(
      'title' => t('Yahoo'),
      'description' => t('Yahoo Maps API Map'),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'yahoo.inc',
        'class' => 'openlayers_layer_type_yahoo',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_virtualearth' => array(
      'title' => t('Virtual Earth'),
      'description' => t('Microsoft Virtual Earth'),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'virtualearth.inc',
        'class' => 'openlayers_layer_type_virtualearth',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_cloudmade' => array(
      'title' => t('CloudMade'),
      'description' => t('<a href="!url">CloudMade</a> Custom Map',
        array('!url' => 'http://cloudmade.com/')),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'cloudmade.inc',
        'class' => 'openlayers_layer_type_cloudmade',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_wms' => array(
      'title' => t('WMS'),
      'description' => t('<a href="!url">Web Map Service</a>',
        array('!url' => 'http://en.wikipedia.org/wiki/Web_Map_Service')),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'wms.inc',
        'class' => 'openlayers_layer_type_wms',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_tms' => array(
      'title' => t('TMS'),
      'description' => t('<a href="!url">Tile Map Service</a>',
        array('!url' => 'http://en.wikipedia.org/wiki/Tile_Map_Service')),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'tms.inc',
        'class' => 'openlayers_layer_type_tms',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_osm' => array(
      'title' => t('OSM'),
      'description' => t('OpenStreetMap Standard'),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'osm.inc',
        'class' => 'openlayers_layer_type_osm',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_xyz' => array(
      'title' => t('XYZ'),
      'description' => t('XYZ'),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'xyz.inc',
        'class' => 'openlayers_layer_type_xyz',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_maptiler' => array(
      'title' => t('MapTiler'),
      'description' => t('<a href="!url">MapTiler</a> or GDAL2Tiles',
        array('!url' => 'http://www.maptiler.org/')),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'maptiler.inc',
        'class' => 'openlayers_layer_type_maptiler',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_kml' => array(
      'title' => t('KML'),
      'description' => t('<a href="!url">KML</a> overlay.', 
        array('!url' => 'http://en.wikipedia.org/wiki/Keyhole_Markup_Language')),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'kml.inc',
        'class' => 'openlayers_layer_type_kml',
        'parent' => 'openlayers_layer_type',
      ),
    ),
    'openlayers_layer_type_raw' => array(
      'title' => t('Raw'),
      'description' => t('Layer type for raw data input - 
        not meant to be added with the UI.' 
      ),
      'layer_type' => array(
        'path' => drupal_get_path('module', 'openlayers') . '/includes/layer_types',
        'file' => 'raw.inc',
        'class' => 'openlayers_layer_type_raw',
        'parent' => 'openlayers_layer_type',
      ),
    ),
  );
}
