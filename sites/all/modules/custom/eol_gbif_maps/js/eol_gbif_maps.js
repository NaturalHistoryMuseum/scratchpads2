(function($){
  const tileSize = 512;

  function gbif_tile_url(gbifHost, taxonId, coord, zoom) {
    return `${gbifHost}/v2/map/occurrence/density/${zoom}/${coord.x}/${coord.y}@1x.png?taxonKey=${taxonId}`;
  }

  Drupal.GM3.eol_gbif_maps_overlay = function(map, settings){
    this.GM3 = map;
    function EOLGBIFMapType(){
      this.tileSize = new google.maps.Size(tileSize, tileSize);
    }
    var self = this;
    EOLGBIFMapType.prototype.getTile = function(coord, zoom, ownerDocument){
      // We want zero-indexed zoom counter, but google uses 1-indexed.
      zoom = zoom - 1;
      var div = ownerDocument.createElement('DIV');
      // "Wrap" x (longitude) at 180th meridian properly
      // NB: Don't touch coord.x: because coord param is by reference, and changing its x property breaks something in Google's lib
      const tilesPerGlobe = 1 << zoom;
      var x = coord.x % tilesPerGlobe;
      if (x < 0) {
          x = tilesPerGlobe + x;
      }

      const y = coord.y;

      const src = gbif_tile_url(Drupal.settings.gm3.settings.eol_gbif_maps.tile_host, settings.taxon_id, { x, y }, zoom);
      // Apparently gbif returns 204 for some regions where there's no data (e.g. oceans)
      // Add onerror handler to hide broken image in these cases
      div.innerHTML = '<img class="eol_gbif_map_tile" src="' + src + '" onerror="this.style.display=\'none\'"/>';
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
