(function($){
  Drupal.GM3.bigimage = function(map){
    this.GM3 = map;
    // Quick hack to alter where we get the images from. ----------------------
    function CoordMapType() {}

    CoordMapType.prototype.tileSize = new google.maps.Size(256,256);
    CoordMapType.prototype.maxZoom = 19;

    CoordMapType.prototype.name = "Tile #s";
    CoordMapType.prototype.alt = "Tile Coordinate Map Type";

    CoordMapType.prototype.getTile = function(coord, zoom, ownerDocument) {
      var div = ownerDocument.createElement('DIV');
      console.log(coord);
      div.innerHTML = '<img src="/bigimage/1/'+zoom+'/'+coord.x+'/'+coord.y+'"/>';
      //div.innerHTML = coord;
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      return div;
    };


    var coordinateMapType = new CoordMapType();

    // Now attach the coordinate map type to the map's registry
    this.GM3.google_map.mapTypes.set('coordinate',coordinateMapType);

    // We can now set the map to use the 'coordinate' map type
    this.GM3.google_map.setMapTypeId('coordinate');

    // Quick hack to alter where we get the images from. ----------------------
  }
})(jQuery);
