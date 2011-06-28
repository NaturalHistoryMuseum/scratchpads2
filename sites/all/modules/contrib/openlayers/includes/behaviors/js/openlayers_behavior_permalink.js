
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Permalink Behavior
 */
(function($) {
  Drupal.behaviors.openlayers_behavior_permalink = {
    'attach': function(context, settings) {
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors['openlayers_behavior_permalink']) {
        // Add control
        var control = new OpenLayers.Control.Permalink();
        data.openlayers.addControl(control);
        control.activate();
      }
    }
  }
})(jQuery);
