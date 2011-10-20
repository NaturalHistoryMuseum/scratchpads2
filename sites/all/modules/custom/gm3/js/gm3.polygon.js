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
    // Add Polygons sent from server.
    if(this.GM3.libraries.polygon.polygons) {
      for( var i in this.GM3.libraries.polygon.polygons) {
        this.add_polygon(this.GM3.libraries.polygon.polygons[i]);
      }
    }
    this.add_transfer_listeners();
  }
  Drupal.GM3.polygon.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'crosshair'});
    this.polygons[this.polygons.length] = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: []});
    this.followline1.setPath([]);
    this.followline2.setPath([]);
    this.followline1.setMap(this.GM3.google_map);
    this.followline2.setMap(this.GM3.google_map);
  }
  Drupal.GM3.polygon.prototype.add_polygon = function(points, editable){
    editable = typeof (editable) != 'undefined' ? editable : true;
    var path_points = new Array();
    for( var i = 0; i < points.length; i++) {
      path_points[i] = new google.maps.LatLng(points[i]['lat'], points[i]['long'])
    }
    if(editable) {
      this.polygons[this.polygons.length] = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: path_points});
    } else {
      var polygon = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: '#000000', strokeOpacity: 0.4, strokeWeight: 1, path: path_points});
      this.GM3.add_listeners_helper(polygon);
    }
  }
  Drupal.GM3.polygon.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'polygon':
        switch(event_type){
          case 'click':
            this.polygons[this.polygons.length - 1].stopEdit();
            this.polygons[this.polygons.length - 1].getPath().push(event.latLng);
            this.polygons[this.polygons.length - 1].runEdit(true);
            break;
          case 'mousemove':
            var pathLength = this.polygons[this.polygons.length - 1].getPath().getLength();
            if(pathLength >= 1) {
              var startingPoint1 = this.polygons[this.polygons.length - 1].getPath().getAt(pathLength - 1);
              var followCoordinates1 = [startingPoint1, event.latLng];
              this.followline1.setPath(followCoordinates1);
              var startingPoint2 = this.polygons[this.polygons.length - 1].getPath().getAt(0);
              var followCoordinates2 = [startingPoint2, event.latLng];
              this.followline2.setPath(followCoordinates2);
            }
            break;
          case 'rightclick':
            this.GM3.set_active_class('default');
            this.followline1.setMap(null);
            this.followline2.setMap(null);
            break;
        }
        break;
      case 'default':
        switch(event_type){
          case 'click':
            if(event_object.getClass && event_object.getClass() == 'Polygon') {
              // Once clicked, stop editing other polygons
              for( var j = 0; j < this.polygons.length; j++) {
                this.polygons[j].stopEdit();
              }
              event_object.runEdit();
            } else {
              // Clicked elsewhere, stop editing.
              for( var j = 0; j < this.polygons.length; j++) {
                this.polygons[j].stopEdit();
              }
            }
            break;
        }
        break;
    }
  }
  Drupal.GM3.polygon.prototype.add_transfer_listeners = function(){
    for( var i = 0; i < this.polygons.length; i++) {
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
})(jQuery);