(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.behaviors.gm3_polygon = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps){
      if(Drupal.settings.gm3.maps[map_id]['polyline']){
        
        var map = null;
        var polyLine;
        var tmpPolyLine;
        var markers = [];
        var vmarkers = [];
        
        console.log(Drupal.settings.gm3.maps[map_id]);
        
        Drupal.settings.gm3.maps[map_id]['polyline']['markers'] = [];
        Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'] = [];
        google.maps.event.addListener(Drupal.settings.gm3.maps[map_id]['google_map'], "click", Drupal.gm3.mapLeftClick);
        Drupal.settings.gm3.maps[map_id]['polyline']['polyline'] = new google.maps.Polyline({strokeColor: "#3355FF", strokeOpacity: 0.8, strokeWeight: 4});
        Drupal.settings.gm3.maps[map_id]['polyline']['polyline'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
        Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'] = new google.maps.Polyline({strokeColor: "#3355FF", strokeOpacity: 0.4, strokeWeight: 4});
        Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'].setMap(Drupal.settings.gm3.maps[map_id]['google_map']);
      }
    }
  }};
})(jQuery);

Drupal.gm3.mapLeftClick = function(event){
  if(event.latLng) {
    var marker = Drupal.gm3.createMarker(event.latLng, this.A.L.id);
    Drupal.settings.gm3.maps[this.A.L.id]['polyline']['markers'].push(marker);
    if(Drupal.settings.gm3.maps[this.A.L.id]['polyline']['markers'].length != 1) {
      var vmarker = Drupal.gm3.createVMarker(event.latLng, this.A.L.id);
      Drupal.settings.gm3.maps[this.A.L.id]['polyline']['vmarkers'].push(vmarker)
      vmarker = null;
    }
    var path = Drupal.settings.gm3.maps[this.A.L.id]['polyline']['polyline'].getPath();
    path.push(event.latLng);
    marker = null;
  }
  event = null;
};

Drupal.gm3.createMarker = function(point, map_id){
  var imageNormal = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polyline']['imgs']['square'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var imageHover = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polyline']['imgs']['square_over'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var marker = new google.maps.Marker({position: point, map: Drupal.settings.gm3.maps[map_id]['google_map'], icon: imageNormal, draggable: true});
  google.maps.event.addListener(marker, "mouseover", function(){
    marker.setIcon(imageHover);
  });
  google.maps.event.addListener(marker, "mouseout", function(){
    marker.setIcon(imageNormal);
  });
  google.maps.event.addListener(marker, "drag", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polyline']['markers'][m] == marker) {
        Drupal.settings.gm3.maps[map_id]['polyline']['polyline'].getPath().setAt(m, marker.getPosition());
        Drupal.gm3.moveVMarker(m, map_id);
        break;
      }
    }
    m = null;
  });
  google.maps.event.addListener(marker, "click", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polyline']['markers'][m] == marker) {
        marker.setMap(null);
        Drupal.settings.gm3.maps[map_id]['polyline']['markers'].splice(m, 1);
        Drupal.settings.gm3.maps[map_id]['polyline']['polyline'].getPath().removeAt(m);
        Drupal.gm3.removeVMarkers(m, map_id);
        break;
      }
    }
    m = null;
  });
  return marker;
};

