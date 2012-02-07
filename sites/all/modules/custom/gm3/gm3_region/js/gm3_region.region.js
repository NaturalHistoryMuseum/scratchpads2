(function($){
  Drupal.GM3.region = function(map){
    // Point object.
    this.GM3 = map;
    this.geo = new google.maps.Geocoder();
    this.countries = new Object();
    // Add Regions sent from server.
    if(this.GM3.libraries.region.regions) {
      this.add_polygons_by_ids(this.GM3.libraries.region.regions, false, false, true);
    }
  }
  // FIXME - Add content from the server and on the server.
  Drupal.GM3.region.prototype.add_polygons_by_ids = function(region_ids, title, content, autofit){
    if(typeof region_ids != 'object') {
      if(typeof region_ids == 'string') {
        region_ids = new Array(region_ids);
      } else {
        // Error, we can't handle this data type.
        return;
      }
    }
    // Execute the callback to get the Polygon. This Polygon should then
    // be added to the map, but without it being editable.
    var region_ids_to_add = new Array();
    for( var i in region_ids) {
      if(this.countries[region_ids[i]] == undefined) {
        this.countries[region_ids[i]] = new Array();
        region_ids_to_add[region_ids_to_add.length] = region_ids[i];
      } else {
        this.remove_polygons_by_id(region_ids[i]);
      }
    }
    // Add the ones we need to add.
    if(region_ids_to_add.length) {
      // We need to do this x regions at a time, else the server will complain
      // that the URL is too long
      if(region_ids_to_add.length > 10) {
        var region_ids_copy = region_ids_to_add;
        var region_ids = new Array();
        var region_ids_index = -1;
        for( var i in region_ids_copy) {
          if(i % 10 == 0) {
            region_ids_index++;
            region_ids[region_ids_index] = new Array();
          }
          region_ids[region_ids_index][region_ids[region_ids_index].length] = region_ids_copy[i];
        }
      } else {
        var region_ids = new Array(region_ids);
      }
      var self = this;
      for( var i in region_ids) {
        $.getJSON(Drupal.settings.gm3_region.callback + '/' + region_ids[i].join(','), function(data, textStatus, jqXHR){
          for( var i in data) {
            // "i" is the index of the region returned (0 if we asked for only
            // one).
            for( var j in data[i]) {
              // "j" becomes the ID of the region
              for( var k in data[i][j]['shape']['coordinates']) {
                if(data[i][j]['shape']['type'] == 'MultiPolygon') {
                  for( var l in data[i][j]['shape']['coordinates'][k]) {
                    // We have a region with multiple shapes.
                    self.countries[j][self.countries[j].length] = self.GM3.children.polygon.add_polygon(data[i][j]['shape']['coordinates'][k][l], false);
                  }
                } else if(data[i][j]['shape']['type'] == 'Polygon') {
                  self.countries[j][self.countries[j].length] = self.GM3.children.polygon.add_polygon(data[i][j]['shape']['coordinates'][k], false);
                }
              }
            }
          }
          if(typeof autofit != 'undefined' && autofit) {// Change this to be an autozoom option
            if(self.GM3.max_lat) {
              self.GM3.google_map.fitBounds(new google.maps.LatLngBounds(new google.maps.LatLng(self.GM3.min_lat, self.GM3.min_lng), new google.maps.LatLng(self.GM3.max_lat, self.GM3.max_lng)));
            }
          }
        })
      }
    }
  }
  Drupal.GM3.region.prototype.remove_polygons_by_id = function(region_id){
    for( var i in this.countries[region_id]) {
      this.countries[region_id][i].setMap(null);
    }
    this.countries[region_id] = undefined;
  }
  Drupal.GM3.region.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
  }
  Drupal.GM3.region.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'region':
        switch(event_type){
          case 'click':
            var self = this;
            this.geo.geocode({location: event.latLng}, function(result, status){
              if(status === 'OK') {
                for(i in result) {
                  if(result[i].types[0] && result[i].types[0] == 'country' && result[i].types[1] && result[i].types[1] == 'political') {
                    var region_code = result[i].address_components[0]['short_name'];
                    $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/" + region_code, function(data){
                      if(data) {
                        self.add_polygons_by_ids(data);
                        this.update_field();
                      }
                    });
                  }
                }
              }
            });
            break;
          case 'rightclick':
            this.GM3.set_active_class('default');
            break;
        }
        break;
    }
  }
})(jQuery);