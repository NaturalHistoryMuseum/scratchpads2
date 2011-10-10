(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3.polygon = Drupal.gm3.polygon || {};
  Drupal.behaviors.gm3_polygon = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if(Drupal.settings.gm3.maps[map_id]['initialized'] && Drupal.settings.gm3.maps[map_id]['libraries']['polygon'] && !Drupal.settings.gm3.maps[map_id]['polygon']) {
        Drupal.gm3.polygon.initialize(map_id);
      }
    }
  }};
  Drupal.gm3.polygon.initialize = function(map_id){
    // Polygon object.
    // We don't currently support geodesic shapes, mainly due to the library
    // we're using being a little buggy in its support for it. For this reason,
    // please avoid loading the geometry library.
    var geodesic = false;
    Drupal.settings.gm3.maps[map_id]['polygon'] = Drupal.settings.gm3.maps[map_id]['polygon'] || {};
    Drupal.settings.gm3.maps[map_id]['polygon']['followline1'] = new google.maps.Polyline({geodesic: geodesic, clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polygon']['followline2'] = new google.maps.Polyline({geodesic: geodesic, clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polygon']['polygons'] = new Array();
    // Clicked to start.
    $('#' + map_id + "-polygon").click(function(){
      $('.gm3-clicked').removeClass('gm3-clicked');
      $(this).parent().addClass('gm3-clicked');
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'crosshair'});
      var current_polygon = Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length;
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon] = new google.maps.Polygon({geodesic: geodesic, map: Drupal.settings.gm3.maps[map_id]['google_map'], strokeColor: Drupal.gm3.polygon.get_line_colour(current_polygon), strokeOpacity: 0.4, strokeWeight: 3, path: []});
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      // Listeners added to map and polygons.
      Drupal.gm3.polygon.add_listeners(Drupal.settings.gm3.maps[map_id]['google_map'], map_id, current_polygon);
      for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; i++) {
        Drupal.gm3.polygon.add_listeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], map_id, current_polygon);
      }
    });
  }
  Drupal.gm3.polygon.add_listeners = function(listener, map_id, current_polygon){
    google.maps.event.addListener(listener, 'mousemove', function(point){
      var pathLength = Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].getPath().getLength();
      if(pathLength >= 1) {
        var startingPoint1 = Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].getPath().getAt(pathLength - 1);
        var followCoordinates1 = [startingPoint1, point.latLng];
        Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setPath(followCoordinates1);
        var startingPoint2 = Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].getPath().getAt(0);
        var followCoordinates2 = [startingPoint2, point.latLng];
        Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setPath(followCoordinates2);
      }
    });
    google.maps.event.addListener(listener, 'rightclick', function(){
      // Unclick the button
      $('.gm3-clicked').removeClass('gm3-clicked');
      $('#gm3-default-button-'+map_id).addClass('gm3-clicked');
      // Remove listeners from map.
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setMap(null);
      google.maps.event.clearListeners(listener, "click");
      google.maps.event.clearListeners(listener, "mousemove");
      // google.maps.event.clearListeners(listener,
      // "rightclick");
      // Remove listeners from all polygons.
      Drupal.gm3.polygon.clear_listeners(map_id);
      // Add click lisener to all polygons to re-enable editing.
      Drupal.gm3.polygon.add_polygon_click_listeners(map_id);
      listener.setOptions({draggableCursor: 'pointer'});
    });
    google.maps.event.addListener(listener, 'click', function(point){
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].stopEdit();
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].getPath().push(point.latLng);
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].runEdit(true);
    });
  }
  Drupal.gm3.polygon.clear_listeners = function(map_id){
    for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; i++) {
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i].stopEdit();
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "rightclick");
    }
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
    // Add back the click to a polygon, so that they can be edited.
    Drupal.gm3.polygon.add_polygon_click_listeners(map_id);
  }
  Drupal.gm3.polygon.add_polygon_click_listeners = function(map_id){
    for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; i++) {
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "click", function(){
        // Clear the click listener from this map.
        google.maps.event.clearListeners(this, "click");
        // Once clicked, stop editing other polygons
        for(j = 0; j < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; j++) {
          Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][j].stopEdit();
        }
        // Add a click listener to the map so that we can finish editing.
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
        google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], "click", function(){
          for(j = 0; j < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; j++) {
            Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][j].stopEdit();
            Drupal.gm3.polygon.add_polygon_click_listeners(map_id);
          }
        });
        this.runEdit();
      });
    }

  }
  Drupal.gm3.polygon.get_line_colour = function(index){
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