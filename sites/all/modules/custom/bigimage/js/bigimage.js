(function($){
  Drupal.GM3.bigimage = function(map, settings){
    this.GM3 = map;
    function BigImageMapType(){}
    BigImageMapType.prototype.tileSize = new google.maps.Size(256, 256);
    BigImageMapType.prototype.maxZoom = 19;
    var self = this;
    BigImageMapType.prototype.getTile = function(coord, zoom, ownerDocument){
      var div = ownerDocument.createElement('DIV');
      div.innerHTML = '<img src="' + Drupal.settings.bigimage.callback + '/' + settings.fid + '/' + zoom + '/' + coord.x + '/' + coord.y + '"/>';
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      return div;
    };
    var bigImageMapType = new BigImageMapType();
    this.GM3.google_map.mapTypes.set('bigimage', bigImageMapType);
    this.GM3.google_map.setMapTypeId('bigimage');
    var self = this;
    google.maps.event.addListener(this.GM3.google_map, 'idle', function(){
      $('a', '#' + self.GM3.id).parent().remove();
    })

    // Following has been added for debugging purposes.
    /*
    function CoordMapType(tileSize){
      this.tileSize = tileSize;
    }
    CoordMapType.prototype.getTile = function(coord, zoom, ownerDocument){
      var div = ownerDocument.createElement('DIV');
      div.innerHTML = coord;
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      div.style.fontSize = '10';
      div.style.borderStyle = 'solid';
      div.style.borderWidth = '1px';
      div.style.borderColor = '#AAAAAA';
      return div;
    };
    this.GM3.google_map.overlayMapTypes.insertAt(0, new CoordMapType(new google.maps.Size(256, 256)));
    */
  }
})(jQuery);
