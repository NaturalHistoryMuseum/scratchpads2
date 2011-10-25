(function($){
  Drupal.GM3.field_point = function(map){
    // All we need to do is alter the point "event" function
    map.children.point.event = function(event_type, event, event_object){
      switch(this.GM3.active_class){
        case 'point':
          switch(event_type){
            case 'click':
              this.add_marker(event.latLng, true);
              // field_point ADDITION.
              this.update_field();              
              // END OF ADDITION
              break;
            case 'rightclick':
              switch(event_object.getClass()){
                case 'Map':
                  this.GM3.set_active_class('default');
                  break;
                case 'Marker':
                  // Loop through this objects points, and unset the one(s) that
                  // equal this object.
                  for( var i = 0; i < this.points.length; i++) {
                    if(this.points[i].position.equals(event_object.position)) {
                      this.clusterer.removeMarker(this.points[i], true);
                      this.points[i].setMap(null);
                      this.points[i] = undefined;
                    }
                  }
                  // Finally, close up the array, which seems pretty clunky, but
                  // perhaps the only way of doing this.
                  var new_points = new Array();
                  var j = 0;
                  for( var i = 0; i < this.points.length; i++) {
                    if(this.points[i] != undefined) {
                      new_points[j] = this.points[i];
                      j++;
                    }
                  }
                  this.points = new_points;
                  // field_point ADDITION.
                  this.update_field();              
                  // END OF ADDITION
                  break;
              }
              break;
          }
          break;
      }
    }
  }
  Drupal.GM3.point.prototype.update_field = function(){
    // Update the field.
    var new_value = '';
    for(var i = 0; i < this.points.length; i++){
      if(i > 0){
        new_value += '|';
      }
      new_value += this.points[i].position.toString();
    }
    $('.'+this.GM3.id+'-hidden_field').val(new_value);
  }
})(jQuery);