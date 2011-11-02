(function($){
  Drupal.GM3.field_region = function(map){
    // All we need to do is alter the point "event" function
    map.children.region.event = function(event_type, event, event_object){
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
                          self.add_polygons_by_id(data);
                          self.update_field();
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
  }
  Drupal.GM3.region.prototype.update_field = function(){
    // Loop through each country.
    var new_values = new Array();
    for( var i in this.countries) {
      if(this.countries[i] != undefined) {
        new_values[new_values.length] = i;
      }
    }
    $('.' + this.GM3.id + '-region').val(new_values);
  }
})(jQuery);