(function($){
  Drupal.behaviors.gm3 = {attach: function(context, settings){
    for(map_index in Drupal.settings.gm3.maps) {
      if($('#' + Drupal.settings.gm3.maps[map_index]['id'], context).length) {
        try {
          Drupal.settings.gm3.maps[map_index]['settings']['center'] = new google.maps.LatLng(Drupal.settings.gm3.maps[map_index]['settings']['center']['latitude'], Drupal.settings.gm3.maps[map_index]['settings']['center']['longitude']);
          Drupal.settings.gm3.maps[map_index]['settings']['mapTypeId'] = google.maps.MapTypeId[Drupal.settings.gm3.maps[map_index]['settings']['mapTypeId']];
          console.log(Drupal.settings.gm3.maps[map_index]);
          //Drupal.settings.gm3.maps[map_index]['gm3_options'] = {zoom: Drupal.settings.gm3.maps[map_index]['settings']['zoom'], center: Drupal.settings.gm3.maps[map_index]['gm3_latlong'], mapTypeId: google.maps.MapTypeId[Drupal.settings.gm3.maps[map_index]['settings']['mapTypeId']]}
          //Drupal.settings.gm3.maps[map_index]['gm3_options'] = Drupal.settings.gm3.maps[map_index]['settings'];
          $('#' + Drupal.settings.gm3.maps[map_index]['id'], context).height(Drupal.settings.gm3.maps[map_index]['settings']['height']);
          $('#' + Drupal.settings.gm3.maps[map_index]['id'], context).width(Drupal.settings.gm3.maps[map_index]['settings']['width']);
          Drupal.settings.gm3.maps[map_index]['gm3_map'] = new google.maps.Map(document.getElementById(Drupal.settings.gm3.maps[map_index]['id']), Drupal.settings.gm3.maps[map_index]['settings']);
        } catch(err) {
          console.log(err);
          //alert('There has been an error with your map.  Please contact an administrator');
        }
      }
    }
  }};
})(jQuery);