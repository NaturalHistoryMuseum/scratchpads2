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
    // Icon
    Drupal.settings.gm3.maps[map_id]['point']['marker_image'] = new google.maps.MarkerImage(Drupal.settings.gm3.settings.images.sprite, new google.maps.Size(16, 16), new google.maps.Point(0, 44), new google.maps.Point(8, 8));
    // Add points sent from server.
    if(Drupal.settings.gm3.maps[map_id]['libraries']['point']['points']){
      for(i in Drupal.settings.gm3.maps[map_id]['libraries']['point']['points']){
        Drupal.gm3.point.add_marker(map_id, new google.maps.LatLng(Drupal.settings.gm3.maps[map_id]['libraries']['point']['points'][i]['lat'], Drupal.settings.gm3.maps[map_id]['libraries']['point']['points'][i]['long']), false);
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
  Drupal.gm3.point.add_marker = function(map_id, latLng, redraw){
    redraw = typeof(redraw) != 'undefined' ? redraw : false;
    var current_point = Drupal.settings.gm3.maps[map_id]['point']['points'].length;
    Drupal.settings.gm3.maps[map_id]['point']['points'][current_point] = new google.maps.Marker({
      position: latLng,
      draggable: true,
      // animation: google.maps.Animation.DROP
      icon: Drupal.settings.gm3.maps[map_id]['point']['marker_image']
    });
    if(redraw){
      Drupal.settings.gm3.maps[map_id]['point']['clusterer'].addMarker(Drupal.settings.gm3.maps[map_id]['point']['points'][current_point], true);
      Drupal.settings.gm3.maps[map_id]['point']['clusterer'].repaint();
    }
  }
  Drupal.gm3.point.clear_listeners = function(map_id){
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
  }
  Drupal.gm3.point.add_listeners = function(map_id){
    for(i = 0; i < Drupal.settings.gm3.maps[map_id]['point']['points'][current_point].length; i++){
      
    }
  }
})(jQuery);