
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

(function($) {
  /**
   * DragPan Behavior
   */
  Drupal.behaviors.openlayers_behavior_dragpan = {
    'attach': function(context, settings) {
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors['openlayers_behavior_dragpan']) {
        // Add control
        var control = new OpenLayers.Control.DragPan();
        data.openlayers.addControl(control);
        control.activate();
      }
    }
  }
})(jQuery);
