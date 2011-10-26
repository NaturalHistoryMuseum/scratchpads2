(function($){
  Drupal.GM3.field_polyline = function(map){
    // All we need to do is alter the polyline "event" function
    map.children.polyline.event = function(event_type, event, event_object){
      switch(this.GM3.active_class){
        case 'polyline':
          switch(event_type){
            case 'click':
              this.polylines[this.polylines.length - 1].stopEdit();
              this.polylines[this.polylines.length - 1].getPath().push(event.latLng);
              this.polylines[this.polylines.length - 1].runEdit(true);
              // field_polyline ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
            case 'mousemove':
              var pathLength = this.polylines[this.polylines.length - 1].getPath().getLength();
              if(pathLength >= 1) {
                var startingPoint = this.polylines[this.polylines.length - 1].getPath().getAt(pathLength - 1);
                var followCoordinates = [startingPoint, event.latLng];
                this.followline.setPath(followCoordinates);
              }
              break;
            case 'rightclick':
              this.GM3.set_active_class('default');
              this.followline.setMap(null);
              // field_polyline ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
          }
          break;
        case 'default':
          switch(event_type){
            case 'click':
              if(event_object.getClass && event_object.getClass() == 'Polyline') {
                // Once clicked, stop editing other polylines
                for( var j = 0; j < this.polylines.length; j++) {
                  this.polylines[j].stopEdit();
                }
                event_object.runEdit();
              } else {
                // Clicked elsewhere, stop editing.
                for( var j = 0; j < this.polylines.length; j++) {
                  this.polylines[j].stopEdit();
                }
              }
              // field_polyline ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
          }
          break;
      }
    }
  }
  // KNOWN BUG - If removing a polyline, it doesn't get removed until the map is
  // clicked/rightclicked on.
  Drupal.GM3.polyline.prototype.update_field = function(){
    // Update the field.
    var new_value = '';
    for( var i = 0; i < this.polylines.length; i++) {
      if(new_value.length) {
        new_value += "\n";
      }
      var path = this.polylines[i].getPath();
      if(path.length > 1) {
        new_value += "POLYGON (("
        for( var j = 0; j < path.length; j++) {
          if(j > 0) {
            new_value += ",";
          }
          new_value += path.b[j].lng() + " " + path.b[j].lat()
        }
        new_value += "))"
      }
    }
    $('.' + this.GM3.id + '-polyline').val(new_value);
  }
})(jQuery);