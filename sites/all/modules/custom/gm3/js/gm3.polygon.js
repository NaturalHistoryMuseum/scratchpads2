(function($){
  Drupal.GM3.polygon = function(map){
    this.GM3 = map;
    // Polygon object.
    // We don't currently support geodesic shapes, mainly due to the library
    // we're using being a little buggy in its support for it. For this reason,
    // please avoid loading the geometry library.
    this.geodesic = false;
    // Editing lines
    this.followline1 = new google.maps.Polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
    this.followline2 = new google.maps.Polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
    // Polygons.
    this.polygons = new Array();
  }

  Drupal.GM3.polygon.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'crosshair'});
    this.polygons[this.polygons.length] = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: []});
    this.followline1.setPath([]);
    this.followline2.setPath([]);
    this.followline1.setMap(this.GM3.google_map);
    this.followline2.setMap(this.GM3.google_map);
  }
  Drupal.GM3.polygon.prototype.event = function(event_type, event){
    switch(event_type){
      case 'click':
        this.polygons[this.polygons.length-1].stopEdit();
        this.polygons[this.polygons.length-1].getPath().push(event.latLng);
        this.polygons[this.polygons.length-1].runEdit(true);
        break;
      case 'mousemove':
        var pathLength = this.polygons[this.polygons.length-1].getPath().getLength();
        if(pathLength >= 1) {
          var startingPoint1 = this.polygons[this.polygons.length-1].getPath().getAt(pathLength - 1);
          var followCoordinates1 = [startingPoint1, event.latLng];
          this.followline1.setPath(followCoordinates1);
          var startingPoint2 = this.polygons[this.polygons.length-1].getPath().getAt(0);
          var followCoordinates2 = [startingPoint2, event.latLng];
          this.followline2.setPath(followCoordinates2);
        }
        break;
      case 'rightclick':
        this.GM3.set_active_class('default');
        break;
    }
  }
  Drupal.GM3.polygon.add_transfer_listeners = function(){
    for(i = 0; i < this.polygons.length; i++) {
      if(this.polygons[i]) {
        this.GM3.add_listeners_helper(this.polygons[i]);
      }
    }
  }
  Drupal.GM3.polygon.prototype.get_line_colour = function(){
    switch(this.polygons.length % 8){
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
  // Clear all listeners.
  /*Drupal.gm3.polygon.clear_listeners = function(map_id){
    for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; i++) {
      Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i].stopEdit();
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], 'click');
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], 'mousemove');
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], 'rightclick');
    }
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], 'click');
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], 'mousemove');
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], 'rightclick');
  }
  // Add listeners to each polygon to allow them to be clicked on for editing.
  Drupal.gm3.polygon.add_edit_listeners = function(map_id){
    for(i = 0; i < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; i++) {
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][i], 'click', function(){
        // Clear the click listener from this map.
        google.maps.event.clearListeners(this, 'click');
        // Once clicked, stop editing other polygons
        for(j = 0; j < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; j++) {
          Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][j].stopEdit();
        }
        // Add a click listener to the map so that we can finish editing.
        google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], 'click');
        google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], 'click', function(){
          for(j = 0; j < Drupal.settings.gm3.maps[map_id]['polygon']['polygons'].length; j++) {
            Drupal.settings.gm3.maps[map_id]['polygon']['polygons'][j].stopEdit();
            Drupal.gm3.add_edit_listeners(map_id);
          }
        });
        this.runEdit();
      });
    }
  }*/
})(jQuery);