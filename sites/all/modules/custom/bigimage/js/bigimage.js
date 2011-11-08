(function($){
  Drupal.GM3.bigimage = function(map){
    this.GM3 = map;
    function CoordMapType() {}
    CoordMapType.prototype.tileSize = new google.maps.Size(256,256);
    CoordMapType.prototype.maxZoom = 19; 
    var self = this;
    CoordMapType.prototype.getTile = function(coord, zoom, ownerDocument) {
      var div = ownerDocument.createElement('DIV');
      div.innerHTML = '<img src="'+Drupal.settings.bigimage.callback+'/'+self.GM3.libraries.bigimage.fid+'/'+zoom+'/'+coord.x+'/'+coord.y+'"/>';
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      return div;
    };
    var coordinateMapType = new CoordMapType();
    this.GM3.google_map.mapTypes.set('coordinate',coordinateMapType);
    this.GM3.google_map.setMapTypeId('coordinate');
  }
})(jQuery);
