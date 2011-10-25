(function($){
  Drupal.GM3.field_polygon = function(map){
    // All we need to do is alter the polygon "event" function
    map.children.polygon.event = function(event_type, event, event_object){
      switch(this.GM3.active_class){
        case 'polygon':
          switch(event_type){
            case 'click':
              this.polygons[this.polygons.length - 1].stopEdit();
              this.polygons[this.polygons.length - 1].getPath().push(event.latLng);
              this.polygons[this.polygons.length - 1].runEdit(true);
              // field_polygon ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
            case 'mousemove':
              var pathLength = this.polygons[this.polygons.length - 1].getPath().getLength();
              if(pathLength >= 1) {
                var startingPoint1 = this.polygons[this.polygons.length - 1].getPath().getAt(pathLength - 1);
                var followCoordinates1 = [startingPoint1, event.latLng];
                this.followline1.setPath(followCoordinates1);
                var startingPoint2 = this.polygons[this.polygons.length - 1].getPath().getAt(0);
                var followCoordinates2 = [startingPoint2, event.latLng];
                this.followline2.setPath(followCoordinates2);
              }
              break;
            case 'rightclick':
              this.GM3.set_active_class('default');
              this.followline1.setMap(null);
              this.followline2.setMap(null);
              // field_polygon ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
          }
          break;
        case 'default':
          switch(event_type){
            case 'click':
              if(event_object.getClass && event_object.getClass() == 'Polygon') {
                // Once clicked, stop editing other polygons
                for( var j = 0; j < this.polygons.length; j++) {
                  this.polygons[j].stopEdit();
                }
                // We need to check this object is one of ours. Else we simply
                // ignore it
                for( var i = 0; i < this.polygons.length; i++) {
                  if(event_object == this.polygons[i]) {
                    this.polygons[i].runEdit();
                  }
                }
              } else {
                // Clicked elsewhere, stop editing.
                for( var j = 0; j < this.polygons.length; j++) {
                  this.polygons[j].stopEdit();
                }
              }
              // field_polygon ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
            case 'rightclick':
              if(event_object.getClass && event_object.getClass() != 'Polygon') {
                // Once clicked, stop editing other polygons
                for( var j = 0; j < this.polygons.length; j++) {
                  this.polygons[j].stopEdit();
                }
              }
              // field_polygon ADDITION.
              this.update_field();
              // END OF ADDITION
              break;
          }
          break;
      }
    }
  }
  Drupal.GM3.polygon.prototype.update_field = function(){
    // Update the field.
    var new_value = '';
    for( var i = 0; i < this.polygons.length; i++) {
      if(i > 0) {
        new_value += "\n";
      }
      new_value += "POLYGON (("
      this.polygons[i].getPaths().forEach(function(paths){        
        for(var j = 0; j < paths.length; j++){
          if(j > 0){
            new_value += ",";
          }
          new_value += paths.b[j].lng() + " " + paths.b[j].lat()
        }          
      })
      new_value += "))"
    }
    console.log(new_value);
    $('.' + this.GM3.id + '-hidden_field').val(new_value);
  }
})(jQuery);