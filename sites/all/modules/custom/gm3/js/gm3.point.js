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
    // Clicked to start.
    $('#' + map_id + "-point").click(function(){
      $('.gm3-clicked').removeClass('gm3-clicked');
      $(this).parent().addClass('gm3-clicked');
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
      google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
      Drupal.settings.gm3.maps[map_id]['google_map'].setOptions({draggableCursor: 'crosshair'});
      google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], "click", function(event){
        var current_point = Drupal.settings.gm3.maps[map_id]['point']['points'].length;
        Drupal.settings.gm3.maps[map_id]['point']['points'][current_point] = new google.maps.Marker({
          position: event.latLng,
          map: Drupal.settings.gm3.maps[map_id]['google_map']
        });        
      });
    });
  }
})(jQuery);