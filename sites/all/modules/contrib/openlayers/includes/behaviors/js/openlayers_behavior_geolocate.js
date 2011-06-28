
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Geolocate Control
 */
(function($) {
  Drupal.behaviors.openlayers_behavior_geolocate = {
    'attach': function(context, settings) {
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors['openlayers_behavior_geolocate']) {
        // Create Geolocate control
        var geolocate = new OpenLayers.Control.Geolocate(data.map.behaviors['openlayers_behavior_geolocate']);
        data.openlayers.addControl(geolocate);
        
        // Add some event handling
        geolocate.events.register("locationupdated", this, function(e) {
          data.openlayers.setCenter(new OpenLayers.Geometry.Point(e.point.x, e.point.y), data.map.behaviors['openlayers_behavior_geolocate'].zoom_level);
        });
        geolocate.events.register("locationfailed", this, function(e) {
          OpenLayers.Console.log('Location detection failed');
        });
        
        // Activiate!
        geolocate.activate();
        
      }
    }
  }
})(jQuery);