Drupal.gm3.createVMarker = function(point, map_id){
  var prevpoint = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length - 2].getPosition();
  var imageNormal = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polyline']['imgs']['square_transparent'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var imageHover = new google.maps.MarkerImage(Drupal.settings.gm3.maps[map_id]['polyline']['imgs']['square_transparent_over'], new google.maps.Size(11, 11), new google.maps.Point(0, 0), new google.maps.Point(6, 6));
  var marker = new google.maps.Marker({position: new google.maps.LatLng(point.lat() - (0.5 * (point.lat() - prevpoint.lat())), point.lng() - (0.5 * (point.lng() - prevpoint.lng()))), map: Drupal.settings.gm3.maps[map_id]['google_map'], icon: imageNormal, draggable: true});
  google.maps.event.addListener(marker, "mouseover", function(){
    marker.setIcon(imageHover);
  });
  google.maps.event.addListener(marker, "mouseout", function(){
    marker.setIcon(imageNormal);
  });
  google.maps.event.addListener(marker, "dragstart", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][m] == marker) {
        var tmpPath = Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'].getPath();
        tmpPath.push(Drupal.settings.gm3.maps[map_id]['polyline']['markers'][m].getPosition());
        tmpPath.push(Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][m].getPosition());
        tmpPath.push(Drupal.settings.gm3.maps[map_id]['polyline']['markers'][m + 1].getPosition());
        break;
      }
    }
    m = null;
  });
  google.maps.event.addListener(marker, "drag", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][m] == marker) {
        Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'].getPath().setAt(1, marker.getPosition());
        break;
      }
    }
    m = null;
  });
  google.maps.event.addListener(marker, "dragend", function(){
    for( var m = 0; m < Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'].length; m++) {
      if(Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][m] == marker) {
        var newpos = marker.getPosition();
        var startMarkerPos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][m].getPosition();
        var firstVPos = new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - startMarkerPos.lat())), newpos.lng() - (0.5 * (newpos.lng() - startMarkerPos.lng())));
        var endMarkerPos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][m + 1].getPosition();
        var secondVPos = new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - endMarkerPos.lat())), newpos.lng() - (0.5 * (newpos.lng() - endMarkerPos.lng())));
        var newVMarker = Drupal.gm3.createVMarker(secondVPos, map_id);
        newVMarker.setPosition(secondVPos);// apply the correct position to the
                                            // vmarker
        var newMarker = Drupal.gm3.createMarker(newpos, map_id);
        Drupal.settings.gm3.maps[map_id]['polyline']['markers'].splice(m + 1, 0, newMarker);
        Drupal.settings.gm3.maps[map_id]['polyline']['polyline'].getPath().insertAt(m + 1, newpos);
        marker.setPosition(firstVPos);
        Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'].splice(m + 1, 0, newVMarker);
        Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'].getPath().removeAt(2);
        Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'].getPath().removeAt(1);
        Drupal.settings.gm3.maps[map_id]['polyline']['tmp_polyline'].getPath().removeAt(0);
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

Drupal.gm3.moveVMarker = function(index, map_id){
  var newpos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][index].getPosition();
  if(index != 0) {
    var prevpos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][index - 1].getPosition();
    Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][index - 1].setPosition(new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - prevpos.lat())), newpos.lng() - (0.5 * (newpos.lng() - prevpos.lng()))));
    prevpos = null;
  }
  if(index != Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length - 1) {
    var nextpos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][index + 1].getPosition();
    Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][index].setPosition(new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - nextpos.lat())), newpos.lng() - (0.5 * (newpos.lng() - nextpos.lng()))));
    nextpos = null;
  }
  newpos = null;
  index = null;
};

Drupal.gm3.removeVMarkers = function(index, map_id){
  if(Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length > 0) {// clicked marker has already been deleted
    if(index != Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length) {
      Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][index].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'].splice(index, 1);
    } else {
      Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'][index - 1].setMap(null);
      Drupal.settings.gm3.maps[map_id]['polyline']['vmarkers'].splice(index - 1, 1);
    }
  }
  if(index != 0 && index != Drupal.settings.gm3.maps[map_id]['polyline']['markers'].length) {
    var prevpos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][index - 1].getPosition();
    var newpos = Drupal.settings.gm3.maps[map_id]['polyline']['markers'][index].getPosition();
    Drupal.settings.gm3.maps[map_id]['polyline']['markers'][index - 1].setPosition(new google.maps.LatLng(newpos.lat() - (0.5 * (newpos.lat() - prevpos.lat())), newpos.lng() - (0.5 * (newpos.lng() - prevpos.lng()))));
    prevpos = null;
    newpos = null;
  }
  index = null;
};