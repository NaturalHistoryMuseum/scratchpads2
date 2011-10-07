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
    // we're using being a little buggy in its support for it.  For this reason,
    // please avoid loading the 
    var geodesic = false;
    Drupal.settings.gm3.maps[map_id]['polygon'] = Drupal.settings.gm3.maps[map_id]['polygon'] || {};
    Drupal.settings.gm3.maps[map_id]['polygon']['followline1'] = new google.maps.Polyline({geodesic: geodesic, clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polygon']['followline2'] = new google.maps.Polyline({geodesic: geodesic, clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polygon']['polygons'] = new Array();
    // Clicked to start.
    document.getElementById(map_id + "-polygon").onclick = function(){
      var current_polygon = Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length;
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'crosshair'});
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon] = new google.maps.Polygon({geodesic: geodesic, map: Drupal.settings.gm3.maps[map_id]['google_map'], strokeColor: Drupal.gm3.polygon.get_line_colour(current_polygon), strokeOpacity: 0.4, strokeWeight: 3, path: []});
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      // Listeners added to map.
      Drupal.gm3.polygon.add_listeners(map_id, Drupal.settings.gm3.maps[map_id]['google_map'], current_polygon);
      // Add listeners to each previous polygon.
      for(i = 0; i <= current_polygon; i++) {
        Drupal.gm3.polygon.add_listeners(map_id, Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], current_polygon);
        // Stop editing the other polygons.
        Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i].stopEdit();
      }
    }
  }
  Drupal.gm3.polygon.add_listeners = function(map_id, adding_to, current_polygon){
    google.maps.event.addListener(adding_to, 'mousemove', function(point){
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
    google.maps.event.addListener(adding_to, 'rightclick', function(){
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setMap(null);
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      for(i = 0; i <= current_polygon; i++) {
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "click");
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "mousemove");
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], "rightclick");
      }
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'pointer'});
    });
    google.maps.event.addListener(adding_to, 'click', function(point){
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].stopEdit();
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].getPath().push(point.latLng);
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][current_polygon].runEdit(true);
    });
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
