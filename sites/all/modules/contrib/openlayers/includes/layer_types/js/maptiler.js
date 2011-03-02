
/**
 * @file
 * Layer handler for TMS layers
 */

/**
 * Openlayer layer handler for TMS layer
 */
Drupal.openlayers.layer.maptiler = function(title, map, options) {
  if (options.maxExtent !== undefined) {
    options.maxExtent = new OpenLayers.Bounds.fromArray(options.maxExtent);
  }
  options.getURL = function(bounds) {
    var res = this.map.getResolution();
    var x = Math.round((bounds.left - this.maxExtent.left) / (res * this.tileSize.w));
    var y = Math.round((bounds.bottom - this.tileOrigin.lat) / (res * this.tileSize.h));
    var z = this.map.getZoom();
    if (this.map.baseLayer.name == 'Virtual Earth Roads' ||
      this.map.baseLayer.name == 'Virtual Earth Aerial' ||
      this.map.baseLayer.name == 'Virtual Earth Hybrid') {
      z = z + 1;
    }
    return this.url + z + '/' + x + '/' + y + '.' + this.type;
  }
  options.projection = 'EPSG:' + options.projection;
  var layer = new OpenLayers.Layer.TMS(title, options.base_url, options);
  return layer;
};
