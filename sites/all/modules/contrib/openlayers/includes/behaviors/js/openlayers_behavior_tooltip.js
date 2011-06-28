
/**
 * Javascript Drupal Theming function for inside of Tooltips
 *
 * To override
 *
 * @param feature
 *  OpenLayers feature object.
 * @return
 *  Formatted HTML.
 */
Drupal.theme.prototype.openlayersTooltip = function(feature) {
  var output = '';
  
  if (feature.attributes.name) {
    output += '<div class="openlayers-popup openlayers-tooltip-name">' + feature.attributes.name + '</div>';
  }
  
  if (feature.attributes.description) {
    output += '<div class="openlayers-popup openlayers-tooltip-description">' + feature.attributes.description + '</div>';
  }
  
  return output;
};

(function($) {
/**
 * OpenLayers Tooltip Behavior
 */
Drupal.behaviors.openlayers_behavior_tooltip = {
  'attach': function(context, settings) {
    var layers, data = $(context).data('openlayers');
    if (data && data.map.behaviors['openlayers_behavior_tooltip']) {
      var map = data.openlayers;
      var options = data.map.behaviors['openlayers_behavior_tooltip'];
      var layers = [];

      // For backwards compatiability, if layers is not
      // defined, then include all vector layers
      if (typeof options.layers == 'undefined' || options.layers.length == 0) {
        layers = map.getLayersByClass('OpenLayers.Layer.Vector');
      }
      else {
        for (var i in options.layers) {
          var selectedLayer = map.getLayersBy('drupalID', options.layers[i]);
          if (typeof selectedLayer[0] != 'undefined') {
            layers.push(selectedLayer[0]);
          }
        }
      }

      // Define feature select events for selected layers.
      popupSelect = new OpenLayers.Control.SelectFeature(layers,
        {
          hover: true,
          clickout: false,
          multiple: false,
          onSelect: function(feature) {
            // Create FramedCloud popup for tooltip.
            var output = Drupal.theme('openlayersTooltip', feature);
            if (typeof output != 'undefined') {
              popup = new OpenLayers.Popup.FramedCloud(
                'tooltip',
                feature.geometry.getBounds().getCenterLonLat(),
                null,
                output,
                null,
                true
              );
              feature.popup = popup;
              feature.layer.map.addPopup(popup);
            }
          },
          onUnselect: function(feature) {
            // Remove popup.
            if (typeof feature.popup != 'undefined') {
              feature.layer.map.removePopup(feature.popup);
              feature.popup.destroy();
              feature.popup = null;
            }
          }
        }
      );

      // Actiate the popups
      map.addControl(popupSelect);
      popupSelect.activate();
    }
  }
};
})(jQuery);
