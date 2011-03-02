
/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Map Form Values Behavior
 */
(function($) {
Drupal.behaviors.openlayers_behavior_mapformvalues = {
  'attach': function(context, settings) {
    var data = $(context).data('openlayers');
    function updateForm(evt) {
      if (evt.object.centerpoint_form) {
        center = evt.object.getCenter().transform(
          evt.object.projection,
          new OpenLayers.Projection('EPSG:4326')).toShortString();
          evt.object.centerpoint_form.val(center);
      }
      if (evt.object.zoom_form) {
        zoom = evt.object.getZoom();
        evt.object.zoom_form.val(zoom);
      }
    }
    if (data && data.map.behaviors['openlayers_behavior_mapformvalues']) {
      centerpoint_form = $(data.map.behaviors['openlayers_behavior_mapformvalues'].center_form);
      zoom_form = $(data.map.behaviors['openlayers_behavior_mapformvalues'].zoom_form);

      if (centerpoint_form.length) {
        data.openlayers.centerpoint_form = centerpoint_form;
        center_point = centerpoint_form.val();
        data.openlayers.setCenter(
          OpenLayers.LonLat.fromString(center_point).transform(
            new OpenLayers.Projection('EPSG:4326'),
            data.openlayers.projection)
          );
      }

      if (zoom_form.length) {
        data.openlayers.zoom_form = zoom_form;
        zoom = zoom_form.val();
        data.openlayers.zoomTo(parseInt(zoom));
      }

      data.openlayers.events.on({'moveend': updateForm});
    }
  }
}
})(jQuery);
