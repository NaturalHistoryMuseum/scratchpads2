(function($){
  Drupal.GM3.bigimage = function(map){
    this.GM3 = map;
    function BigImageMapType(){}
    BigImageMapType.prototype.tileSize = new google.maps.Size(256, 256);
    BigImageMapType.prototype.maxZoom = 19;
    var self = this;
    BigImageMapType.prototype.getTile = function(coord, zoom, ownerDocument){
      var div = ownerDocument.createElement('DIV');
      div.innerHTML = '<img src="' + Drupal.settings.bigimage.callback + '/' + self.GM3.libraries.bigimage.fid + '/' + zoom + '/' + coord.x + '/' + coord.y + '"/>';
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      return div;
    };
    var bigImageMapType = new BigImageMapType();
    this.GM3.google_map.mapTypes.set('bigimage', bigImageMapType);
    this.GM3.google_map.setMapTypeId('bigimage');
    var self = this;
    google.maps.event.addListener(this.GM3.google_map, 'idle', function(){
      $('a','#' + self.GM3.id).parent().remove();
    })
  }
})(jQuery);