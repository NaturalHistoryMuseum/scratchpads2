(function($){
  Drupal.GM3 = function(map){
    this.settings = map.settings;
    this.id = map.id;
    this.initialized = false;
    this.tools = typeof (map.tools) != 'undefined' ? map.tools : new Array();
    this.libraries = typeof (map.libraries) != 'undefined' ? map.libraries : new Object();
    this.active_class = 'default';
    this.children = new Object();
    try {
      $('#' + this.id).height(this.settings['height']);
      $('#' + this.id).width(this.settings['width']);
      this.default_settings();
      // Create the map
      this.google_map = new google.maps.Map(document.getElementById(this.id), this.settings);
      this.initialized = true;
      // Add libraries
      for(id in this.libraries){
        if(Drupal.GM3[id]){
          this.children[id] = new Drupal.GM3.point(this);
        }
      }
      this.add_toolbar_listeners();
    } catch(err) {
      $('#' + this.id).html(Drupal.t('There has been an error with your map. Please contact an administrator.'));
    }    
    return this;
  }

  Drupal.GM3.prototype.add_toolbar_listeners = function(){
    // Click the stuff!
    var self=this;
    $('.gm3-tools ul li div').click(function(){
      $('.gm3-clicked').removeClass('gm3-clicked');
      $(this).addClass('gm3-clicked');
      if(self.children[$(this).data('gm3-class')] && self.children[$(this).data('gm3-class')].add_listeners) {
        self.active_class = $(this).data('gm3-class');
        self.clear_listeners();
        $(this).parent().addClass('gm3-clicked');
        self[self.active_class].add_listeners();
      } else {
        return;
        // Default button clicked (or missing the class).
        $('.gm3-clicked').removeClass('gm3-clicked');
        $(this).parent().addClass('gm3-clicked');
        this.google_map.setOptions({draggableCursor: 'pointer'});
        this.clear_listeners(this.id);
        this.add_edit_listeners(this.id);
      }
    });
  }

  Drupal.GM3.prototype.default_settings = function(){
    // MapTypeID
    this.settings['mapTypeId'] = eval(this.settings['mapTypeId']);
    // Center
    this.settings['center'] = new google.maps.LatLng(this.settings['center']['latitude'], this.settings['center']['longitude']);
    // Map control
    if(this.settings['mapTypeControlOptions'] && this.settings['mapTypeControlOptions']['mapTypeIds']) {
      for(map_type in this.settings['mapTypeControlOptions']['mapTypeIds']) {
        this.settings['mapTypeControlOptions']['mapTypeIds'][map_type] = eval(this.settings['mapTypeControlOptions']['mapTypeIds'][map_type]);
      }
    }
    if(this.settings['mapTypeControlOptions'] && this.settings['mapTypeControlOptions']['position'] && this.settings['mapTypeControlOptions']['style']) {
      this.settings['mapTypeControlOptions']['position'] = eval(this.settings['mapTypeControlOptions']['position']);
      this.settings['mapTypeControlOptions']['style'] = eval(this.settings['mapTypeControlOptions']['style']);
    }
    // PanControlOptions
    if(this.settings['panControlOptions'] && this.settings['panControlOptions']['position']) {
      this.settings['panControlOptions']['position'] = eval(this.settings['panControlOptions']['position']);
    }
    // rotateControlOptions
    if(this.settings['rotateControlOptions'] && this.settings['rotateControlOptions']['position']) {
      this.settings['rotateControlOptions']['position'] = eval(this.settings['rotateControlOptions']['position']);
    }
    // scaleControlOptions
    if(this.settings['scaleControlOptions'] && this.settings['scaleControlOptions']['position'] && this.settings['scaleControlOptions']['style']) {
      this.settings['scaleControlOptions']['position'] = eval(this.settings['scaleControlOptions']['position']);
      this.settings['scaleControlOptions']['style'] = eval(this.settings['scaleControlOptions']['style']);
    }
    // streetViewControlOptions
    if(this.settings['streetViewControlOptions'] && this.settings['streetViewControlOptions']['position']) {
      this.settings['streetViewControlOptions']['position'] = eval(this.settings['streetViewControlOptions']['position']);
    }
    // zoomControlOptions
    if(this.settings['zoomControlOptions'] && this.settings['zoomControlOptions']['position'] && this.settings['zoomControlOptions']['style']) {
      this.settings['zoomControlOptions']['position'] = eval(this.settings['zoomControlOptions']['position']);
      this.settings['zoomControlOptions']['style'] = eval(this.settings['zoomControlOptions']['style']);
    }
  }

  Drupal.behaviors.gm3 = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if($('#' + map_id, context).length) {
        // Create the new GM3 map object.
        Drupal.settings.gm3.maps[map_id] = new Drupal.GM3(Drupal.settings.gm3.maps[map_id]);
      }
    }
  }};

  Drupal.GM3.prototype.clear_listeners = function(map_id){
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "click");
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "mousemove");
    google.maps.event.clearListeners(Drupal.settings.gm3.maps[map_id]['google_map'], "rightclick");
    // Clear all listeners from this map.
    for(i in gm3) {
      if(gm3[i]['clear_listeners']) {
        gm3[i]['clear_listeners'](map_id);
      }
    }
  }
  Drupal.GM3.add_transfer_listeners = function(map_id){
    // Add transfer listeners so that polygons and other objects pass on their
    // clicks to the map.
    for(i in gm3) {
      if(gm3[i]['add_transfer_listeners']) {
        gm3[i]['add_transfer_listeners'](map_id);
      }
    }
  }
  Drupal.GM3.add_listeners = function(map_id){
    // Add listeners
    for(i in gm3) {
      if(gm3[i]['add_listeners']) {
        gm3[i]['add_listeners'](map_id);
      }
    }
  }
  Drupal.GM3.add_edit_listeners = function(map_id){
    // Add Edit listeners
    for(i in gm3) {
      if(gm3[i]['add_edit_listeners']) {
        gm3[i]['add_edit_listeners'](map_id);
      }
    }
  }
})(jQuery);