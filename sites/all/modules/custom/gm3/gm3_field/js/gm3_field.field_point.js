(function($){
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
})(jQuery);