(function($){
  if(typeof google != 'undefined') {
    Drupal.GM3.polyline = function(map, settings){
      this.GM3 = map;
      // Polyline object.
      // We don't currently support geodesic shapes, mainly due to the library
      // we're using being a little buggy in its support for it. For this
      // reason,
      // please avoid loading the geometry library.
      this.geodesic = false;
      // Editing lines
      this.followline = new L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
      // Polylines.
      this.polylines = new Array();
      // Add Polylines sent from server.
      if(settings.polylines) {
        for(const polyline of settings.polylines) {
          if(!polyline.polyline) {
            this.add_polyline(polyline);
          } else {
            var content = polyline.content || '';
            this.add_polyline(polyline.polyline, polyline.editable, content);
          }
        }
      }
    }
    Drupal.GM3.polyline.prototype.active = function(){
      this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
      this.polylines[this.polylines.length] = new L.polyline({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: []});
      this.followline.setPath([]);
      this.followline.setMap(this.GM3.google_map);
    }
    Drupal.GM3.polyline.prototype.add_polyline = function(points, editable, content, title){
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
        // We don't add a popup to an editable polyline.
        this.polylines[this.polylines.length] = new L.polyline({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: path_points});
      } else {
        // Add the popup also if we have content!
        content = typeof (content) != 'undefined' ? content : '';
        title = typeof (title) != 'undefined' ? title : '';
        var polyline = new L.polyline({geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: '#000000', strokeOpacity: 0.4, strokeWeight: 1, path: path_points});
        this.GM3.add_listeners_helper(polyline);
        if(content) {
          this.GM3.add_popup(polyline, content, title);
        }
        // Return the polyline so that it can be saved elsewhere.
        return polyline;
      }
    }
    Drupal.GM3.polyline.prototype.event = function(event_type, event, event_object){
      switch(this.GM3.activeClass){
        case 'polyline':
          switch(event_type){
            case 'click':
              if(this.polylines[this.polylines.length - 1].getPath().length == 0) {
                if(this.GM3.max_objects == "-1" || this.GM3.num_objects < this.GM3.max_objects) {
                  this.GM3.num_objects++;
                } else {
                  this.GM3.message(Drupal.t('Please delete an object from the map before adding another'), 'warning');
                  break;
                }
              }
              this.polylines[this.polylines.length - 1].stopEdit();
              this.polylines[this.polylines.length - 1].getPath().push(event.latLng);
              this.polylines[this.polylines.length - 1].runEdit(true);
              if(this.update_field) {
                this.update_field();
              }
              break;
            case 'mousemove':
              var pathLength = this.polylines[this.polylines.length - 1].getPath().getLength();
              if(pathLength >= 1) {
                var startingPoint = this.polylines[this.polylines.length - 1].getPath().getAt(pathLength - 1);
                var followCoordinates = [startingPoint, event.latLng];
                this.followline.setPath(followCoordinates);
              }
              break;
            case 'rightclick':
              this.GM3.set_active_class('default');
              this.followline.setMap(null);
              if(this.update_field) {
                this.update_field();
              }
              break;
          }
          break;
        case 'default':
          switch(event_type){
            case 'click':
              if(event_object.getClass && event_object.getClass() == 'Polyline') {
                // Once clicked, stop editing other polylines
                for( var j = 0; j < this.polylines.length; j++) {
                  this.polylines[j].stopEdit();
                }
                event_object.runEdit();
              } else {
                // Clicked elsewhere, stop editing.
                for( var j = 0; j < this.polylines.length; j++) {
                  this.polylines[j].stopEdit();
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
    Drupal.GM3.polyline.prototype.add_transfer_listeners = function(){
      for( var i = 0; i < this.polylines.length; i++) {
        if(this.polylines[i]) {
          this.GM3.add_listeners_helper(this.polylines[i]);
        }
      }
    }
    Drupal.GM3.polyline.prototype.get_line_colour = function(){
      switch(this.polylines.length % 8){
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
