(function($){
  if(typeof Drupal.GM3 != 'undefined') {
    Drupal.GM3.point.prototype.update_field = function(){
      // Update the field.
      var new_value = '';
      for( var i = 0; i < this.points.length; i++) {
        if(i > 0) {
          new_value += '|';
        }
        new_value += this.points[i].position.toString();
      }
      $('.' + this.GM3.id + '-point').val(new_value);
    }
    // Add a listener to the field so that we can update the points if they're
    // changed.
    Drupal.GM3.point.prototype.active = function(){
      this.GM3.google_map.setOptions({draggableCursor: 'pointer'});
      var self = this;
      $('.' + this.GM3.id + '-point').keyup(function(){
        if(self.points.length == 1) {
          var position_parts = $('.' + self.GM3.id + '-point').val().replace("(", "").replace(")", "").split(", ");
          if(position_parts.length == 2) {
            var position = new google.maps.LatLng(position_parts[0], position_parts[1]);
            if(position) {
              self.points[0].setPosition(position);
              // Todo: Why do we need the "reset" param here?
              self.GM3.add_latlng(position, true);
              self.GM3.autozoom();
            }
          }
        }
      });
    }
  }
})(jQuery);
