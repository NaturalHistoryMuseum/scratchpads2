(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.behaviors.gm3_polygon = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      var map = Drupal.settings.gm3.maps[map_id]['google_map'];
      mapPolygon = new google.maps.Polygon({
        map: map,
        strokeColor: '#ff0000',
        strokeOpacity: 0.6,
        strokeWeight: 4,
        path: [new google.maps.LatLng(50.91607609098315, 34.80485954492187), new google.maps.LatLng(50.91753710953153, 34.80485954492187), new google.maps.LatLng(50.91759122044873, 34.815159227539056), new google.maps.LatLng(50.9159678655622, 34.815159227539056), new google.maps.LatLng(50.91044803534999, 34.81258430688476), new google.maps.LatLng(50.91044803534999, 34.81584587304687), new google.maps.LatLng(50.90931151845126, 34.81533088891601), new google.maps.LatLng(50.90931151845126, 34.811897661376946), new google.maps.LatLng(50.90395327929007, 34.8094944020996), new google.maps.LatLng(50.9040074060014, 34.80700531213378), new google.maps.LatLng(50.90914915662899, 34.809666063476556), new google.maps.LatLng(50.90920327729935, 34.8065761586914),
            new google.maps.LatLng(50.91033979684091, 34.80700531213378), new google.maps.LatLng(50.910285677492006, 34.81035270898437), new google.maps.LatLng(50.91607609098315, 34.81301346032714)]});
      mapPolygon.runEdit(true);
      google.maps.event.addListener(mapPolygon, 'click', Drupal.gm3.polygon_click);
    }
  }};
})(jQuery);

Drupal.gm3.polygon_add_click = function(){

}

Drupal.gm3.polygon_click = function(){
  document.getElementById("info").innerHTML = 'path:[';
  mapPolygon.getPath().forEach(function(vertex, inex){
    document.getElementById("info").innerHTML += 'new google.maps.LatLng(' + vertex.lat() + ',' + vertex.lng() + ')' + ((inex < mapPolygon.getPath().getLength() - 1) ? ',' : '');
  });
  document.getElementById("info").innerHTML += ']';
}