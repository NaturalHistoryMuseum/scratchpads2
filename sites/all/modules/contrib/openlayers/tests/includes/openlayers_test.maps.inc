<?php

/**
 * @file
 * Holds testing maps
 */

/**
 * Separated implementation of hook_openlayers_maps().
 */
function _openlayers_test_openlayers_maps() {
  $items = array();

  // Map with some behaviors
  $behaviors_test = new stdClass();
  $behaviors_test->api_version = 1;
  $behaviors_test->name = 'behaviors_test';
  $behaviors_test->title = t('Test: Behaviors');
  $behaviors_test->description = t('This is a test map.');
  $behaviors_test->data = array(
    'projection' => '4326',
    'projection' => '900913',
    'width' => 'auto',
    'default_layer' => 'osm_mapnik',
    'height' => '400px',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2'
      )
    ),
    'options' => array(
      'displayProjection' => '4326',
      'maxExtent' => openlayers_get_extent('4326'),
    ),
    'behaviors' => array(
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_layerswitcher' => array(),
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_navigation' => array(),
      'openlayers_behavior_fullscreen' => array(),
      'openlayers_behavior_mouseposition' => array(),
      'openlayers_behavior_dragpan' => array(),
      'openlayers_behavior_boxselect' => array(),
      'openlayers_behavior_permalink' => array(),
      'openlayers_behavior_scaleline' => array(),
      'openlayers_behavior_zoombox' => array(),
      'openlayers_behavior_zoomtomaxextent' => array(),
    ),
    'layers' => array(
      'osm_mapnik' => 'osm_mapnik',
    )
  );
  $items['behaviors_test'] = $behaviors_test;

  /*
  // Map with popups
  $popup_test = new stdClass();
  $popup_test->api_version = 1;
  $popup_test->name = 'popup_test';
  $popup_test->title = t('Test: Popups');
  $popup_test->description = t('This is a test map.');
  $popup_test->data = array(
    'projection' => '4326',
    'projection' => '900913',
    'width' => 'auto',
    'default_layer' => 'osm_mapnik',
    'height' => '400px',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2'
      )
    ),
    'options' => array(
      'displayProjection' => '4326',
      'maxExtent' => openlayers_get_extent('4326'),
    ),
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
  // Add features
  $features = array();
  for ($i = 0; $i < 30; $i++) {
    $x = 2 + rand(-30, 30);
    $y = 2 + rand(-30, 30);
    $features['feature_' . $i . $j] = array(
      'wkt' => 'POINT(' . $x . ' ' . $y . ')',
      'projection' => '4326',
    );
  }
  $popup_test->data['layers']['style_vector']['features'] = $features;

  $items['popup_test'] = $popup_test;
  */

  $openlayers_maps_cck_views = new stdClass;
  $openlayers_maps_cck_views->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps_cck_views->api_version = 1;
  $openlayers_maps_cck_views->name = 'openlayers_test_map_cck_views';
  $openlayers_maps_cck_views->title = 'Test: OpenLayers Test Views';
  $openlayers_maps_cck_views->description = 'This map tests a custom content type with a WKT field and a views layer.';
  $openlayers_maps_cck_views->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_attribution' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(),
      'openlayers_behavior_navigation' => array(),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_popup' => array(),
    ),
    'default_layer' => 'osm_mapnik',
    'layers' => array(
      'osm_mapnik' => 'osm_mapnik',
      'openlayers_test_view_openlayers_1' => 'openlayers_test_view_openlayers_1',
    ),
    'layer_styles' => array(),
    'layer_activated' => array(
      'openlayers_test_view_openlayers_1' => 'openlayers_test_view_openlayers_1',
    ),
    'layer_switcher' => array(
      'openlayers_test_view_openlayers_1' => 'openlayers_test_view_openlayers_1',
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'default',
      'select' => 'default',
      'temporary' => 'default',
    ),
    'options' => NULL,
  );

  // We only want to provide the Views/CCK map if the
  // modules are installed and the view is provide
  if (module_exists('content') && module_exists('views') && module_exists('content_copy')) {
    $layers = openlayers_layers_load();
    if (!empty($layers['openlayers_test_view_openlayers_1'])) {
      $items['openlayers_test_map_cck_views'] = $openlayers_maps_cck_views;
    }
  }

  // Return maps
  return $items;
}