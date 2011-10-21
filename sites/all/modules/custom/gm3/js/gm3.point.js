(function($){
  Drupal.GM3.point = function(map){
    // Point object.
    this.GM3 = map;
    this.points = new Array();
    this.markers = new Array();
    // Icon
    // FIXME - Add a way of setting this image.
    this.marker_image = new google.maps.MarkerImage(Drupal.settings.gm3.settings.images.sprite, new google.maps.Size(16, 16), new google.maps.Point(0, 44), new google.maps.Point(8, 8));
    // Add points sent from server.
    if(this.GM3.libraries.point.points) {
      for( var i in this.GM3.libraries.point.points) {
        this.add_marker(new google.maps.LatLng(this.GM3.libraries.point.points[i]['lat'], this.GM3.libraries.point.points[i]['long']), false, this.GM3.libraries.point.points[i]['title'], this.GM3.libraries.point.points[i]['content']);
      }
    }
    // Clusterer
    this.clusterer = new MarkerClusterer(this.GM3.google_map, this.points, {averageCenter: true, maxZoom: 12, minimumClusterSize: 5});
    this.autofit = typeof (this.GM3.libraries.point.autofit) != 'undefined' ? this.GM3.libraries.point.autofit : false;
    if(this.autofit) {
      this.clusterer.fitMapToMarkers();
    }
  }
  Drupal.GM3.point.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
  }
  Drupal.GM3.point.prototype.add_marker = function(latLng, redraw, title, content){
    redraw = typeof (redraw) != 'undefined' ? redraw : false;
    title = typeof (title) != 'undefined' ? title : '';
    content = typeof (content) != 'undefined' ? content : '';
    var current_point = this.points.length;
    this.points[current_point] = new google.maps.Marker({position: latLng, draggable: true, title: title + " :: " + latLng.toString(), icon: this.marker_image});
    if(content) {
      google.maps.event.addListener(this.points[current_point], "click", function(event){
        var info_window = new InfoBubble({map: this.map, content: content, position: latLng, shadowStyle: 1, padding: 0, borderRadius: 4, arrowSize: 10, borderWidth: 1, borderColor: '#2c2c2c', disableAutoPan: true, hideCloseButton: true, arrowPosition: 30, backgroundClassName: 'phoney', arrowStyle: 2});
        info_window.open();
      });
    }
    if(redraw) {
      this.clusterer.addMarker(this.points[current_point], true);
      this.clusterer.repaint();
    }
    // Add transfer listeners so the added points can be rightclicked.
    this.GM3.add_listeners_helper(this.points[current_point]);
  }
  Drupal.GM3.point.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'point':
        switch(event_type){
          case 'click':
            this.add_marker(event.latLng, true);
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
                break;
            }
            break;
        }
        break;
    }
  }
  Drupal.GM3.point.prototype.add_transfer_listeners = function(){
    // We should perhaps just add transfer listeners so that the "map" is
    // clicked on. This could inturn send an ajax request to the server to find
    // out what point was clicked on. Clunky, but may well scale.
    for( var i = 0; i < this.points.length; i++) {
      if(this.points[i]) {
        this.GM3.add_listeners_helper(this.points[i]);
      }
    }
  }
})(jQuery);