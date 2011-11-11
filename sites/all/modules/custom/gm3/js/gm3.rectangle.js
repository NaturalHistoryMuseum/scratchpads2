(function($){
  Drupal.GM3.rectangle = function(map){
    this.GM3 = map;
    // Rectangle object.
    // We don't currently support geodesic shapes, mainly due to the library
    // we're using being a little buggy in its support for it. For this reason,
    // please avoid loading the geometry library.
    this.geodesic = false;
    // Editing lines
    this.followlineN = new google.maps.Polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
    this.followlineE = new google.maps.Polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
    this.followlineS = new google.maps.Polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
    this.followlineW = new google.maps.Polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
    this.firstclick = false;
    // Rectanlges
    this.rectangles = new Array();
    // Add Rectangles sent from server.
    if(this.GM3.libraries.rectangle.rectangles) {
      for( var i in this.GM3.libraries.rectangle.rectangles) {
        // this.add_rectangle(this.GM3.libraries.rectangle.rectangles[i]);
      }
    }
    this.add_transfer_listeners();
  }
  Drupal.GM3.rectangle.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
    this.first_click = false;
    this.rectangles[this.rectangles.length] = new google.maps.Polygon({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: []});
    this.followlineN.setPath([]);
    this.followlineN.setMap(this.GM3.google_map);
    this.followlineE.setPath([]);
    this.followlineE.setMap(this.GM3.google_map);
    this.followlineS.setPath([]);
    this.followlineS.setMap(this.GM3.google_map);
    this.followlineW.setPath([]);
    this.followlineW.setMap(this.GM3.google_map);
  }
  Drupal.GM3.rectangle.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'rectangle':
        switch(event_type){
          case 'click':
            // Is this the first click? If so, we start a rectangle, else we
            // finish a rectangle.
            if(this.first_click) {
              console.log('Second click');
            } else {
              this.first_click = event;
            }
            break;
          case 'mousemove':
            var pathLength = this.rectangles[this.rectangles.length - 1].getPath().getLength();
            if(pathLength >= 1) {
              var startingPoint1 = this.rectangles[this.rectangles.length - 1].getPath().getAt(pathLength - 1);
              var followCoordinates1 = [startingPoint1, event.latLng];
              this.followline1.setPath(followCoordinates1);
              var startingPoint2 = this.rectangles[this.rectangles.length - 1].getPath().getAt(0);
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
              for( var j = 0; j < this.rectangles.length; j++) {
                this.rectangles[j].stopEdit();
              }
              // We need to check this object is one of ours. Else we simply
              // ignore it
              for( var i = 0; i < this.rectangles.length; i++) {
                if(event_object == this.rectangles[i]) {
                  this.rectangles[i].runEdit();
                }
              }
            } else {
              // Clicked elsewhere, stop editing.
              for( var j = 0; j < this.rectangles.length; j++) {
                this.rectangles[j].stopEdit();
              }
            }
            break;
          case 'rightclick':
            if(event_object.getClass && event_object.getClass() != 'Polygon') {
              // Once clicked, stop editing other polygons
              for( var j = 0; j < this.rectangles.length; j++) {
                this.rectangles[j].stopEdit();
              }
            }
            break;
        }
        break;
    }
  }
  Drupal.GM3.rectangle.prototype.add_transfer_listeners = function(){
    for( var i = 0; i < this.rectangles.length; i++) {
      if(this.rectangles[i]) {
        this.GM3.add_listeners_helper(this.rectangles[i]);
      }
    }
  }
  Drupal.GM3.rectangle.prototype.get_line_colour = function(){
    switch(this.rectangles.length % 8){
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