
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Navigation Behavior
 */
(function($) {
Drupal.behaviors.openlayers_behavior_navigation = {
  'attach': function(context, settings) {
    var data = $(context).data('openlayers');
    if (data && data.map.behaviors['openlayers_behavior_navigation']) {
      // Add control
      var options = {
        'zoomWheelEnabled': data.map.behaviors['openlayers_behavior_navigation'].zoomWheelEnabled
      };
      var control = new OpenLayers.Control.Navigation(options);
      data.openlayers.addControl(control);
      control.activate();
    }
  }
};
})(jQuery);
