(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3.polyline = Drupal.gm3.polyline || {};
  Drupal.behaviors.gm3_polyline = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if(Drupal.settings.gm3.maps[map_id]['initialized'] && Drupal.settings.gm3.maps[map_id]['libraries']['polyline'] && !Drupal.settings.gm3.maps[map_id]['polyline']) {
        Drupal.gm3.polyline.initialize(map_id);
      }
    }
  }};
  Drupal.gm3.polyline.initialize = function(map_id){
    // Polygon object.
    // We don't currently support geodesic shapes, mainly due to the library
    // we're using being a little buggy in its support for it. For this reason,
    // please avoid loading the
    var geodesic = false;
    Drupal.settings.gm3.maps[map_id]['polyline'] = Drupal.settings.gm3.maps[map_id]['polyline'] || {};
    Drupal.settings.gm3.maps[map_id]['polyline']['followline1'] = new google.maps.Polyline({geodesic: geodesic, clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polyline']['followline2'] = new google.maps.Polyline({geodesic: geodesic, clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polyline']['polylines'] = new Array();
    // Clicked to start.
    $('#' + map_id + "-polyline").click(function(){
      $('.gm3-clicked').removeClass('gm3-clicked');
      $(this).parent().addClass('gm3-clicked');
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'crosshair'});
      var current_polyline = Drupal.settings.gm3.maps[map_id]['polyline']['polylines'].length;
      Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline] = new google.maps.Polygon({geodesic: geodesic, map: Drupal.settings.gm3.maps[map_id]['google_map'], strokeColor: Drupal.gm3.polyline.get_line_colour(current_polyline), strokeOpacity: 0.4, strokeWeight: 3, path: []});
      Drupal.settings.gm3.maps[map_id]['polyline']['followline1'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polyline']['followline2'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polyline']['followline1'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      Drupal.settings.gm3.maps[map_id]['polyline']['followline2'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      // Listeners added to map and polylines.
      Drupal.gm3.polyline.add_listeners(map_id, current_polyline);
    });
  }
  Drupal.gm3.polyline.add_listeners = function(map_id, current_polyline){
    google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'mousemove', function(point){
      var pathLength = Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline].getPath().getLength();
      if(pathLength >= 1) {
        var startingPoint1 = Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline].getPath().getAt(pathLength - 1);
        var followCoordinates1 = [startingPoint1, point.latLng];
        Drupal.settings.gm3.maps[map_id]['polyline']['followline1'].setPath(followCoordinates1);
        var startingPoint2 = Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline].getPath().getAt(0);
        var followCoordinates2 = [startingPoint2, point.latLng];
        Drupal.settings.gm3.maps[map_id]['polyline']['followline2'].setPath(followCoordinates2);
      }
    });
    google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'rightclick', function(){
      // Unclick the button
      $('.gm3-clicked').removeClass('gm3-clicked');
      $('#gm3-default-button-'+map_id).addClass('gm3-clicked');
      // Remove listeners from map.
      Drupal.settings.gm3.maps[map_id]['polyline']['followline1'].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polyline']['followline2'].setMap(null);
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      // google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'],
      // "rightclick");
      // Remove listeners from all polylines.
      for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polyline']['polylines'].length; i++) {
        Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][i].stopEdit();
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][i], "click");
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][i], "mousemove");
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][i], "rightclick");
      }
      // Add click lisener to all polylines to re-enable editing.
      Drupal.gm3.polyline.add_polyline_click_listeners(map_id);
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'pointer'});
    });
    google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'click', function(point){
      Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline].stopEdit();
      Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline].getPath().push(point.latLng);
      Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][current_polyline].runEdit(true);
    });
  }
  Drupal.gm3.polyline.add_polyline_click_listeners = function(map_id){
    for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polyline']['polylines'].length; i++) {
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][i], "click", function(){
        // Clear the click listener from this map.
        google.maps.event.clearListeners(this, "click");
        // Once clicked, stop editing other polylines
        for(j = 0; j < Drupal.settings.gm3.maps[map_id]['polyline']['polylines'].length; j++) {
          Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][j].stopEdit();
        }
        // Add a click listener to the map so that we can finish editing.
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
        google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], "click", function(){
          for(j = 0; j < Drupal.settings.gm3.maps[map_id]['polyline']['polylines'].length; j++) {
            Drupal.settings.gm3.maps[map_id]['polyline']['polylines'][j].stopEdit();
            Drupal.gm3.polyline.add_polyline_click_listeners(map_id);
          }
        });
        this.runEdit();
      });
    }

  }
  Drupal.gm3.polyline.get_line_colour = function(index){
    switch(index % 8){
      default:
      case 0:
        return '#ff0000';
      case 1:
        return '#00ff00';
      case 2:
        return '#0000ff';
      case 3:
        return '#ffff00';
      case 4:
        return '#ff00ff';
      case 5:
        return '#00ffff';
      case 6:
        return '#000000';
      case 7:
        return '#ffffff';
    }
  }
})(jQuery);