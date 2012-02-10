(function($){
  Drupal.GM3.field_region = function(map){
    // All we need to do is alter the point "event" function
    map.children.region.active = function(){
      // We add a little text to the top left of the map to say what level
      // we will be selecting.
      map.children.region.selecting_level = this.get_level_from_zoom(this.GM3.google_map.getZoom());
      map.children.region.clicked_on_country = false;
      $('#' + this.GM3.id).append('<div class="gm3_information" style="cursor:pointer"><p>' + this.get_message_from_level(this.selecting_level) + '</p></div>');
      var self = this;
      $('#' + this.GM3.id + ' .gm3_information').click(function(){
        // We reduce the level by one, unless we're on one, then we set it as 4
        if(self.selecting_level < 2) {
          self.selecting_level = 4;
        } else {
          self.selecting_level--;
        }
        $('#' + self.GM3.id + ' .gm3_information p').html(self.get_message_from_level(self.selecting_level));
      });
      this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
    }
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
                      $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/" + region_code + "/" + self.selecting_level, function(data){
                        if(data) {
                          self.add_polygons_by_ids(data);
                          self.update_field();
                        }
                      });
                    }
                  }
                } else if(status === 'OVER_QUERY_LIMIT') {
                  this.GM3.message(Drupal.t('Woah, slow down, Google is getting annoyed.'), 'warning');
                } else if(status === 'ZERO_RESULTS') {
                  // Could be one of the following:
                  // Kosovo, Kashmir, St Vincent and the Grenadines, Anguilla
                  // or the Sea.
                  $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/UNKNOWN/" + self.selecting_level, function(data){
                    if(data) {
                      self.add_polygons_by_ids(data);
                      self.update_field();
                    }
                  });
                } else {
                  // Likely to be an error, although given the total lack of
                  // documentation, it could well be something else.
                }
              });
              break;
            case 'zoom_changed':
              this.selecting_level = this.get_level_from_zoom(this.GM3.google_map.getZoom());
              $('#' + this.GM3.id + ' .gm3_information p').html(this.get_message_from_level(this.selecting_level));
              break;
            case 'rightclick':
              this.GM3.set_active_class('default');
              break;
          }
          break;
      }
    }
  }
  Drupal.GM3.region.prototype.get_message_from_level = function(level){
    switch(level){
      case 1:
        return "Selecting by continent (Level 1)";
      case 2:
        return "Selecting by sub-continent (Level 2)";
      case 3:
        return "Selecting by country/subcountry (Level 3)";
      case 4:
      default:
        return "Selecting by country/subcountry (Level 4)";
    }
  }
  Drupal.GM3.region.prototype.get_level_from_zoom = function(zoom){
    if(zoom < 3) {
      return 1;
    } else if(zoom < 5) {
      return 2;
    } else if(zoom < 6) {
      return 3;
    }
    return 4;
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