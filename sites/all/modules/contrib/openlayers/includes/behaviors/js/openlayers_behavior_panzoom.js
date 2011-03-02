
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Pan Zoom Bar Behavior
 */
(function($) {
  Drupal.behaviors.openlayers_behavior_panzoom = {
    'attach': function(context, settings) {
    var data = $(context).data('openlayers');
    if (data && data.map.behaviors['openlayers_behavior_panzoom']) {
      // Add control
      var control = new OpenLayers.Control.PanZoom();
      data.openlayers.addControl(control);
      control.activate();
    }
  }
}
})(jQuery);
