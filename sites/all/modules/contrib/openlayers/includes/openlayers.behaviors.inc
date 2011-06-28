<?php

/**
 * @file
 * OpenLayers default packaged behaviors
 */

/**
 * Implementation of hook_openlayers_behaviors().
 *
 */
function _openlayers_openlayers_behaviors() {
  return array(
    'openlayers_behavior_attribution' => array(
      'title' => t('Attribution'),
      'description' => t('Allows layers to provide attribution to the map if it exists.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_attribution.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_attribution',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_argparser' => array(
      'title' => t('ArgParser'),
      'description' => t('Parses Permalink-formatted arguments without adding a 
        Permalink link to the map'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_argparser.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_argparser',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_popup' => array(
      'title' => t('Pop Up'),
      'description' => t('Adds clickable info boxes to points or shapes on maps.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_popup.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_popup',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_tooltip' => array(
      'title' => t('Tooltip'),
      'description' => t('Adds info boxes on hover to points or shapes on maps.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_tooltip.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_tooltip',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_fullscreen' => array(
      'title' => t('Fullscreen'),
      'description' => t('Provides a button that expands maps to the size of the page.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_fullscreen.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_fullscreen',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_keyboarddefaults' => array(
      'title' => t('Keyboard Defaults'),
      'description' => t('Provides keyboard shortcuts to pan and zoom the map.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_keyboarddefaults.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_keyboarddefaults',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_layerswitcher' => array(
      'title' => t('Layer Switcher'),
      'description' => t('Provides the ability to switch layers in the map interface.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_layerswitcher.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_layerswitcher',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_mouseposition' => array(
      'title' => t('Mouse Position'),
      'description' => t('Provides a visual indication of the mouse position to the user.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_mouseposition.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_mouseposition',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_navigation' => array(
      'title' => t('Navigation'),
      'description' => t('Provides the ability to navigate the map interface.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_navigation.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_navigation',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_dragpan' => array(
      'title' => t('DragPan'),
      'description' => t('Provides the ability to pan in the map interface.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_dragpan.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_dragpan',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_panzoom' => array(
      'title' => t('PanZoom'),
      'description' => t('Provides the ability to pan and zoom in the map interface.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_panzoom.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_panzoom',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_panzoombar' => array(
      'title' => t('PanZoomBar'),
      'description' => t('Gives user ability to pan and zoom, with a zoom bar, in the map interface.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_panzoombar.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_panzoombar',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_mapformvalues' => array(
      'title' => t('Map Form Values'),
      'description' => t('Provides a way of updating form elements with zoom level and centering from a map.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_mapformvalues.inc',
      'ui_visibility' => FALSE,
      'behavior' => array(
        'class' => 'openlayers_behavior_mapformvalues',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_boxselect' => array(
      'title' => t('Boxselect'),
      'description' => t('Allows the selection and display of a box.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_boxselect.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_boxselect',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_permalink' => array(
      'title' => t('Permalink'),
      'description' =>  t('Provides a URL that will link to a specific map position.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_permalink.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_permalink',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_scaleline' => array(
      'title' => t('Scale Line'),
      'description' => t('Provides a line of scale in the map interface.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_scaleline.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_scaleline',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_zoombox' => array(
      'title' => t('Zoom Box'),
      'description' => t('Provides a zoom box that can be drawn on shift + click.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_zoombox.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_zoombox',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_zoompanel' => array(
      'title' => t('Zoom Panel'),
      'description' => t('Provides a Zoom In, Zoom Out, and Zoom to Max Extent Button'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_zoompanel.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_zoompanel',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_zoomtomaxextent' => array(
      'title' => t('Zoom to Max Extent'),
      'description' => t('Provides button to zoom to the maximum extent of the map.'),
      'type' => 'map',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_zoomtomaxextent.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_zoomtomaxextent',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_zoomtolayer' => array(
      'title' => t('Zoom to Layer'),
      'description' => t('Zooms to the extent of a given layer on map loading.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_zoomtolayer.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_zoomtolayer',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_drawfeatures' => array(
       'title' => t('Draw Features'),
       'description' => t('Provides functionality for adding features to a map.'),
       'type' => 'layer',
       'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
       'file' => 'openlayers_behavior_drawfeatures.inc',
       'behavior' => array(
         'class' => 'openlayers_behavior_drawfeatures',
         'parent' => 'openlayers_behavior',
       ),
    ),
    'openlayers_behavior_cluster' => array(
      'title' => t('Cluster'),
      'description' => t('Provides vector layer features clustering by proximity.'),
      'type' => 'layer',
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_cluster.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_cluster',
        'parent' => 'openlayers_behavior',
      ),
    ),
    'openlayers_behavior_geolocate' => array(
      'title' => t('Geolocate'),
      'description' => t('Provides the geolcoation control and simply zooms to users location.'),
      'path' => drupal_get_path('module', 'openlayers') . '/includes/behaviors',
      'file' => 'openlayers_behavior_geolocate.inc',
      'behavior' => array(
        'class' => 'openlayers_behavior_geolocate',
        'parent' => 'openlayers_behavior',
      ),
    ),
  );
}
