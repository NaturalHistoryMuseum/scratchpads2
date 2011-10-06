(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.behaviors.gm3_polygon = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if(Drupal.settings.gm3.maps[map_id]['initialized'] && Drupal.settings.gm3.maps[map_id]['libraries']['polygon'] && !Drupal.settings.gm3.maps[map_id]['polygon']) {
        Drupal.gm3.initialize(map_id);
      }
    }
  }};
  Drupal.gm3.initialize = function(map_id){
    Drupal.settings.gm3.maps[map_id]['polygon'] = Drupal.settings.gm3.maps[map_id]['polygon'] || {};
    Drupal.settings.gm3.maps[map_id]['polygon']['followline1'] = new google.maps.Polyline({clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polygon']['followline2'] = new google.maps.Polyline({clickable: false, map: Drupal.settings.gm3.maps[map_id]['google_map'], path: [], strokeColor: "#787878", strokeOpacity: 1, strokeWeight: 2});
    Drupal.settings.gm3.maps[map_id]['polygon']['polygon'] = new google.maps.Polygon({
      map: Drupal.settings.gm3.maps[map_id]['google_map'],
      strokeColor: '#ff0000',
      strokeOpacity: 0.6,
      strokeWeight: 4,
      path: []});
    Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].runEdit(true);
    document.getElementById("block-menu-devel").onclick = function(){
      Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].stopEdit();
      Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polygon']['polygon'] = null;
      // Add some text about right clicking to end.
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'crosshair'});
      Drupal.settings.gm3.maps[map_id]['polygon']['polygon'] = new google.maps.Polygon({map: Drupal.settings.gm3.maps[map_id]['google_map'], strokeColor: '#ff0000', strokeOpacity: 0.6, strokeWeight: 4, path: []});
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setPath([]);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'click', function(point){
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].stopEdit();
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().push(point.latLng);
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].runEdit(true);
      });
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'rightclick', function(){
        Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setMap(null);
        Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setMap(null);
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
        Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'pointer'});
      });
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['polygon']['polygon'], 'click', function(){
        alert('Clicked on shape');
      })
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'mousemove', function(point){
        var pathLength = Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().getLength();
        if(pathLength >= 1) {
          var startingPoint1 = Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().getAt(pathLength - 1);
          var followCoordinates1 = [startingPoint1, point.latLng];
          Drupal.settings.gm3.maps[map_id]['polygon']['followline1'].setPath(followCoordinates1);

          var startingPoint2 = Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().getAt(0);
          var followCoordinates2 = [startingPoint2, point.latLng];
          Drupal.settings.gm3.maps[map_id]['polygon']['followline2'].setPath(followCoordinates2);
        }
      });
    }
  }
})(jQuery);