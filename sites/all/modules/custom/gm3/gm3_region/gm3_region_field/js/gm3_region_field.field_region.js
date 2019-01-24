(function($){
  if(typeof Drupal.GM3 != 'undefined') {
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
          // We reduce the level by one, unless we're on one, then we set it as
          // 5
          if(self.selecting_level < 2) {
            self.selecting_level = 5;
          } else {
            self.selecting_level--;
          }
          $('#' + self.GM3.id + ' .gm3_information p').html(self.get_message_from_level(self.selecting_level));
        });
        this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
      }
      map.children.region.event = function(event_type, event, event_object){
        switch(this.GM3.activeClass){
          case 'region':
            switch(event_type){
              case 'click':
                if(this.GM3.max_objects == "-1" || this.GM3.num_objects < this.GM3.max_objects) {
                  var self = this;
                  this.geo.geocode({location: event.latLng}, function(result, status){
                    if(status === 'OK') {
                      var region_code = false;
                      for(i in result) {
                        if(result[i].types[0] && result[i].types[0] == 'country' && result[i].types[1] && result[i].types[1] == 'political') {
                          region_code = result[i].address_components[0]['short_name'];
                        }
                      }
                      if(!region_code) {
                        for(i in result[0].address_components) {
                          if(result[0].address_components[i].types[0] && result[0].address_components[i].types[0] == 'country' && result[0].address_components[i].types[1] && result[0].address_components[i].types[1] == 'political') {
                            region_code = result[0].address_components[i]['short_name'];
                          }
                        }
                      }
                      if(region_code) {
                        $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/" + region_code + "/" + self.selecting_level, function(data){
                          self.add_region_from_click(data);
                        });
                      }
                    } else if(status === 'OVER_QUERY_LIMIT') {
                      this.GM3.message(Drupal.t('Woah, slow down, Google is getting annoyed.'), 'warning');
                    } else if(status === 'ZERO_RESULTS') {
                      // Could be one of the following:
                      // Kosovo, Kashmir, St Vincent and the Grenadines,
                      // Anguilla
                      // or the Sea.
                      $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/UNKNOWN/" + self.selecting_level, function(data){
                        self.add_region_from_click(data);
                      });
                    } else {
                      // Likely to be an error, although given the total lack of
                      // documentation, it could well be something else.
                    }
                  });
                } else {
                  this.GM3.message(Drupal.t('Please delete an object from the map before adding another.'), 'warning');
                }
                break;
              case 'rightclick':
                var self = this;
                this.geo.geocode({location: event.latLng}, function(result, status){
                  if(status === 'OK') {
                    for(i in result) {
                      if(result[i].types[0] && result[i].types[0] == 'country' && result[i].types[1] && result[i].types[1] == 'political') {
                        var region_code = result[i].address_components[0]['short_name'];
                        $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/" + region_code + "/5", function(data){
                          self.remove_region_from_click(data);
                        });
                      }
                    }
                  } else if(status === 'OVER_QUERY_LIMIT') {
                    this.GM3.message(Drupal.t('Woah, slow down, Google is getting annoyed.'), 'warning');
                  } else if(status === 'ZERO_RESULTS') {
                    // Could be one of the following:
                    // Kosovo, Kashmir, St Vincent and the Grenadines, Anguilla
                    // or the Sea.
                    $.getJSON(Drupal.settings.gm3_region.callback2 + "/" + event.latLng.toString() + "/UNKNOWN/5", function(data){
                      self.remove_region_from_click(data);
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
            }
            break;
        }
      }
    }
    Drupal.GM3.region.prototype.add_region_from_click = function(data){
      if(data) {
        // We add this ID if we haven't already done so.
        var notfound = true;
        for( var i in this.countries) {
          if(i == data) {
            notfound = false;
          }
        }
        if(notfound) {
          this.add_polygons_by_ids(data);
          this.update_field();
          this.GM3.num_objects++;
        } else {
          this.GM3.message(Drupal.t('Already selected that region'));
        }
      }
    }
    Drupal.GM3.region.prototype.remove_region_from_click = function(data){
      if(data) {
        // Split the string, and attempt to remove each one.
        data = data.split(':');
        data[1] = data[0] + ':' + data[1];
        data[2] = data[1] + ':' + data[2];
        data[3] = data[2] + ':' + data[3];
        data[4] = data[3] + ':' + data[4];
        var found_region = false;
        for( var i in data) {
          if(this.countries[data[i]] != undefined) {
            found_region = true;
            this.remove_polygons_by_id(data[i]);
            this.GM3.num_objects--;
          }
        }
        if(found_region) {
          this.update_field();
        }
      }
    }
    Drupal.GM3.region.prototype.get_message_from_level = function(level){
      switch(level){
        case 1:
          return Drupal.t("Selecting by continent (Level 1)");
        case 2:
          return Drupal.t("Selecting by sub-continent (Level 2)");
        case 3:
          return Drupal.t("Selecting by country/subcountry (Level 3)");
        case 5:
          return Drupal.t("Selecting by vice county (Level 5) - UK Only");
        case 4:
        default:
          return Drupal.t("Selecting by country/subcountry (Level 4)");
      }
    }
    Drupal.GM3.region.prototype.get_level_from_zoom = function(zoom){
      if(zoom < 3) {
        return 1;
      } else if(zoom < 5) {
        return 2;
      } else if(zoom < 6) {
        return 3;
      } else if(zoom < 7) {
        return 4;
      }
      return 5;
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
  }
})(jQuery);
