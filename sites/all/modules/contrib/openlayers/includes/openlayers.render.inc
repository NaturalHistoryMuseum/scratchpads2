<?php

/**
 * @file
 * Processing functions for layers and behaviors
 * @ingroup openlayers
 */

/**
 * Initialize the layer array into an indexed array of layer objects
 *
 * @param $layers
 *   Array of layers to process
 * @param $map
 *   Map array
 * @return $layer_data
 *   Array of initialized layer objects
 */
function _openlayers_layers_process($layers = array(), &$map = array()) {
  $layer_data = array();

  $weight = 0;
  foreach ($layers as $layer) {
    if ($layer_object = openlayers_layer_load($layer)) {
      $layer_object->render($map);
      $layer_object->data['title'] = $layer_object->title;
      $layer_object->data['weight'] = ++$weight;
      $layer_data[$layer_object->name] = $layer_object->data;
    }
  }

  return $layer_data;
}

/**
 * Execute render() method for all enabled behaviors.
 *
 * @param $behaviors
 *   Array of behaviors to process
 * @param $map
 *   Map array
 * @return $rendered
 *  Indexed array of rendered behaviors
 */
function _openlayers_behaviors_render($behaviors = array(), &$map = array()) {
  $rendered = array();
  
  foreach (openlayers_behaviors() as $key => $plugin) {
    if (isset($behaviors[$key]) && $class = ctools_plugin_get_class($plugin, 'behavior')) {
      $behavior = new $class($behaviors[$key], $map);
      $rendered[$key] = $behavior->render($map);
    }
  }

  return $rendered;
}

/**
 * Process Styles
 *
 * Get full data for any styles
 *
 * TODO: KILL KILL KILL
 *
 * @param $styles
 *   Array of map styles ( <style_role> : <style_name> | <style_array> )
 * @param $layer_styles
 *   Array of layer styles ( <layer_name> : <style_name> )
 * @param $map
 *   Map array
 * @return $processed
 *   Array of processed styles ( <style_name> => <style_array> )
 */
function _openlayers_styles_process($styles = array(), 
  $layer_styles = array(), &$map = array()) {

  // Get styles info array
  $styles_info = openlayers_styles();

  // Go through styles
  $processed = array();
  foreach ($styles as $k => $style) {
    // Check if array, if array, just pass on
    if (is_array($style)) {
      $processed[$k] = $style;
    }
    elseif (!empty($styles_info[$style]) && $info = $styles_info[$style]->data) {
      $processed[$k] = $info;
    }
  }

  // Add layer styles
  foreach ($layer_styles as $style) {
    if (!isset($processed[$style]) && 
        !empty($styles_info[$style]) && 
        $info = $styles_info[$style]->data) {
      $processed[$style] = $info;
    }
  }
  
  // Update URLs to support different types of paths
  foreach ($processed as $k => $style) {
    if (!empty($style['externalGraphic'])) {
      // Check full URL or absolute path
      if (!valid_url($style['externalGraphic'], TRUE) 
        && strpos($style['externalGraphic'], '/') !== 0) {
        // Make full URL from Drupal path
        $processed[$k]['externalGraphic'] = url($style['externalGraphic'], array('absolute' => TRUE));
      }
    }
  }

  // Run through theme function
  $processed = theme('openlayers_styles', array(
    'styles' => $processed, 
    'map' => $map)
  );

  // Return processed
  return $processed;
}

/**
 * Create Map ID
 *
 * Create a unique ID for any maps that are not assigned an ID
 *
 * @note
 *   Technically someone can assign a map ID identical
 *   to the one that is created
 * @return
 *   New map id
 */
function _openlayers_create_map_id() {
  return 'openlayers-map-' . substr(md5(uniqid(mt_rand())), 0, 8);
}
