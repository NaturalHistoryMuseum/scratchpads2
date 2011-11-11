(function($){
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