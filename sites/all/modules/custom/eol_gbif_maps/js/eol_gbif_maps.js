(function(){
  Drupal.GM3.eol_gbif_maps_overlay = class extends L.Evented {
    constructor(settings, listeners) {
      super();
      this.on(listeners);
      const gbifTileLayer = L.tileLayer(
        Drupal.settings.gm3.settings.eol_gbif_maps.tile_host + '/v2/map/occurrence/density/{z}/{x}/{y}{r}.png?taxonKey={taxonId}',
        {
          taxonId: settings.taxon_id,
          // Overrides the default tileLayer's value for {r} (retina string)
          r: L.Browser.retina ? '@2x' : '@1x'
        }
        );

      const overlayControl = L.control.layers({}, {
        [Drupal.t("GBIF data")]: gbifTileLayer
      });
      this.fire('addlayer', { layer: gbifTileLayer });
      this.fire('addlayer', { layer: overlayControl });
    }
  }
})();
