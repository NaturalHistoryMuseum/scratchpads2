/*jslint white: false */
/*jslint forin: true */
/*global OpenLayers Drupal $ document jQuery window */

/**
 * @file
 * This file holds the main javascript API for OpenLayers. It is
 * responsable for loading and displaying the map.
 *
 * @ingroup openlayers
 */

/**
 * This is a workaround for a bug involving IE and VML support.
 * See the Drupal Book page describing this problem:
 * http://drupal.org/node/613002
 */

document.namespaces;

(function($) {

Drupal.settings.openlayers = {};
Drupal.settings.openlayers.maps = {};

/**
 * Minimal OpenLayers map bootstrap.
 * All additional operations occur in additional Drupal behaviors.
 */
Drupal.behaviors.openlayers = {
  'attach': function(context, settings) {
    if (typeof(Drupal.settings.openlayers) === 'object' &&
        Drupal.settings.openlayers.maps &&
        !$(context).data('openlayers')) {
      $('.openlayers-map:not(.openlayers-processed)').each(function() {
        // By setting the stop_render variable to TRUE, this will
        // halt the render process.  If set, one could remove this setting
        // then call Drupal.attachBehaviors again to get it started
        var map_id = $(this).attr('id');
        if (Drupal.settings.openlayers.maps[map_id] && Drupal.settings.openlayers.maps[map_id].stop_render != true) {
          var map = Drupal.settings.openlayers.maps[map_id];
          $(this).addClass('openlayers-processed');

          // Use try..catch for error handling.
          try {
            // Set OpenLayers language based on document language,
            // rather than browser language
            OpenLayers.Lang.setCode($('html').attr('lang'));

            $(this)
              // @TODO: move this into markup in theme function, doing this dynamically is a waste.
              .css('width', map.width)
              .css('height', map.height);

            var options = {};
            // This is necessary because the input JSON cannot contain objects
            options.projection = new OpenLayers.Projection('EPSG:' + map.projection);
            options.displayProjection = new OpenLayers.Projection('EPSG:' + map.displayProjection);

            // TODO: work around this scary code
            if (map.projection === '900913') {
              options.maxExtent = new OpenLayers.Bounds(
                -20037508.34, -20037508.34, 20037508.34, 20037508.34);
            }
            if (map.projection === '4326') {
              options.maxExtent = new OpenLayers.Bounds(-180, -90, 180, 90);
            }

            options.maxResolution = 1.40625;
            options.controls = [];

            // Change image, CSS, and proxy paths if specified
            if (map.image_path) {
              OpenLayers.ImgPath = Drupal.openlayers.relatePath(map.image_path,
                Drupal.settings.basePath);
            }
            if (map.css_path) {
              options.theme = Drupal.openlayers.relatePath(map.css_path,
                Drupal.settings.basePath);
            }
            if (map.proxy_host) {
              OpenLayers.ProxyHost = Drupal.openlayers.relatePath(map.proxy_host,
                Drupal.settings.basePath);
            }

            // Initialize openlayers map
            var openlayers = new OpenLayers.Map(map.id, options);

            // Run the layer addition first
            Drupal.openlayers.addLayers(map, openlayers);

            // Attach data to map DOM object
            $(this).data('openlayers', {'map': map, 'openlayers': openlayers});

            // Finally, attach behaviors
            Drupal.attachBehaviors(this);

            if ($.browser.msie) {
              Drupal.openlayers.redrawVectors();
            }
          }
          catch (e) {
            if (typeof console != 'undefined') {
              console.log(e);
            }
            else {
              $(this).text('Error during map rendering: ' + e);
            }
          }
        }
      });
    }
  }
};

/**
 * Collection of helper methods.
 */
Drupal.openlayers = {
  // Determine path based on format.
  'relatePath': function(path, basePath) {
    // Check for a full URL or an absolute path.
    if (path.indexOf('://') >= 0 || path.indexOf('/') == 0) {
      return path;
    }
    else {
      return basePath + path;
    }
  },
  /*
   * Redraw Vectors.
   * This is necessary because various version of IE cannot draw vectors on
   * $(document).ready()
   */
  'redrawVectors': function() {
    $(window).load(
      function() {
        var map;
        for (map in Drupal.settings.openlayers.maps) {
          $.each($('#' + map).data('openlayers')
            .openlayers.getLayersByClass('OpenLayers.Layer.Vector'),
            function(i, layer) {
              layer.redraw();
            }
          );
        }
      }
    );
  },
  /**
   * Add layers to the map
   *
   * @param map Drupal settings object for the map.
   * @param openlayers OpenLayers Map Object.
   */
  'addLayers': function(map, openlayers) {

    var sorted = [];
    for (var name in map.layers) {
      sorted.push({'name': name, 'weight': map.layers[name].weight });
    }
    sorted.sort(function(a, b) {
      var x = a.weight, y = b.weight;
      return ((x < y) ? -1 : ((x > y) ? 1 : 0));
    });

    for (var i = 0; i < sorted.length; ++i) {
      var layer,
        name = sorted[i].name,
        options = map.layers[name];

      // Add reference to our layer ID
      options.drupalID = name;
      // Ensure that the layer handler is available
      if (options.layer_handler !== undefined &&
        Drupal.openlayers.layer[options.layer_handler] !== undefined) {
        var layer = Drupal.openlayers.layer[options.layer_handler](map.layers[name].title, map, options);

        layer.visibility = !!(!map.layer_activated || map.layer_activated[name]);

        if (layer.isBaseLayer === false) {
          layer.displayInLayerSwitcher = (!map.layer_switcher || map.layer_switcher[name]);
        }

        if (map.center.wrapdateline === '1') {
          // TODO: move into layer specific settings
          layer.wrapDateLine = true;
        }

        openlayers.addLayer(layer);
      }
    }

    openlayers.setBaseLayer(openlayers.getLayersBy('drupalID', map.default_layer)[0]);

    // Zoom & center
    if (map.center.initial) {
      var center = OpenLayers.LonLat.fromString(map.center.initial.centerpoint).transform(
            new OpenLayers.Projection('EPSG:4326'),
            new OpenLayers.Projection('EPSG:' + map.projection));
      var zoom = parseInt(map.center.initial.zoom, 10);
      openlayers.setCenter(center, zoom, false, false);
    }

    // Set the restricted extent if wanted.
    // Prevents the map from being panned outside of a specfic bounding box.
    if (typeof map.center.restrict !== 'undefined' && map.center.restrict.restrictextent) {
      openlayers.restrictedExtent = OpenLayers.Bounds.fromString(
          map.center.restrict.restrictedExtent);
    }
  },
  /**
   * Abstraction of OpenLayer's feature adding syntax to work with Drupal output.
   * Ideally this should be rolled into the PHP code, because we don't want to manually
   * parse WKT
   */
  'addFeatures': function(map, layer, features) {
    var newFeatures = [];

    // Go through features
    for (var key in features) {
      var feature = features[key];
      var newFeatureObject = this.objectFromFeature(feature);

      // If we have successfully extracted geometry add additional
      // properties and queue it for addition to the layer
      if (newFeatureObject) {
        var newFeatureSet = [];

        // Check to see if it is a new feature, or an array of new features.
        if (typeof(newFeatureObject[0]) === 'undefined') {
          newFeatureSet[0] = newFeatureObject;
        }
        else {
          newFeatureSet = newFeatureObject;
        }

        // Go through new features
        for (var i in newFeatureSet) {
          var newFeature = newFeatureSet[i];

          // Transform the geometry if the 'projection' property is different from the map projection
          if (feature.projection) {
            if (feature.projection !== map.projection) {
              var featureProjection = new OpenLayers.Projection('EPSG:' + feature.projection);
              var mapProjection = new OpenLayers.Projection('EPSG:' + map.projection);
              newFeature.geometry.transform(featureProjection, mapProjection);
            }
          }

          // Add attribute data
          if (feature.attributes) {
            // Attributes belong to features, not single component geometries
            // of them. But we're creating a geometry for each component for
            // better performance and clustering support. Let's call these
            // "pseudofeatures".
            //
            // In order to identify the real feature each geometry belongs to
            // we then add a 'fid' parameter to the "pseudofeature".
            // NOTE: 'drupalFID' is only unique within a single layer.
            newFeature.attributes = feature.attributes;
            newFeature.data = feature.attributes;
            newFeature.drupalFID = key;
          }

          // Add style information
          if (feature.style) {
            newFeature.style = jQuery.extend({},
                OpenLayers.Feature.Vector.style['default'],
                feature.style);
          }

          // Push new features
          newFeatures.push(newFeature);
        }
      }
    }

    // Add new features if there are any
    if (newFeatures.length !== 0) {
      layer.addFeatures(newFeatures);
    }
  },
  'getStyleMap': function(map, layername) {
    if (map.styles) {
      var stylesAdded = {};
      // Grab and map base styles.
      for (var style in map.styles) {
        stylesAdded[style] = new OpenLayers.Style(map.styles[style]);
      }
      // Implement layer-specific styles.
      if (map.layer_styles !== undefined && map.layer_styles[layername]) {
        var style = map.layer_styles[layername];
        stylesAdded['default'] = new OpenLayers.Style(map.styles[style]);
      }
      return new OpenLayers.StyleMap(stylesAdded);
    }
    else {
      return new OpenLayers.StyleMap({
        'default': new OpenLayers.Style({
          pointRadius: 5,
          fillColor: '#ffcc66',
          strokeColor: '#ff9933',
          strokeWidth: 4,
          fillOpacity: 0.5
        }),
        'select': new OpenLayers.Style({
          fillColor: '#66ccff',
          strokeColor: '#3399ff'
        })
      });
    }
  },
  'objectFromFeature': function(feature) {
    var wktFormat = new OpenLayers.Format.WKT();
    // Extract geometry either from wkt property or lon/lat properties
    if (feature.wkt) {
      return wktFormat.read(feature.wkt);
    }
    else if (feature.lon) {
      return wktFormat.read('POINT(' + feature.lon + ' ' + feature.lat + ')');
    }
  }
};

Drupal.openlayers.layer = {};
})(jQuery);
