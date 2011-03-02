
/**
 * OpenLayers Raw Layer Handler
 */
Drupal.openlayers.layer.openlayers_raw = function(title, map, options) {
  options.options = options.options || {};

  // Note, so that we do not pass all the features along to the Layer
  // options, we use the options.options to give to Layer
  options.options.drupalID = options.drupalID;
  var layer = new OpenLayers.Layer.Vector(title, options.options);

  options.options.styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);

  if (options.features) {
    Drupal.openlayers.addFeatures(map, layer, options.features);
  }

  return layer;
};
