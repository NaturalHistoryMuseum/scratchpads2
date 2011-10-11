(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3.point = Drupal.gm3.point || {};
  Drupal.behaviors.gm3_point = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if(Drupal.settings.gm3.maps[map_id]['initialized'] && Drupal.settings.gm3.maps[map_id]['libraries']['point'] && !Drupal.settings.gm3.maps[map_id]['point']) {
        Drupal.gm3.point.initialize(map_id);
      }
    }
  }};
  Drupal.gm3.point.initialize = function(map_id){
    // Point object.
    Drupal.settings.gm3.maps[map_id]['point'] = Drupal.settings.gm3.maps[map_id]['point'] || {};
    Drupal.settings.gm3.maps[map_id]['point']['points'] = new Array();
    Drupal.settings.gm3.maps[map_id]['point']['markers'] = new Array();
    Drupal.settings.gm3.maps[map_id]['point']['info_windows'] = new Array();
    // Icon
    Drupal.settings.gm3.maps[map_id]['point']['marker_image'] = new google.maps.MarkerImage(Drupal.settings.gm3.settings.images.sprite, new google.maps.Size(16, 16), new google.maps.Point(0, 44), new google.maps.Point(8, 8));
    // Add points sent from server.
    if(Drupal.settings.gm3.maps[map_id]['libraries']['point']['points']){
      for(i in Drupal.settings.gm3.maps[map_id]['libraries']['point']['points']){
        Drupal.gm3.point.add_marker(map_id, new google.maps.LatLng(Drupal.settings.gm3.maps[map_id]['libraries']['point']['points'][i]['lat'], Drupal.settings.gm3.maps[map_id]['libraries']['point']['points'][i]['long']), false, Drupal.settings.gm3.maps[map_id]['libraries']['point']['points'][i]['title'], Drupal.settings.gm3.maps[map_id]['libraries']['point']['points'][i]['content']);
      }
    }
    // Clusterer
    Drupal.settings.gm3.maps[map_id]['point']['clusterer'] = new MarkerClusterer(Drupal.settings.gm3.maps[map_id]['google_map'], Drupal.settings.gm3.maps[map_id]['point']['points'], {
      averageCenter: true,
      maxZoom: 12,
      minimumClusterSize: 5
    });
    Drupal.settings.gm3.maps[map_id]['point']['clusterer'].fitMapToMarkers();
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
    if(Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished']){
      for(i in Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished']){
        Drupal.settings.gm3.maps[map_id]['libraries']['point']['finished'][i](map_id);
      }        
    }
  }
  Drupal.gm3.point.add_marker = function(map_id, latLng, redraw, title, content){
    redraw = typeof(redraw) != 'undefined' ? redraw : false;
    title = typeof(title) != 'undefined' ? title : '';
    content = typeof(content) != 'undefined' ? content : '';
    var current_point = Drupal.settings.gm3.maps[map_id]['point']['points'].length;
    Drupal.settings.gm3.maps[map_id]['point']['points'][current_point] = new google.maps.Marker({
      position: latLng,
      draggable: true,
      title: title + " :: "+latLng.toString(),
      icon: Drupal.settings.gm3.maps[map_id]['point']['marker_image']
    });
    if(content){
      Drupal.settings.gm3.maps[map_id]['point']['markers'][latLng.toString()] = content;
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['point']['points'][current_point], "click", function(event){
        Drupal.settings.gm3.maps[map_id]['point']['info_windows'][latLng.toString()] = new InfoBubble({
          map: this.map,
          content: content,
          position: latLng,
          shadowStyle: 1,
          padding: 0,
          borderRadius: 4,
          arrowSize: 10,
          borderWidth: 1,
          borderColor: '#2c2c2c',
          disableAutoPan: true,
          hideCloseButton: true,
          arrowPosition: 30,
          backgroundClassName: 'phoney',
          arrowStyle: 2
        });
        Drupal.settings.gm3.maps[map_id]['point']['info_windows'][latLng.toString()].open();
      });
    }
    if(redraw){
      Drupal.settings.gm3.maps[map_id]['point']['clusterer'].addMarker(Drupal.settings.gm3.maps[map_id]['point']['points'][current_point], true);
      Drupal.settings.gm3.maps[map_id]['point']['clusterer'].repaint();
    }
  }
  Drupal.gm3.point.clear_listeners = function(map_id){
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
  }
})(jQuery);