
/**
 * Process Vector Layers
 *
 * @param layerOptions
 *   Object of options.
 * @param map
 *   Reference to OpenLayers object.
 * @return
 *   Valid OpenLayers layer.
 */
Drupal.openlayers.layer.vector = function(title, map, options) {
  var styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);
  var features = [];

  // Since we add features manually, we take the features from the options
  // store locally and remove from original
  if (options.features !== undefined) {
    features = options.features;
    delete options.features;
  }

  // Create layer
  var layer = new OpenLayers.Layer.Vector(title, options);

  // Add features
  Drupal.openlayers.addFeatures(map, layer, features);

  // Add styles
  layer.styleMap = styleMap;
  return layer;
};
