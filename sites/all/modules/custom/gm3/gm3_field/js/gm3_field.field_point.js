(function($){
  Drupal.GM3.field_point = function(map){
    // Point object.
    this.GM3 = map;
  }
  Drupal.GM3.field_point.prototype.event = function(event_type, event, event_object){
    switch(this.GM3.active_class){
      case 'point':
        switch(event_type){
          case 'click':
            console.log('Adding marker');
            break;
          case 'rightclick':
            if(event_object.getClass() == 'Marker') {
              console.log('Removing marker');
            }
            break;
        }
        break;
    }
  }
})(jQuery);