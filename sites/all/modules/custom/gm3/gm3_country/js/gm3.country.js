(function($){
  Drupal.GM3.country = function(map){
    // Point object.
    this.GM3 = map;
    this.geo = new google.maps.Geocoder();
    this.countries = new Object();
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
            this.geo.geocode({location: event.latLng}, function(result, status){
              if(status === 'OK'){
                for(i in result){
                  if(result[i].types[0] && result[i].types[0] == 'country' && result[i].types[1] && result[i].types[1] == 'political'){
                    console.log(result);
                    var country_name = result[i].address_components[0]['long_name'];
                    var country_code = result[i].address_components[0]['short_name'];
                    if(self.countries[country_code] == country_name){
                      self.countries[country_code] = undefined;
                    } else {
                      self.countries[country_code] = country_name;
                    }
                    console.log(self.countries);
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