(function($){
  if(typeof Drupal.GM3 != 'undefined') {
    // KNOWN BUG - If removing a rectangle, it doesn't get removed until the map
    // is
    // clicked/rightclicked on.
    Drupal.GM3.rectangle.prototype.update_field = function(){
      // Update the field.
      var new_value = '';
      for( var i = 0; i < this.rectangles.length; i++) {
        if(new_value.length) {
          new_value += "\n";
        }
        this.rectangles[i].getPaths().forEach(function(paths){
          // Only continue if the path has three or more points.
          if(paths.length > 2) {
            new_value += "POLYGON (("
            for( var ii = 0; ii < paths.length; ii++) {
              if(ii > 0) {
                new_value += ",";
              }
              new_value += paths.j[ii].lng() + " " + paths.j[ii].lat()
            }
            new_value += "))"
          }
        })
      }
      $('.' + this.GM3.id + '-rectangle').val(new_value);
    }
  }
})(jQuery);