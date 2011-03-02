<?php

/**
 * @file
 *
 * OpenLayers default packaged layers
 */
function _openlayers_openlayers_layers() {
  $layers = array();

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'google_satellite';
  $layer->title = 'Google Maps Satellite';
  $layer->description = 'Google Maps Satellite Imagery.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'satellite',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_google',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'google_hybrid';
  $layer->title = 'Google Maps Hybrid';
  $layer->description = 'Google Maps with roads and terrain.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'hybrid',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_google',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'google_normal';
  $layer->title = 'Google Maps Normal';
  $layer->description = 'Standard Google Maps Roads';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'normal',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_google',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'google_physical';
  $layer->title = 'Google Maps Physical';
  $layer->description = 'Google Maps Hillshades';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'physical',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_google',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'yahoo_satellite';
  $layer->title = 'Yahoo Maps Satellite';
  $layer->description = 'Yahoo satellite imagery tiles.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'satellite',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_yahoo',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'yahoo_street';
  $layer->title = 'Yahoo Maps Street';
  $layer->description = 'Yahoo streets tiles.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'street',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_yahoo',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'yahoo_hybrid';
  $layer->title = 'Yahoo Maps Hybrid';
  $layer->description = 'Yahoo hybrid of streets and satellite tiles.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'hybrid',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_yahoo',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'virtualearth_street';
  $layer->title = 'Virtual Earth Street';
  $layer->description = 'Virtual Earth (Bing) street tiles.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'street',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_virtualearth',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'virtualearth_satellite';
  $layer->title = 'Virtual Earth Satellite';
  $layer->description = 'Virtual Earth (Bing) satellite tiles.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'satellite',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_virtualearth',
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'virtualearth_hybrid';
  $layer->title = 'Virtual Earth Hybrid';
  $layer->description = 'Virtual Earth (Bing) hybrid of streets and satellite tiles.';
  $layer->data = array(
    'baselayer' => TRUE,
    'type' => 'hybrid',
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_virtualearth',
  );
  $layers[$layer->name] = $layer;

  // OpenStreetMap Mapnik
  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'osm_mapnik';
  $layer->title = 'OSM Mapnik';
  $layer->description = 'The main OpenStreetMap map';
  $layer->data = array(
    'baselayer' => TRUE,
    'attribution' => t('&copy;<a href="@ccbysa">CCBYSA</a> 2010
      <a href="@openstreetmap">OpenStreetMap.org</a> contributors',
      array(
        '@openstreetmap' => 'http://www.openstreetmap.org/',
        '@ccbysa' => 'http://creativecommons.org/licenses/by-sa/2.0/',
      )
    ),
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_xyz',
    'url' => 'http://tile.openstreetmap.org/${z}/${x}/${y}.png',
  );
  $layers[$layer->name] = $layer;

  // OpenStreetMap Tiles @ Home
  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'osm_tah';
  $layer->title = 'OSM Tiles@Home';
  $layer->description = 'Alternative, community-rendered OpenStreetMap';
  $layer->data = array(
    'baselayer' => TRUE,
    'attribution' => t('&copy;<a href="@ccbysa">CCBYSA</a> 2010
      <a href="@openstreetmap">OpenStreetMap.org</a> contributors',
      array(
        '@openstreetmap' => 'http://www.openstreetmap.org/',
        '@ccbysa' => 'http://creativecommons.org/licenses/by-sa/2.0/',
      )
    ),
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_xyz',
    'url' => 'http://tah.openstreetmap.org/Tiles/tile/${z}/${x}/${y}.png',
  );
  $layers[$layer->name] = $layer;

  // OpenStreetMap Cycling Map
  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'osm_cycle';
  $layer->title = 'OSM Cycling Map';
  $layer->description = 'OpenStreetMap with highlighted bike lanes';
  $layer->data = array(
    'baselayer' => TRUE,
    'attribution' => t('&copy;<a href="@ccbysa">CCBYSA</a> 2010
      <a href="@openstreetmap">OpenStreetMap.org</a> contributors',
      array(
        '@openstreetmap' => 'http://www.openstreetmap.org/',
        '@ccbysa' => 'http://creativecommons.org/licenses/by-sa/2.0/',
      )
    ),
    'projection' => array('900913'),
    'layer_type' => 'openlayers_layer_type_xyz',
    'url' => 'http://andy.sandbox.cloudmade.com/tiles/cycle/${z}/${x}/${y}.png',
  );
  $layers[$layer->name] = $layer;

  // OpenStreetMap 426 hybrid overlay
  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'osm_4326_hybrid';
  $layer->title = 'OSM Overlay';
  $layer->description = 'Semi-transparent hybrid overlay. Projected into 
    WSG84 for use on non spherical-mercator maps.';
  $layer->data = array(
    'baselayer' => FALSE,
    'attribution' => t('&copy;<a href="@ccbysa">CCBYSA</a> 2010
      <a href="@openstreetmap">OpenStreetMap.org</a> contributors',
      array(
        '@openstreetmap' => 'http://www.openstreetmap.org/',
        '@ccbysa' => 'http://creativecommons.org/licenses/by-sa/2.0/',
      )
    ),
    'projection' => array('4326'),
    'layer_type' => 'openlayers_layer_type_wms',
    'url' => 'http://oam.hypercube.telascience.org/tiles',
    'params' => array(
      'isBaseLayer' => FALSE,
      'layers' => 'osm-4326-hybrid',
    ),
    'options' => array(
      'buffer' => 1,
    ),
  );
  $layers[$layer->name] = $layer;

  $layer = new stdClass();
  $layer->api_version = 1;
  $layer->name = 'wms_default';
  $layer->title = 'Default OpenLayers WMS';
  $layer->description = 'MetaCarta basemap of province and water boundaries';
  $layer->data = array(
    'projection' => array('4326'),
    'baselayer' => TRUE,
    'layer_type' => 'openlayers_layer_type_wms',
    'base_url' => 'http://labs.metacarta.com/wms-c/Basic.py',
    'params' => array(
      'isBaseLayer' => TRUE,
    ),
    'options' => array(
      'layers' => array('basic'),
      'maxExtent' => openlayers_get_extent('4326'),
    ),
  );
  $layers[$layer->name] = $layer;


  return $layers;
}

