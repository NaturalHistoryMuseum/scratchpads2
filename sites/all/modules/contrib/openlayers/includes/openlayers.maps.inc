<?php

/**
 * @file
 * This file contains map implementations
 *
 * @ingroup openlayers
 */

/**
 * Map definitions
 *
 * Internal callback for openlayers map implementation.
 *
 * @return
 *   Array of maps
 */
function _openlayers_openlayers_maps() {
  $items = array();

  // Create full map array
  $default = new stdClass();
  $default->api_version = 1;
  $default->name = 'default';
  $default->title = t('Default Map');
  $default->description = t('This is the default map that comes with the OpenLayers module.');
  $default->data = array(
    'projection' => '900913',
    'width' => 'auto',
    'height' => '400px',
    'default_layer' => 'osm_mapnik',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2'
      )
    ),
    'displayProjection' => '4326',
    'maxExtent' => openlayers_get_extent('4326'),
    'behaviors' => array(
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_layerswitcher' => array(),
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_navigation' => array(),
    ),
    'layers' => array(
      'osm_mapnik' => 'osm_mapnik',
    )
  );
  $items['default'] = $default;
  
  // Backwards compatibilty for features presets.  Since features
  // no longers cares about the preset hooks, we need to manually include
  // the preset file that stores the old hooks.
  $found_features = FALSE;
  if (module_exists('features')) {
    $features = features_get_features();
    
    foreach ($features as $feature) {
      // Only utilize enabled features and look for presets
      if ($feature->status > 0 && !empty($feature->info['features']['openlayers_map_presets'])) {
        $found_features = TRUE;
        // Include the file we need.
        $presets = $feature->info['features']['openlayers_map_presets'];
        $module = $feature->name;
        $inc = module_load_include('inc', $module, $module . '.openlayers_presets');
      }
    }
  }
  
  // If features found, we have to explicilty reset the implement
  // cache.  This could be a significant performance hit on a site
  // that has a high number of modules installed.
  if ($found_features) {
    module_implements('openlayers_presets', FALSE, TRUE);
  }

  // Backwards compatibilty for normal presets
  $data = module_invoke_all('openlayers_presets');
  if (count($data) > 0) {
    $items = $items + $data;
  }

  
  return $items;
}
