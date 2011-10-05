(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.gm3.polygon = Drupal.gm3.polygon || {};
  Drupal.behaviors.gm3_polygon = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps){
      if(Drupal.settings.gm3.maps[map_id]['polygon']){        
        Drupal.settings.gm3.maps[map_id]['polygon']['markers'] = [];
        Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'] = [];
        google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], "click", Drupal.gm3.polygon.mapLeftClick);
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'] = new google.maps.Polygon({strokeColor: "#3355FF", strokeOpacity: 0.8, strokeWeight: 4});
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
        Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'] = new google.maps.Polygon({strokeColor: "#3355FF", strokeOpacity: 0.4, strokeWeight: 4});
        Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      }
    }
  }};
})(jQuery);

Drupal.gm3.polygon.mapLeftClick = function(event){
  if(event.latLng) {
    var marker = Drupal.gm3.polygon.createMarker(event.latLng, this.A.L.id);
    Drupal.settings.gm3.maps[this.A.L.id]['polygon']['markers'].push(marker);
    if(Drupal.settings.gm3.maps[this.A.L.id]['polygon']['markers'].length != 1) {
      var vmarker = Drupal.gm3.polygon.createVMarker(event.latLng, this.A.L.id);
      Drupal.settings.gm3.maps[this.A.L.id]['polygon']['vmarkers'].push(vmarker)
      vmarker = null;
    }
    var path = Drupal.settings.gm3.maps[this.A.L.id]['polygon']['polygon'].getPath();
    path.push(event.latLng);
    marker = null;
  }
  event = null;
};

Drupal.gm3.polygon.createMarker = function(point, map_id){
  var imageNormal = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polygon']['imgs']['square'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var imageHover = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polygon']['imgs']['square_over'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var marker = new google.maps.Marker({position: point, map: Drupal.settings.gm3.maps[map_id]['google_map'], icon: imageNormal, draggable: true});
  google.maps.event.addListener(marker, "mouseover", function(){
    marker.setIcon(imageHover);
  });
  google.maps.event.addListener(marker, "mouseout", function(){
    marker.setIcon(imageNormal);
  });
  google.maps.event.addListener(marker, "drag", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polygon']['markers'][m] == marker) {
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().setAt(m, marker.getPosition());
        Drupal.gm3.polygon.moveVMarker(m, map_id);
        break;
      }
    }
    m = null;
  });
  google.maps.event.addListener(marker, "click", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polygon']['markers'][m] == marker) {
        marker.setMap(null);
        Drupal.settings.gm3.maps[map_id]['polygon']['markers'].splice(m, 1);
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().removeAt(m);
        Drupal.gm3.polygon.removeVMarkers(m, map_id);
        break;
      }
    }
    m = null;
  });
  return marker;
};

Drupal.gm3.polygon.createVMarker = function(point, map_id){
  var prevpoint = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length - 2].getPosition();
  var imageNormal = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polygon']['imgs']['square_transparent'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var imageHover = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polygon']['imgs']['square_transparent_over'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var marker = new google.maps.Marker({position: new google.maps.LatLng(point.lat() - (0.5 * (point.lat() - prevpoint.lat())), point.lng() - (0.5 * (point.lng() - prevpoint.lng()))), map: Drupal.settings.gm3.maps[map_id]['google_map'], icon: imageNormal, draggable: true});
  google.maps.event.addListener(marker, "mouseover", function(){
    marker.setIcon(imageHover);
  });
  google.maps.event.addListener(marker, "mouseout", function(){
    marker.setIcon(imageNormal);
  });
  google.maps.event.addListener(marker, "dragstart", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][m] == marker) {
        var tmpPath = Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'].getPath();
        tmpPath.push(Drupal.settings.gm3.maps[map_id]['polygon']['markers'][m].getPosition());
        tmpPath.push(Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][m].getPosition());
        tmpPath.push(Drupal.settings.gm3.maps[map_id]['polygon']['markers'][m + 1].getPosition());
        break;
      }
    }
    m = null;
  });
  google.maps.event.addListener(marker, "drag", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][m] == marker) {
        Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'].getPath().setAt(1, marker.getPosition());
        break;
      }
    }
    m = null;
  });
  google.maps.event.addListener(marker, "dragend", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][m] == marker) {
        var newpos = marker.getPosition();
        var startMarkerPos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][m].getPosition();
        var firstVPos = new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - startMarkerPos.lat())), newpos.lng() - (0.5 * (newpos.lng() - startMarkerPos.lng())));
        var endMarkerPos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][m + 1].getPosition();
        var secondVPos = new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - endMarkerPos.lat())), newpos.lng() - (0.5 * (newpos.lng() - endMarkerPos.lng())));
        var newVMarker = Drupal.gm3.polygon.createVMarker(secondVPos, map_id);
        newVMarker.setPosition(secondVPos);// apply the correct position to the
                                            // vmarker
        var newMarker = Drupal.gm3.polygon.createMarker(newpos, map_id);
        Drupal.settings.gm3.maps[map_id]['polygon']['markers'].splice(m + 1, 0, newMarker);
        Drupal.settings.gm3.maps[map_id]['polygon']['polygon'].getPath().insertAt(m + 1, newpos);
        marker.setPosition(firstVPos);
        Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'].splice(m + 1, 0, newVMarker);
        Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'].getPath().removeAt(2);
        Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'].getPath().removeAt(1);
        Drupal.settings.gm3.maps[map_id]['polygon']['tmp_polygon'].getPath().removeAt(0);
        newpos = null;
        startMarkerPos = null;
        firstVPos = null;
        endMarkerPos = null;
        secondVPos = null;
        newVMarker = null;
        newMarker = null;
        break;
      }
    }
  });
  return marker;
};

Drupal.gm3.polygon.moveVMarker = function(index, map_id){
  var newpos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][index].getPosition();
  if(index != 0) {
    var prevpos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][index - 1].getPosition();
    Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][index - 1].setPosition(new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - prevpos.lat())), newpos.lng() - (0.5 * (newpos.lng() - prevpos.lng()))));
    prevpos = null;
  }
  if(index != Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length - 1) {
    var nextpos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][index + 1].getPosition();
    Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][index].setPosition(new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - nextpos.lat())), newpos.lng() - (0.5 * (newpos.lng() - nextpos.lng()))));
    nextpos = null;
  }
  newpos = null;
  index = null;
};

Drupal.gm3.polygon.removeVMarkers = function(index, map_id){
  if(Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length > 0) {// clicked marker has already been deleted
    if(index != Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length) {
      Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][index].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'].splice(index, 1);
    } else {
      Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'][index - 1].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polygon']['vmarkers'].splice(index - 1, 1);
    }
  }
  if(index != 0 && index != Drupal.settings.gm3.maps[map_id]['polygon']['markers'].length) {
    var prevpos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][index - 1].getPosition();
    var newpos = Drupal.settings.gm3.maps[map_id]['polygon']['markers'][index].getPosition();
    Drupal.settings.gm3.maps[map_id]['polygon']['markers'][index - 1].setPosition(new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - prevpos.lat())), newpos.lng() - (0.5 * (newpos.lng() - prevpos.lng()))));
    prevpos = null;
    newpos = null;
  }
  index = null;
};