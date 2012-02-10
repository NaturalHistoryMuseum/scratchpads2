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
        if(typeof (this.GM3.libraries.polygon.polygons[i]['polygon']) == 'undefined') {
          this.add_polygon(this.GM3.libraries.polygon.polygons[i]);
        } else {
          var content = typeof (this.GM3.libraries.polygon.polygons[i]['content']) != 'undefined' ? this.GM3.libraries.polygon.polygons[i]['content'] : '';
          var title = typeof (this.GM3.libraries.polygon.polygons[i]['title']) != 'undefined' ? this.GM3.libraries.polygon.polygons[i]['title'] : '';
          this.add_polygon(this.GM3.libraries.polygon.polygons[i]['polygon'], this.GM3.libraries.polygon.polygons[i]['editable'], content);
        }
      }
    }
    this.add_transfer_listeners();
  }
  Drupal.GM3.polygon.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
    this.polygons[this.polygons.length] = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: []});
    this.followline1.setPath([]);
    this.followline2.setPath([]);
    this.followline1.setMap(this.GM3.google_map);
    this.followline2.setMap(this.GM3.google_map);
  }
  Drupal.GM3.polygon.prototype.add_polygon = function(points, editable, content, title){
    editable = typeof (editable) != 'undefined' ? editable : true;
    var path_points = new Array();
    for( var i = 0; i < points.length; i++) {
      if(points[i]['lat'] == undefined) {
        // We have a string rather than an array, split it
        if(typeof points[i] == 'object') {
          points[i] = String(points[i]);
        }
        points[i] = points[i].split(",");
        path_points[i] = new google.maps.LatLng(points[i][1], points[i][0]);
      } else {
        path_points[i] = new google.maps.LatLng(points[i]['lat'], points[i]['long']);
      }
      this.GM3.add_latlng(path_points[i]);
    }
    if(editable) {
      // We don't add a popup to an editable polygon.
      this.polygons[this.polygons.length] = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: path_points});
    } else {
      // Add the popup also if we have content!
      content = typeof (content) != 'undefined' ? content : '';
      title = typeof (title) != 'undefined' ? title : '';
      var polygon = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: '#000000', strokeOpacity: 0.4, strokeWeight: 1, path: path_points});
      this.GM3.add_listeners_helper(polygon);
      if(content) {
        this.GM3.add_popup(polygon, content, title);
      }
      // Return the polygon so that it can be saved elsewhere.
      return polygon;
    }
  }
  Drupal.GM3.polygon.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'polygon':
        switch(event_type){
          case 'click':
            if(this.polygons[this.polygons.length - 1].getPath().length == 0) {
              if(this.GM3.max_objects == "-1" || this.GM3.num_objects < this.GM3.max_objects) {
                this.GM3.num_objects++;
              } else {
                this.GM3.message(Drupal.t('Please delete an object from the map before adding another'), 'warning');
                break;
              }
            }
            this.polygons[this.polygons.length - 1].stopEdit();
            this.polygons[this.polygons.length - 1].getPath().push(event.latLng);
            this.polygons[this.polygons.length - 1].runEdit(true);
            if(this.update_field) {
              this.update_field();
            }
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
            if(this.update_field) {
              this.update_field();
            }
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
              // We need to check this object is one of ours. Else we simply
              // ignore it
              for( var i = 0; i < this.polygons.length; i++) {
                if(event_object == this.polygons[i]) {
                  this.polygons[i].runEdit();
                }
              }
            } else {
              // Clicked elsewhere, stop editing.
              for( var j = 0; j < this.polygons.length; j++) {
                this.polygons[j].stopEdit();
              }
            }
            if(this.update_field) {
              this.update_field();
            }
            break;
          case 'rightclick':
            if(event_object.getClass && event_object.getClass() != 'Polygon') {
              // Once clicked, stop editing other polygons
              for( var j = 0; j < this.polygons.length; j++) {
                this.polygons[j].stopEdit();
              }
            }
            if(this.update_field) {
              this.update_field();
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