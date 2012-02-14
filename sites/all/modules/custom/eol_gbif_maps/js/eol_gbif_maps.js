(function($){
  Drupal.GM3.eol_gbif_maps_overlay = function(map){
    this.GM3 = map;
    function EOLGBIFMapType(){
      this.tileSize = new google.maps.Size(256, 256);
    }
    var self = this;
    EOLGBIFMapType.prototype.getTile = function(coord, zoom, ownerDocument){
      var div = ownerDocument.createElement('DIV');
      console.log(Drupal.settings.gm3.settings.eol_gbif_maps.tile_url + coord.x + '_' + coord.y + '_' + zoom + '_' + self.GM3.libraries.eol_gbif_maps_overlay.taxon_id);
      div.innerHTML = '<img class="eol_gbif_map_tile" src="' + Drupal.settings.gm3.settings.eol_gbif_maps.tile_url + coord.x + '_' + coord.y + '_' + zoom + '_' + self.GM3.libraries.eol_gbif_maps_overlay.taxon_id + '"/>';
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      return div;
    };
    this.GM3.google_map.overlayMapTypes.insertAt(0, new EOLGBIFMapType());
  }
})(jQuery);