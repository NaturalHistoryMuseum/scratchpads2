
/**
 * @file
 * DrawFeatures Behavior
 */

/**
 * Behavior for Draw Features
 */
(function($) {
Drupal.behaviors.openlayers_behavior_drawfeatures = {
    'attach': function(context, settings) {
      function openlayers_behavior_drawfeatures_update(features) {
        WktWriter = new OpenLayers.Format.WKT();
        while (features.type == 'featureadded' && this.feature_limit &&
          (this.feature_limit < features.object.features.length)) {
          features.feature.layer.removeFeatures(features.object.features.shift());
        }
  
        var features_copy = features.object.clone();
        for (var i in features_copy.features) {
          features_copy.features[i].geometry.transform(
            features.object.map.projection,
            new OpenLayers.Projection('EPSG:4326')
          );
        }
        this.element.val(WktWriter.write(features_copy.features));
      }
  
      var data = $(context).data('openlayers');
      if (data && data.map.behaviors['openlayers_behavior_drawfeatures']) {
        var feature_types = data.map.behaviors['openlayers_behavior_drawfeatures'].feature_types;
        this.element =
          $('#' + data.map.behaviors['openlayers_behavior_drawfeatures'].element_id);
        this.feature_limit =
          data.map.behaviors['openlayers_behavior_drawfeatures'].feature_limit;
        var dataLayer = new OpenLayers.Layer.Vector(
          Drupal.t('Feature Layer'),
          {
            projection: new OpenLayers.Projection('EPSG:4326'),
            drupalID: 'openlayers_drawfeatures_layer'
          }
        );
        dataLayer.styleMap = Drupal.openlayers.getStyleMap(data.map, 'openlayers_drawfeatures_layer');
        data.openlayers.addLayer(dataLayer);
  
        if (this.element.text() != '') {
          var wktFormat = new OpenLayers.Format.WKT();
          var features = wktFormat.read(this.element.text());
          if (features.constructor == Array) {
            for (var i in features) {
              features[i].geometry = features[i].geometry.transform(
                new OpenLayers.Projection('EPSG:4326'),
                data.openlayers.projection
              );
            }
          }
          else {
            features.geometry = features.geometry.transform(
              new OpenLayers.Projection('EPSG:4326'),
              data.openlayers.projection
            );
            features = [features];
          }
          dataLayer.addFeatures(features);
        }
  
        // registering events late, because adding data
        // would result in a reprojection loop
        dataLayer.events.register('featureadded', this,
          openlayers_behavior_drawfeatures_update);
        dataLayer.events.register('featureremoved', this,
          openlayers_behavior_drawfeatures_update);
        dataLayer.events.register('featuremodified', this,
          openlayers_behavior_drawfeatures_update);
  
  
  
        var control = new OpenLayers.Control.EditingToolbar(dataLayer);
        data.openlayers.addControl(control);
        control.activate();
  
        // Build an array of the requested feature classes
        var feature_classmap = {
          'point': 'OpenLayers.Handler.Point',
          'path': 'OpenLayers.Handler.Path',
          'polygon': 'OpenLayers.Handler.Polygon'
        };
  
        var feature_classes = [];
        for (var i in feature_types) {
          if (feature_types[i] !== 0) {
            feature_classes.push(feature_classmap[feature_types[i]]);
          }
        }
  
        // Reconstruct editing toolbar controls so to only contain
        // the tools for the requested feature types / classes
        // plus the navigation tool
        control.controls = $.map(control.controls,
          function(control) {
            return (control.CLASS_NAME == 'OpenLayers.Control.Navigation' ||
              $.inArray(control.handler.CLASS_NAME, feature_classes) != -1)
              ? control : null;
          }
        );
  
        control.activateControl(control.getControlsByClass('OpenLayers.Control.Navigation')[0]);
        control.redraw();
  
        this.element.parents('form').bind('submit',
          {
            control: control,
            dataLayer: dataLayer
          }, function(evt) {
            $.map(evt.data.control.controls, function(c) { c.deactivate(); });
            dataLayer.events.triggerEvent('featuremodified');
          }
        );
  
        // Add modify feature tool
        control.addControls(new OpenLayers.Control.ModifyFeature(
          dataLayer, {
            displayClass: 'olControlModifyFeature',
            deleteCodes: [46, 68, 100],
            handleKeypress: function(evt) {
              if (this.feature && $.inArray(evt.keyCode, this.deleteCodes) > -1) {
                // We must unselect the feature before we delete it
                var feature_to_delete = this.feature;
                this.selectControl.unselectAll();
                this.layer.removeFeatures([feature_to_delete]);
              }
            }
          }
          )
        );
      }
  }
  }
})(jQuery);
