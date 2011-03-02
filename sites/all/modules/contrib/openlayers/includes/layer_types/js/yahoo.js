
/**
 * Process Yahoo Layers
 *
 * @param layerOptions
 *   Object of options.
 * @param mapid
 *   Map ID.
 * @return
 *   Valid OpenLayers layer.
 */
Drupal.openlayers.layer.yahoo = function(title, map, options) {
  var styleMap = Drupal.openlayers.getStyleMap(map, options.drupalID);

  yahoo_type_map = {
    'street': YAHOO_MAP_REG,
    'satellite': YAHOO_MAP_SAT,
    'hybrid': YAHOO_MAP_HYB
  };

  options.sphericalMercator = true;
  options.maxExtent = new OpenLayers.Bounds(
    -20037508, -20037508, 20037508, 20037508);
  options.type = yahoo_type_map[options.type];

  var layer = new OpenLayers.Layer.Yahoo(title, options);
  layer.styleMap = styleMap;
  return layer;
};
