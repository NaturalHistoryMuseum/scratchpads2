
/**
 * Global variables to help with scope
 *
 * TODO: Move this to a better place, like the map data().
 */
Drupal.openlayers = Drupal.openlayers || {};
Drupal.openlayers.popup = Drupal.openlayers.popup || {};
Drupal.openlayers.popup.popupSelect = Drupal.openlayers.popup.popupSelect || {};
Drupal.openlayers.popup.selectedFeature = Drupal.openlayers.popup.selectedFeature || {};


(function($) {
/**
 * Javascript Drupal Theming function for inside of Popups
 *
 * To override
 *
 * @param feature
 *  OpenLayers feature object.
 * @return
 *  Formatted HTML.
 */
Drupal.theme.prototype.openlayersPopup = function(feature) {
  var output = '';
  
  if (feature.attributes.name) {
    output += '<div class="openlayers-popup openlayers-tooltip-name">' + feature.attributes.name + '</div>';
  }
  
  if (feature.attributes.description) {
    output += '<div class="openlayers-popup openlayers-tooltip-description">' + feature.attributes.description + '</div>';
  }
  
  return output;
}

/**
 * OpenLayers Popup Behavior
 */
Drupal.behaviors.openlayers_behavior_popup =  {
  attach: function(context) {
  var layers, data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_popup']) {
    var map = data.openlayers;
    var options = data.map.behaviors['openlayers_behavior_popup'];
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

    popupSelect = new OpenLayers.Control.SelectFeature(layers,
      {
        onSelect: function(feature) {
          // Create FramedCloud popup.
          popup = new OpenLayers.Popup.FramedCloud(
            'popup',
            feature.geometry.getBounds().getCenterLonLat(),
            null,
            Drupal.theme('openlayersPopup', feature),
            null,
            true,
            function(evt) {
              Drupal.openlayers.popup.popupSelect.unselect(
                Drupal.openlayers.popup.selectedFeature
              );
            }
          );

          // Assign popup to feature and map.
          feature.popup = popup;
          feature.layer.map.addPopup(popup);
          Drupal.openlayers.popup.selectedFeature = feature;
        },
        onUnselect: function(feature) {
          // Remove popup if feature is unselected.
          feature.layer.map.removePopup(feature.popup);
          feature.popup.destroy();
          feature.popup = null;
        }
      }
    );

    map.addControl(popupSelect);
    popupSelect.activate();
    Drupal.openlayers.popup.popupSelect = popupSelect;
  }
  }
}
})(jQuery);
