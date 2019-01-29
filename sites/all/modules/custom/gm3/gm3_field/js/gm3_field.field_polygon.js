(function($){
  if(typeof Drupal.GM3 != 'undefined') {
    // KNOWN BUG - If removing a polygon, it doesn't get removed until the map
    // is
    // clicked/rightclicked on.
    Drupal.GM3.polygon.prototype.updateField = function(){
      // Update the field.
      var new_value = '';
      for( var i = 0; i < this.polygons.length; i++) {
        if(new_value.length) {
          new_value += "\n";
        }
        this.polygons[i].getLatLngs().forEach(function(paths){
          // Only continue if the path has three or more points.
          if(paths.length > 2) {
            new_value += "POLYGON (("
            for( var ii = 0; ii < paths.length; ii++) {
              if(ii > 0) {
                new_value += ",";
              }
              new_value += paths[ii].lng + " " + paths[ii].lat
            }
            new_value += "," + paths[0].lng + " " + paths[0].lat
            new_value += "))"
          }
        })
      }
      this.fire('update', { cls: id => `.${id}-polygon`, value: new_value });
    }
  }
})(jQuery);
