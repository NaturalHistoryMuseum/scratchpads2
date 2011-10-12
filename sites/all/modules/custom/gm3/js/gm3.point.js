(function($){
  Drupal.GM3.points = function(map){
    // Point object.
    this.points = new Array();
    this.markers = new Array();
    this.info_windows = new Array();
    this.google_map = map.google_map;
    // Icon
    this.marker_image = new google.maps.MarkerImage(Drupal.settings.gm3.settings.images.sprite, new google.maps.Size(16, 16), new google.maps.Point(0, 44), new google.maps.Point(8, 8));
    // Add points sent from server.
    if(map.libraries.point.points) {
      for(i in map.libraries.point.points) {
        this.add_marker(new google.maps.LatLng(map.libraries.point.points[i]['lat'], map.libraries.point.points[i]['long']), false, map.libraries.point.points[i]['title'], map.libraries.point.points[i]['content']);
      }
    }
    // Clusterer
    this.clusterer = new MarkerClusterer(this.google_map, this.points, {averageCenter: true, maxZoom: 12, minimumClusterSize: 5});
    this.clusterer.fitMapToMarkers();
    // Clicked to start.
    $('#' + map_id + "-point").click(function(){
      $('.gm3-clicked').removeClass('gm3-clicked');
      $(this).parent().addClass('gm3-clicked');
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'crosshair'});
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], "click", function(event){
        Drupal.gm3.point.add_marker(map_id, event.latLng, true)
      });
    });
    // Finally, process any of the registered "Finished points" functions.
    if(Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished']) {
      for(i in Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished']) {
        Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished'][i](map_id);
      }
    }
  }
  Drupal.GM3.prototype.set_marker_image = function(marker_image){
    this.marker_image = marker_image;
  }
  Drupal.GM3.points.prototype.add_marker = function(latLng, redraw, title, content){
    redraw = typeof (redraw) != 'undefined' ? redraw : false;
    title = typeof (title) != 'undefined' ? title : '';
    content = typeof (content) != 'undefined' ? content : '';
    var current_point = this.points.length;
    this.points[current_point] = new google.maps.Marker({position: latLng, draggable: true, title: title + " :: " + latLng.toString(), icon: this.marker_image});
    if(content) {
      //Drupal.settings.gm3.maps[map_id]['point']['markers'][latLng.toString()] = content;
      google.maps.event.addListener(this.points[current_point], "click", function(event){
        Drupal.settings.gm3.maps[map_id]['point']['info_windows'][latLng.toString()] = new InfoBubble({map: this.map, content: content, position: latLng, shadowStyle: 1, padding: 0, borderRadius: 4, arrowSize: 10, borderWidth: 1, borderColor: '#2c2c2c', disableAutoPan: true, hideCloseButton: true, arrowPosition: 30, backgroundClassName: 'phoney', arrowStyle: 2});
        Drupal.settings.gm3.maps[map_id]['point']['info_windows'][latLng.toString()].open();
      });
    }
    if(redraw) {
      Drupal.settings.gm3.maps[map_id]['point']['clusterer'].addMarker(Drupal.settings.gm3.maps[map_id]['point']['points'][current_point], true);
      Drupal.settings.gm3.maps[map_id]['point']['clusterer'].repaint();
    }
  }

  Drupal.behaviors.gm3_point = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if(Drupal.settings.gm3.maps[map_id]['initialized'] && Drupal.settings.gm3.maps[map_id]['libraries']['point']) {
        var points = new Drupal.GM3.points(Drupal.settings.gm3.maps[map_id]);
      }
    }
  }};

  /* Drupal.gm3.point.clear_listeners = function(map_id){
    // Clear all listeners from this map.
  }
  Drupal.gm3.point.add_transfer_listeners = function(map_id){
    // Add transfer listeners so that polygons and other objects pass on their
  }
  Drupal.gm3.point.add_listeners = function(map_id){
    // Add listeners
  }
  Drupal.gm3.point.add_edit_listeners = function(map_id){
    // Add Edit listeners
  }*/
})(jQuery);