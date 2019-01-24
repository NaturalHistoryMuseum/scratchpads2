(function($){
  if(typeof google != 'undefined') {
    Drupal.GM3.rectangle = function(map, settings){
      this.GM3 = map;
      // Rectangle object.
      // We don't currently support geodesic shapes, mainly due to the library
      // we're using being a little buggy in its support for it. For this
      // reason,
      // please avoid loading the geometry library.
      this.geodesic = false;
      // Editing lines
      this.followlineN = new L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 0.7, strokeWeight: 2});
      this.followlineE = new L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 0.7, strokeWeight: 2});
      this.followlineS = new L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 0.7, strokeWeight: 2});
      this.followlineW = new L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 0.7, strokeWeight: 2});
      this.firstclick = false;
      // Rectanlges
      this.rectangles = new Array();
      // Add Rectangles sent from server.
      if(settings.rectangles) {
        for(const rectangle of settings.rectangles) {
          if(!rectangle.rectangle) {
            this.rectangles.push(this.GM3.children.polygon.add_polygon(rectangle, false));
          } else {
            const content = rectangle.content || '';
            const title = rectangle.title || '';
            this.GM3.children.polygon.add_polygon(rectangle.rectangle, rectangle.editable, content, title);
          }
        }
      }
      this.add_transfer_listeners();
    }
    Drupal.GM3.rectangle.prototype.active = function(){
      this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
      this.first_click = false;
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
      switch(this.GM3.activeClass){
        case 'rectangle':
          switch(event_type){
            case 'click':
              if(this.GM3.max_objects == "-1" || this.GM3.num_objects < this.GM3.max_objects) {
                // Is this the first click? If so, we start a rectangle, else we
                // finish a rectangle.
                if(this.first_click) {
                  // We have a second click, we add the rectangle, and clear the
                  // first
                  // click.
                  var points = new Array({'lat': this.first_click.latLng.lat(), 'long': this.first_click.latLng.lng()}, {'lat': this.first_click.latLng.lat(), 'long': event.latLng.lng()}, {'lat': event.latLng.lat(), 'long': event.latLng.lng()}, {'lat': event.latLng.lat(), 'long': this.first_click.latLng.lng()});
                  this.rectangles[this.rectangles.length] = this.GM3.children.polygon.add_polygon(points, false);
                  this.GM3.num_objects++;
                  this.GM3.set_active_class('default');
                  this.followlineN.setMap(null);
                  this.followlineE.setMap(null);
                  this.followlineS.setMap(null);
                  this.followlineW.setMap(null);
                  if(this.update_field) {
                    this.update_field();
                  }
                } else {
                  this.first_click = event;
                }
              } else {
                this.GM3.message(Drupal.t('Please delete an object from the map before adding another'), 'warning');
              }
              break;
            case 'mousemove':
              if(this.first_click) {
                var neLatLng = new google.maps.LatLng(this.first_click.latLng.lat(), event.latLng.lng());
                var swLatLng = new google.maps.LatLng(event.latLng.lat(), this.first_click.latLng.lng());
                this.followlineN.setPath([this.first_click.latLng, neLatLng]);
                this.followlineE.setPath([neLatLng, event.latLng]);
                this.followlineS.setPath([event.latLng, swLatLng]);
                this.followlineW.setPath([swLatLng, this.first_click.latLng]);
              }
              break;
            case 'rightclick':
              if(event_object.getClass() == 'Polygon') {
                for( var i = 0; i < this.rectangles.length; i++) {
                  if(this.rectangles[i] == event_object) {
                    this.rectangles[i].setMap(null);
                    this.rectangles[i] = null;
                  }
                }
              }
              // Close up the array
              var new_rectangles = new Array();
              var j = 0;
              for( var i = 0; i < this.rectangles.length; i++) {
                if(this.rectangles[i] != undefined) {
                  new_rectangles[j] = this.rectangles[i];
                  j++;
                }
              }
              this.rectangles = new_rectangles;
              if(this.update_field) {
                this.update_field();
              }
              this.GM3.set_active_class('default');
              this.followlineN.setMap(null);
              this.followlineE.setMap(null);
              this.followlineS.setMap(null);
              this.followlineW.setMap(null);
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
  }
})(jQuery);
