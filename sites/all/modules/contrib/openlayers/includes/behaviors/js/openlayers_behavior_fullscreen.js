
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Global variables to help with scope
 *
 * TODO: Move this to a better place, like the map data().
 */
Drupal.openlayers = Drupal.openlayers || {};
Drupal.openlayers.fullscreen = Drupal.openlayers.fullscreen || {};

(function($) {
/**
 * Attribution Behavior
 */
Drupal.behaviors.openlayers_behavior_fullscreen = {
  'attach': function(context, settings) {
    var fullscreenPanel;
    var data = $(context).data('openlayers');
    if (data && data.map.behaviors['openlayers_behavior_fullscreen']) {
      var opts = data.map.behaviors['openlayers_behavior_fullscreen'];

      // Create new panel control and add.
      fullscreenPanel = new OpenLayers.Control.Panel(
        {
          displayClass: 'openlayers_behavior_fullscreen_button_panel'
        }
      );
      data.openlayers.addControl(fullscreenPanel);

      // Create toggleing control and cutton.
      var toggler = OpenLayers.Function.bind(
        Drupal.openlayers.fullscreen.fullscreenToggle, data);
      var button = new OpenLayers.Control.Button({
        displayClass: 'openlayers_behavior_fullscreen_button',
        title: Drupal.t('Fullscreen'),
        trigger: toggler
      });
      fullscreenPanel.addControls([button]);

      // Make fullscreen by default if activited.
      if (opts['activated'] == true) {
        toggler();
      }
    }
  }
};

/**
 * Toggling function for FullScreen control.
 */
Drupal.openlayers.fullscreen.fullscreenToggle = function() {
  var map = this.openlayers;
  var $map = $(this.openlayers.div);
  var extent = map.getExtent();

  $map.parent().toggleClass('openlayers_map_fullscreen');
  $map.toggleClass('openlayers_map_fullscreen');
  $map.data('openlayers').openlayers.updateSize();
  $map.data('openlayers').openlayers.zoomToExtent(extent, true);
}
})(jQuery);