/**
 * This function is for the po editor to be able to find these strings, 
 * since in the codebase they are not in t()'s, because they are later 
 * run through t() in the layer loader function
 */
function _openlayers_openlayers_layers_i18n() {
  $translatable_strings = array(
    // titles
    t('Google Maps Satellite'),
    t('Google Maps Hybrid'),
    t('Google Maps Normal'),
    t('Google Maps Physical'),
    t('Yahoo Maps Street'),
    t('Yahoo Maps Hybrid'),
    t('Yahoo Maps Satellite'),
    t('Virtual Earth Street'),
    t('Virtual Earth Satellite'),
    t('Virtual Earth Hybrid'),
    t('OSM Mapnik'),
    t('OSM Tiles@Home'),
    t('OSM Cycling Map'),
    t('OSM Overlay'),
    t('Default OpenLayers WMS'),
    // descriptions
    t('Alternative, community-rendered OpenStreetMap'),
    t('Google Maps Hillshades'),
    t('Google Maps Satellite Imagery.'),
    t('Google Maps with roads and terrain.'),
    t('MetaCarta basemap of province and water boundaries'),
    t('OpenStreetMap with highlighted bike lanes'),
    t('Semi-transparent hybrid overlay. Projected into 
      WSG84 for use on non spherical-mercator maps.'),
    t('Standard Google Maps Roads'),
    t('The main OpenStreetMap map'),
    t('Virtual Earth (Bing) hybrid of streets and satellite tiles.'),
    t('Virtual Earth (Bing) satellite tiles.'),
    t('Virtual Earth (Bing) street tiles.'),
    t('Yahoo hybrid of streets and satellite tiles.'),
    t('Yahoo satellite imagery tiles.'),
    t('Yahoo streets tiles.'),
  );
}
