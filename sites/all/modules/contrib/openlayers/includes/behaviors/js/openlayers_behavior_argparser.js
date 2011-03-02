
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */


(function($) {
/**
 * ArgParser Behavior
 */
Drupal.behaviors.openlayers_behavior_argparser = {
  'attach': function(context, settings) {
    var data = $(context).data('openlayers');
    if (data && data.map.behaviors['openlayers_behavior_argparser']) {
      // Add control
      var control = new OpenLayers.Control.ArgParser();
      data.openlayers.addControl(control);
      control.activate();
    }
  }
};
})(jQuery);
