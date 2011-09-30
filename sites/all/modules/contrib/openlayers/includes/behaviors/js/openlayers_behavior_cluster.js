
/**
 * @file
 * OpenLayers Behavior implementation for clustering.
 */

(function ($) {
  
/**
 * OpenLayers Cluster Behavior
 */
Drupal.behaviors.openlayers_cluster = {
  attach: function(context) {
    var data = $(context).data('openlayers');
    if (data && data.map.behaviors.openlayers_behavior_cluster) {
      var options = data.map.behaviors.openlayers_behavior_cluster;
      var map = data.openlayers;
      var distance = parseInt(options.distance, 10);
      var threshold = parseInt(options.threshold, 10);
      var layers = [];
      for (var i in options.clusterlayer) {
        var selectedLayer = map.getLayersBy('drupalID', options.clusterlayer[i]);
        if (typeof selectedLayer[0] != 'undefined') {
          layers.push(selectedLayer[0]);
        }
      }

      // Go through chosen layers
      for (var i in layers) {
        var layer = layers[i];
        // Ensure vector layer
        if (layer.CLASS_NAME == 'OpenLayers.Layer.Vector') {
          var cluster = new OpenLayers.Strategy.Cluster(options);
          layer.addOptions({ 'strategies': [cluster] });
          cluster.setLayer(layer);
          cluster.features = layer.features.slice();
          cluster.activate();
          cluster.cluster();
        }
      }
    }
  }
};

/*
 * Override of callback used by 'popup' behaviour to support clusters
 */
Drupal.theme.openlayersPopup = function(feature) {
  if (feature.cluster)
  {
    var output = '';
    var visited = []; // to keep track of already-visited items
    for (var i = 0; i < feature.cluster.length; i++) {
      var pf = feature.cluster[i]; // pseudo-feature
      if (typeof pf.drupalFID != 'undefined') {
        var mapwide_id = feature.layer.drupalID + pf.drupalFID;
        if (mapwide_id in visited) continue;
        visited[mapwide_id] = true;
      }
      output += '<div class="openlayers-popup openlayers-popup-feature">' +
        Drupal.theme.prototype.openlayersPopup(pf) + '</div>';
    }
    return output;
  }
  else
  {
    return Drupal.theme.prototype.openlayersPopup(feature);
  }
};

})(jQuery);
