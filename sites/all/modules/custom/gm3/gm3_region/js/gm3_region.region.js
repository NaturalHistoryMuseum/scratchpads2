(function($){
  Drupal.GM3.region = function(map){
    // Point object.
    this.GM3 = map;
    this.geo = new google.maps.Geocoder();
    this.countries = new Object();
    // Add Regions sent from server.
    if(this.GM3.libraries.region.regions) {
      for( var i in this.GM3.libraries.region.regions) {
        this.add_polygons_by_id(this.GM3.libraries.region.regions[i]);
      }
    }
  }
  Drupal.GM3.region.prototype.add_polygons_by_id = function(region_id){
    // Execute the callback to get the Polygon. This Polygon should then
    // be added to the map, but without it being editable.
    if(this.countries[region_id] == undefined){
      this.countries[region_id] = new Array();
      var self = this;
      $.ajax({url: Drupal.settings.gm3_region.callback + '/' + region_id, success: function(data, textStatus, jqXHR){
        var polygons = eval(data);
        for( var j in polygons) {
          self.countries[region_id][self.countries[region_id].length] = self.GM3.children.polygon.add_polygon(polygons[j], false);
        }
      }})
    } else {
      this.remove_polygons_by_id(region_id);
    }
  }
  Drupal.GM3.region.prototype.remove_polygons_by_id = function(region_id){
    for(var i in this.countries[region_id]){
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
                      if(data){
                        self.add_polygons_by_id(data);
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