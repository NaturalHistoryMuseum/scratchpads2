/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */


(function($) {
/**
 * Geofield Behavior
 */
Drupal.behaviors.openlayers_behavior_geofield = {
  'attach': function(context, settings) {
    var data = $(context).data('openlayers');

    /*
     * Helper method called on addFeature
     */
    function setBounds(box) {
      bounding_box = box.geometry.getBounds().transform(
        box.layer.map.projection,
        new OpenLayers.Projection('EPSG:4326')).toArray();
      for (var j = 0; j < 4; j++) {
        box.layer.map.bounds_form[j].val(bounding_box[j]);
      }
      for(i = 0; i < selections_layer.features.length; i++) {
        if(selections_layer.features[i] != box) {
          selections_layer.features[i].destroy();
        }
      }
    }

    /*
     * Helper method called on addFeature
     */
    function setPoint(point) {
      p = point.clone().geometry.transform(
        point.layer.map.projection,
        new OpenLayers.Projection('EPSG:4326'));

        point.layer.map.centerpoint_form[0].val(p.x);
        point.layer.map.centerpoint_form[1].val(p.y);

        for (var i = 0; i < point_selections_layer.features.length; i++) {
          if (point_selections_layer.features[i] != point) {
            point_selections_layer.features[i].destroy();
          }
        }
      }

    if (data && data.map.behaviors['openlayers_behavior_geofield']) {

      centerpoint_form =
        [$(data.map.behaviors['openlayers_behavior_geofield'].centerpoint['lon']),
         $(data.map.behaviors['openlayers_behavior_geofield'].centerpoint['lat'])];

      bounds_form =
        [$(data.map.behaviors['openlayers_behavior_geofield'].bounds[0]),
         $(data.map.behaviors['openlayers_behavior_geofield'].bounds[1]),
         $(data.map.behaviors['openlayers_behavior_geofield'].bounds[2]),
         $(data.map.behaviors['openlayers_behavior_geofield'].bounds[3])];

      data.openlayers.centerpoint_form = centerpoint_form;
      data.openlayers.bounds_form = bounds_form;

      center_point = centerpoint_form[0].val() + ', ' + centerpoint_form[1].val();

      /*
       * Point Drawing
       */

      point_selections_layer = new OpenLayers.Layer.Vector('Temporary Point Layer');
      point_control = new OpenLayers.Control.DrawFeature(
        point_selections_layer,
        OpenLayers.Handler.Point,
        {
          featureAdded: setPoint
        }
      );
      data.openlayers.addLayer(point_selections_layer);
      data.openlayers.addControl(point_control);
      point_control.activate();

      /*
       * Bounds drawing
       */
      selections_layer = new OpenLayers.Layer.Vector('Temporary Box Layer');

      control = new OpenLayers.Control.DrawFeature(
        selections_layer,
        OpenLayers.Handler.RegularPolygon,
        {
          featureAdded: setBounds
        }
      );

      control.handler.setOptions({
          'keyMask': OpenLayers.Handler.MOD_SHIFT,
          'sides': 4,
          'irregular': true});

      data.openlayers.addLayer(selections_layer);
      data.openlayers.addControl(control);
      control.activate();

      /*
       * Draw box if the form has values
       */
      if (centerpoint_form[0].val()) {
        geometry = new OpenLayers.Geometry.Point(
          centerpoint_form[0].val(),
          centerpoint_form[1].val()).transform(
            new OpenLayers.Projection('EPSG:4326'),
            data.openlayers.projection);
        feature = new OpenLayers.Feature.Vector(geometry);
        point_selections_layer.addFeatures([feature]);
      }

      /*
       * Draw box if the form has values
       */
      bbox = $.map(bounds_form, function(a){ return a.val(); }).join(',');
      if (bbox != ',,,,') {
        geometry = new OpenLayers.Bounds.fromString(bbox).toGeometry().transform(
            new OpenLayers.Projection('EPSG:4326'),
            data.openlayers.projection);
        feature = new OpenLayers.Feature.Vector(geometry);
        selections_layer.addFeatures([feature]);
      }
    }
  }
};
})(jQuery);
