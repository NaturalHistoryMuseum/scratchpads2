(function($){
  Drupal.gm3_infobubble = function(latlng, content, map){

    // Now initialize all properties.
    this.latlng_ = latlng;
    this.content_ = content;
    this.map_ = map;

    // We define a property to hold the image's
    // div. We'll actually create this div
    // upon receipt of the add() method so we'll
    // leave it null for now.
    this.div_ = null;

    // Explicitly call setMap() on this overlay
    this.setMap(map);

  }
  Drupal.gm3_infobubble.prototype = new google.maps.OverlayView();
  Drupal.gm3_infobubble.prototype.onAdd = function(){
    var div = document.createElement('DIV');
    div.style.border = "solid 1px black";
    div.style.borderRadius = '3px';
    div.style.MozBorderRadius = '3px';
    div.style.position = "absolute";
    div.innerHTML=this.content_;

    // Set the overlay's div_ property to this DIV
    this.div_ = div;

    // We add an overlay to a map via one of the map's panes.
    // We'll add this overlay to the overlayImage pane.
    var panes = this.getPanes();
    panes.overlayLayer.appendChild(div);    
  }
  Drupal.gm3_infobubble.prototype.onRemove = function(){
    this.div_.parentNode.removeChild(this.div_);
    this.div_ = null;
  }
  Drupal.gm3_infobubble.prototype.draw = function(){
    // We need to retrieve the projection from this overlay to do this.
    var overlayProjection = this.getProjection();    
    // Where are we sticking this?
    var point = overlayProjection.fromLatLngToDivPixel(this.latlng_);
    // Resize the image's DIV to fit the indicated dimensions.
    var div = this.div_;
    div.style.left = point.x + 'px';
    div.style.top = point.y - div.offsetHeight - 2 + 'px';
    console.log(div.offsetWidth);
    console.log(div.innerHTML);
    div.style.width = div.offsetWidth-2+'px';
    div.style.height = div.offsetHeight-2+'px';
  }
})(jQuery);