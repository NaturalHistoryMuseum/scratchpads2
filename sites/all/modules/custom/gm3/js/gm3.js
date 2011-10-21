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
      for(id in this.libraries) {
        if(Drupal.GM3[id]) {
          this.children[id] = new Drupal.GM3[id](this);
        }
      }
      // Add listeners
      this.add_toolbar_listeners();
      this.add_map_moved_listener();
    } catch(err) {
      $('#' + this.id).html(Drupal.t('There has been an error with your map. Please contact an administrator.'));
    }
    return this;
  }
  Drupal.GM3.prototype.add_toolbar_listeners = function(){
    // Click the stuff!
    var self = this;
    $('#toolbar-' + this.id + ' li div').click(function(){
      self.set_active_class($(this).data('gm3-class'));
    });
  }
  Drupal.GM3.prototype.add_map_moved_listener = function(){
    // Ensure the user can not pan the map constantly. This is due to the
    // overlays we are using.
    this.allowedBounds = new google.maps.LatLngBounds(new google.maps.LatLng(-89.99999, -179.99999), new google.maps.LatLng(89.99999, 179.99999));
    this.lastValidCenter = this.google_map.getCenter();
    var self = this;
    google.maps.event.addListener(this.google_map, 'center_changed', function(event){
      if(self.allowedBounds.contains(self.google_map.getCenter())) {
        self.lastValidCenter = self.google_map.getCenter();
      } else {
        self.google_map.panTo(self.lastValidCenter);
      }
    });
  }
  Drupal.GM3.prototype.active = function(){
    this.google_map.setOptions({draggableCursor: 'pointer'});
  }
  Drupal.GM3.prototype.set_active_class = function(active_class){
    $('.gm3-clicked', '#toolbar-' + this.id).removeClass('gm3-clicked');
    $('div[data-gm3-class="' + active_class + '"]', '#toolbar-' + this.id).parent().addClass('gm3-clicked');
    this.active_class = active_class;
    this.add_listeners();
    if(this.active_class == 'default') {
      this.active();
    } else {
      if(this.children[this.active_class] && this.children[this.active_class].active) {
        this.children[this.active_class].active();
      }
    }
  }
  Drupal.GM3.prototype.add_listeners = function(){
    for(id in this.children) {
      // Add transfer listeners for each library
      if(this.children[id].add_transfer_listeners) {
        this.children[id].add_transfer_listeners();
      }
      // Add listeners for each library (if they define one).
      if(this.children[id].add_listeners) {
        this.children[id].add_listeners();
      }
    }
    // Add listeners to the map. These will in turn execute the callbacks for
    // the currently active class (or default).
    this.add_listeners_helper();
  }
  Drupal.GM3.prototype.event = function(event_type, event){}
  Drupal.GM3.prototype.add_listeners_helper = function(map_object){
    map_object = typeof (map_object) != 'undefined' ? map_object : this.google_map;
    var self = this;
    var events_array = ["click", "dblclick", "mousemove", "rightclick"];
    for(i in events_array) {
      eval('google.maps.event.clearListeners(map_object, "' + events_array[i] + '");' + 'google.maps.event.addListener(map_object, "' + events_array[i] + '", function(event){' + 'if(self.active_class == "default"){' + 'var child_overrode = false;' + 'for(i in self.children){' + 'if(self.children[i].event){' + 'child_overrode = self.children[i].event("' + events_array[i] + '", event, this);}' + 'if(child_overrode) {return;}}' + 'self.event("' + events_array[i] + '", event, this);}' + 'else {' + 'if(self.children[self.active_class].event) {' + 'self.children[self.active_class].event("' + events_array[i] + '", event, this);}}})');
    }
  }
  Drupal.GM3.prototype.clear_listeners = function(){
    for(id in this.children) {
      // Clear transfer listeners for each library (mostly not needed).
      if(this.children[id].clear_transfer_listeners) {
        this.children[id].clear_transfer_listeners();
      }
      // Clear listeners for each library (if they define one).
      if(this.children[id].clear_listeners) {
        this.children[id].clear_listeners();
      }
    }
    // Add listeners to the map. These will in turn execute the callbacks for
    // the currently active class (or default).
    this.clear_listeners_helper();
  }
  Drupal.GM3.prototype.clear_listeners_helper = function(map_object){
    map_object = typeof (map_object) != 'undefined' ? map_object : this.google_map;
    var self = this;
    google.maps.event.clearListeners(map_object, "click");
    google.maps.event.clearListeners(map_object, "dblclick");
    google.maps.event.clearListeners(map_object, "rightclick");
    google.maps.event.clearListeners(map_object, "mousemove");
  }
  Drupal.GM3.prototype.event = function(event_type, event){}
  Drupal.GM3.prototype.clear_listeners = function(){
    // Clear listeners from the map.
    google.maps.event.clearListeners(this.google_map, "click");
    google.maps.event.clearListeners(this.google_map, "mousemove");
    google.maps.event.clearListeners(this.google_map, "rightclick");
    // Clear all listeners from the children.
    for(i in this.children) {
      if(this.children[i]['clear_listeners']) {
        this.children[i]['clear_listeners']();
      }
    }
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
  // Entry point. Add a map to a page. This should hopefully work via AJAX.
  Drupal.behaviors.gm3 = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if($('#' + map_id, context).length && typeof(Drupal.settings.gm3.maps[map_id]['google_map']) == 'undefined') {
        // Create the new GM3 map object.
        Drupal.settings.gm3.maps[map_id] = new Drupal.GM3(Drupal.settings.gm3.maps[map_id]);
      }
    }
  }};
})(jQuery);