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
  
  // Load Layers and assign weights
  foreach ($layers as $key => $layer){
    if ($layer_object = openlayers_layer_load($layer)) {
      $layers[$key] = $layer_object;
      if (!empty($map['layer_weight'][$key])) {
        $layers[$key]->weight = $map['layer_weight'][$key];
      }
      else $layers[$key]->weight = 0;
    }
  }
  
  // Sort layers
  usort($layers, '_openlayers_layers_process_sort');
  
  // Process into array-based layer data for the map
  foreach ($layers as $layer_object) {
    $layer_object->render($map);
    $layer_object->data['title'] = $layer_object->title;
    $layer_object->data['weight'] = $layer_object->weight;
    $layer_data[$layer_object->name] = $layer_object->data;
  }

  return $layer_data;
}

/**
 * Callback function for sorting
 *
 * @param $a
 *   Layer $a
 * @param $b
 *   Layer $b
 * @return $a_greater_b
 *   Return the weight different - allowing usort to sort
 */
function _openlayers_layers_process_sort($a, $b) {
  return intval($a->weight - $b->weight);
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
        $processed[$k]['externalGraphic'] = openlayers_style_path($style['externalGraphic']);
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

/**
 * URL Style
 *
 * Takes in a path and makes full URL for style.  Overall, this
 * can be handled by url(), but we have to avoid some encoding
 * for variable replacement.  Note that this is not perfect as
 * it will decode values that maybe not specifically part of the
 * attribute replacement.
 *
 * @param $path
 *   Path to process.
 * @return
 *   Processed path.
 */
function openlayers_style_path($path) {
    $path = url($path, array('absolute' => TRUE));
    $path = str_replace('%24%7B', '${', $path);
    $path = str_replace('%7D', '}', $path);
    return $path;
}
