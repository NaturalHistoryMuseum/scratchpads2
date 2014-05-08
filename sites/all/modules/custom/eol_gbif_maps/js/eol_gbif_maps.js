(function($){
  Drupal.GM3.eol_gbif_maps_overlay = function(map){
    this.GM3 = map;
    function EOLGBIFMapType(){
      this.tileSize = new google.maps.Size(256, 256);
    }
    var self = this;
    EOLGBIFMapType.prototype.getTile = function(coord, zoom, ownerDocument){
      var div = ownerDocument.createElement('DIV');
      div.innerHTML = '<img class="eol_gbif_map_tile" src="' + Drupal.settings.gm3.settings.eol_gbif_maps.tile_url + self.GM3.libraries.eol_gbif_maps_overlay.taxon_id + '&x=' + coord.x + '&y=' + coord.y + '&z=' + zoom + '' + '"/>';
      div.style.width = this.tileSize.width + 'px';
      div.style.height = this.tileSize.height + 'px';
      return div;
    };
    this.overlay = new EOLGBIFMapType();
    this.GM3.google_map.overlayMapTypes.insertAt(0, this.overlay);
    // Add a toggle button.
    $('#' + this.GM3.id).parent().append('<div class="gbif-toggle" style="position:relative; top:-20px;left:100px;height:0px;"><form><input id="gbif-toggle" type="checkbox" checked="checked"><label class="option" for="gbif-toggle">' + Drupal.t('GBIF data') + '</label></form></div>');
    var self = this;
    $('#gbif-toggle').change(function(){
      if($('#gbif-toggle').attr('checked')) {
        self.GM3.google_map.overlayMapTypes.insertAt(0, self.overlay);
      } else {
        self.GM3.google_map.overlayMapTypes.removeAt(0);
      }
    });
  }
})(jQuery);