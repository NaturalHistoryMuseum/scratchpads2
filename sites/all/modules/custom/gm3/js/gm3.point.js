(function($){
  Drupal.GM3.point = function(map){
    // Point object.
    this.GM3 = map;
    this.points = new Array();
    this.markers = new Array();
    // FIXME - Add a way of setting this image.
    this.marker_images = new Array();
    for( var i = 0; i < 8; i++) {
      this.marker_images[i] = new google.maps.MarkerImage(Drupal.settings.gm3.settings.images.sprite, new google.maps.Size(18, 25), new google.maps.Point(11 + (i * 18), 0), new google.maps.Point(9, 25));
    }
    // Add points sent from server.
    if(this.GM3.libraries.point.points) {
      for( var i in this.GM3.libraries.point.points) {
        // Default editable to false
        var editable = typeof (this.GM3.libraries.point.points[i]['editable']) != 'undefined' ? this.GM3.libraries.point.points[i]['editable'] : false;
        this.add_marker(new google.maps.LatLng(this.GM3.libraries.point.points[i]['latitude'], this.GM3.libraries.point.points[i]['longitude']), editable, false, this.GM3.libraries.point.points[i]['colour'], this.GM3.libraries.point.points[i]['title'], this.GM3.libraries.point.points[i]['content']);
      }
    }
    // Clusterer
    this.clusterer = new MarkerClusterer(this.GM3.google_map, this.points, {averageCenter: true, maxZoom: 12, minimumClusterSize: 5});
  }
  Drupal.GM3.point.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
  }
  Drupal.GM3.point.prototype.add_marker = function(latLng, editable, redraw, colour, title, content){
    if(this.GM3.num_objects < this.GM3.max_objects) {
      this.GM3.add_latlng(latLng);
      redraw = typeof (redraw) != 'undefined' ? redraw : false;
      title = typeof (title) != 'undefined' ? title : '';
      content = typeof (content) != 'undefined' ? content : '';
      var current_point = this.points.length;
      if(typeof (colour) == 'undefined') {
        colour = this.points.length % 8;
      }
      this.points[current_point] = new google.maps.Marker({position: latLng, draggable: editable, title: title + " :: " + latLng.toString(), icon: this.marker_images[colour]});
      // Add transfer listeners so the added points can be rightclicked.
      this.GM3.add_listeners_helper(this.points[current_point]);
      if(content) {
        this.GM3.add_popup(this.points[current_point], content, title);
      }
      if(redraw) {
        this.clusterer.addMarker(this.points[current_point], true);
        this.clusterer.repaint();
      }
      this.GM3.num_objects++;
    } else {
      alert(Drupal.t('Please delete an object from the map before adding another'));
    }
  }
  Drupal.GM3.point.prototype.event = function(event_type, event, event_object){
    if(event_type == 'zoom_changed' || event_type == 'bounds_changed') {
      // Looks like we're stealing a listener from the clusterer. We'll do
      // things
      // ourselves.
      this.clusterer.repaint();
    }
    switch(this.GM3.active_class){
      case 'point':
        switch(event_type){
          case 'click':
            this.add_marker(event.latLng, true, true);
            if(this.update_field) {
              this.update_field();
            }
            break;
          case 'rightclick':
            switch(event_object.getClass()){
              case 'Map':
                this.GM3.set_active_class('default');
                break;
              case 'Marker':
                // Loop through this objects points, and unset the one(s) that
                // equal this object.
                for( var i = 0; i < this.points.length; i++) {
                  if(this.points[i].position.equals(event_object.position)) {
                    this.clusterer.removeMarker(this.points[i], true);
                    this.points[i].setMap(null);
                    this.points[i] = undefined;
                    this.GM3.num_objects--;
                  }
                }
                // Finally, close up the array, which seems pretty clunky, but
                // perhaps the only way of doing this.
                var new_points = new Array();
                var j = 0;
                for( var i = 0; i < this.points.length; i++) {
                  if(this.points[i] != undefined) {
                    new_points[j] = this.points[i];
                    j++;
                  }
                }
                this.points = new_points;
                if(this.update_field) {
                  this.update_field();
                }
                break;
            }
            break;
        }
        break;
    }
  }
})(jQuery);