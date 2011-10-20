(function($){
  Drupal.GM3.country = function(map){
    // Point object.
    this.GM3 = map;
    this.geo = new google.maps.Geocoder();
    // Polygons.
    this.countries = new Array();
    // Add Polygons sent from server.
    this.countries = typeof (this.GM3.libraries.country.countries) != 'undefined' ? this.GM3.libraries.country.countries : new Array();
    console.log(this.countries);
    var self=this;
    this.country_options = {
      getTileUrl: function(coord, zoom) {
        var tiles_accross = Math.pow(2, zoom);
        if(zoom == 0){
          tiles_accross = 0;
        }
        if(coord.x < 0 || coord.x > tiles_accross || coord.y < 0 || coord.y > tiles_accross){
          return null;
        }
        return "http://157.140.127.175/tiles/example/" + zoom + "/" + coord.x + "/" + coord.y + ".png";
      },
      tileSize: new google.maps.Size(256, 256)
    };
    this.countryMapOverlay = new google.maps.ImageMapType(this.country_options);
    this.GM3.google_map.overlayMapTypes.insertAt(0, this.countryMapOverlay);
  }
  Drupal.GM3.country.prototype.get_query_string = function(){
    var query_string = '';
    for(i in this.countries){
      if(query_string != ''){
        query_string += ',';
      }
      query_string += this.countries[i];
    }
    return query_string;
  }
  Drupal.GM3.country.prototype.active = function(){
    this.GM3.google_map.setOptions({draggableCursor: 'crosshair'});
  }
  Drupal.GM3.country.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'country':
        switch(event_type){
          case 'click':
            var self=this;
            console.log(event.latLng);
            this.geo.geocode({location: event.latLng}, function(result, status){
              if(status === 'OK'){
                for(i in result){
                  if(result[i].types[0] && result[i].types[0] == 'country' && result[i].types[1] && result[i].types[1] == 'political'){
                    var country_name = result[i].address_components[0]['long_name'];
                    var country_code = result[i].address_components[0]['short_name'];
                    if(self.countries[country_code] == country_name){
                      self.countries[country_code] = undefined;
                    } else {
                      self.countries[country_code] = country_name;
                    }
                  } 
                }
              } else {
                alert(Drupal.t('There has been an error with Google\'s service.  Please try again later.'));
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
  Drupal.GM3.country.prototype.highlight_country = function(){
    
  }
})(jQuery);