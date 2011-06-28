
/**
 * @file
 * This file holds the javascript functions for the map UI
 *
 * @ingroup openlayers
 */

/**
 * Test whether function exists, 
 * even if it is the child of another object
 * @param head the function name as a string,
 *  optionally with dots for invoking children
 * @return bool true or false for existence
 */
function function_exists(head) {
  return _function_exists(head.split('.'), window);
}

function _function_exists(head, f) {
  if (head.length == 0) {
    return true;
  }
  h = head.shift();
  if (typeof f[h] !== 'undefined') {
    return _function_exists(head, f[h]);
  }
  else {
    return false;
  }
}

(function ($) {
/**
 * Drupal behaviors for OpenLayers UI form.
 */
Drupal.behaviors.openlayers_ui = {
  'attach': function(context, settings) {
    
    // TODO: Review the following
    /*
    // Automatic options.  We do it here, instead of in Form API because
    // Form API enforces the disabled
    $("#edit-options-automatic-options:not(.openlayers-ui-processed)").each(function() {
      $(this).addClass('openlayers-ui-processed');
      $(this).change(function() {
        var $thisCheck = $(this);
        var $autoOptions = $thisCheck.parent()
          .parent()
          .parent()
          .find('input:not("#edit-options-automatic-options")');
        if ($thisCheck.is(':checked')) {
          $autoOptions.attr('disabled', 'disabled');
        }
        else {
          $autoOptions.removeAttr('disabled');
        }
      });
  
      // When form is submitted, if disabled, FAPI does not read values   
      $(this).parents('form').submit(function() {
        $("#edit-options-automatic-options").attr('checked', false).trigger('change');
      });
      $(this).trigger('change');
    });
  
    // Update map positioning when text fields are changed.
    $("#edit-center-lat:not(.openlayers-ui-processed), #edit-center-lon:not(.openlayers-ui-processed), #edit-center-zoom:not(.openlayers-ui-processed)").each(function() {
      $(this).addClass('openlayers-ui-processed');
      $(this).change(function() {
        Drupal.openlayers_ui.updateMapCenter();
      });
    });
    */
  
    // mark openlayers dependencies as valid or invalid
    $('.openlayers-dependency-flag').each(function() {
      if (!function_exists($(this).find('.openlayers-dependency-value').text())) {
        $(this).find('.openlayers-dependency-broken').show();
      }
    });
    
    // Since CTools dependency is not working
    $('#edit-behaviors:not(.openlayers-behaviors-checks-processed)').each(function () {
      $('#edit-behaviors').addClass('openlayers-behaviors-checks-processed');
      $('#edit-behaviors table tbody tr td > div.form-type-checkbox').each(function () {
        var $thisBehavior = $(this);
        var $thisCheck = $('input[type=checkbox]', $thisBehavior);

        if ($thisCheck.attr('checked')) {
          $thisBehavior.siblings().show();
        }
        else {
          $thisBehavior.siblings().hide(); 
        }
        
        $thisCheck.click(function() {
          if ($thisCheck.attr('checked')) {
            $thisBehavior.siblings().show();
          }
          else {
            $thisBehavior.siblings().hide(); 
          }
        });
      
      });
    });
  
    // Run once on load.
    Drupal.openlayers_ui.updateMapCenter();
  }
};

/**
 * Register form center value updating events.
 */
Drupal.behaviors.openlayers_ui_center = {
  'attach': function(context, settings) {
    var data = $(context).data('openlayers');
    if (data) {
      data.openlayers.events.register('moveend', data.map, function() {
          Drupal.openlayers_ui.updateCenterFormValues()
      });
      data.openlayers.events.register('zoomend', data.map, function() {
          Drupal.openlayers_ui.updateCenterFormValues()
      });
    }
  }
};

/**
 * Helper functions.
 */
Drupal.openlayers_ui = {

  /**
   * Update the center of the helpmap using the values from the form
   *
   * Take the center lat, lon and zoom values from the form and update
   * the helper map.
   */
  'updateMapCenter': function() {
    var data = $('#openlayers-center-helpmap').data('openlayers');
    if (data) {
      var projection = $('#edit-projections-projection').val();
      var zoom = $('#edit-center-zoom').val();
      var lonlat = $('#edit-center-initial-centerpoint').val();
      if (typeof lonlat == Array) {
        // Create new center
        var center = new OpenLayers.LonLat(
            parseFloat(lonlat[0]),
            parseFloat(lonlat[1]));
        // Transform for projection
        center.transform(
            new OpenLayers.Projection('EPSG:' + projection),
            new OpenLayers.Projection('EPSG:4326'));
        // Set center of map.
        data.openlayers.setCenter(center, zoom);
      }
    }
  },

  /**
   * Event callback for updating center form field values when map 
   * is dragged or zoomed.
   */
  'updateCenterFormValues': function() {
    var data = $('#openlayers-center-helpmap').data('openlayers');
    if (data) {
      var helpmap = data.openlayers;
      var projection = $('#edit-projections-projection').val();
      var zoom = helpmap.getZoom();
      var center = helpmap.getCenter();

      // Transform center
      center.transform(
          new OpenLayers.Projection('EPSG:4326'),
          new OpenLayers.Projection('EPSG:' + projection));

      // Get new lat and lon
      var lat = center.lat;
      var lon = center.lon;

      // Set new values
      $('#edit-center-zoom').val(zoom);
      $('#edit-center-lat').val(lat);
      $('#edit-center-lon').val(lon);
    }
  }
}
})(jQuery);
