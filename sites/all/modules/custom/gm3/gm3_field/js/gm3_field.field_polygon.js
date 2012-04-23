(function($){
  // KNOWN BUG - If removing a polygon, it doesn't get removed until the map is
  // clicked/rightclicked on.
  Drupal.GM3.polygon.prototype.update_field = function(){
    // Update the field.
    var new_value = '';
    for( var i = 0; i < this.polygons.length; i++) {
      if(new_value.length) {
        new_value += "\n";
      }
      this.polygons[i].getPaths().forEach(function(paths){
        // Only continue if the path has three or more points.
        if(paths.length > 2) {
          new_value += "POLYGON (("
          for( var j = 0; j < paths.length; j++) {
            if(j > 0) {
              new_value += ",";
            }
            new_value += paths.b[j].lng() + " " + paths.b[j].lat()
          }
          new_value += "," + paths.b[0].lng() + " " + paths.b[0].lat()
          new_value += "))"
        }
      })
    }
    $('.' + this.GM3.id + '-polygon').val(new_value);
  }
})(jQuery);