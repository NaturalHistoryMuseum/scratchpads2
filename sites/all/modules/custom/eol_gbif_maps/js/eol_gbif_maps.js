(function(){
  Drupal.GM3.eol_gbif_maps_overlay = class extends L.Control.Layers {
    constructor(settings) {
      const gbifOptions = new URLSearchParams({
        bin: 'square',
        squareSize: 64,
        style: 'purpleYellow-noborder.poly'
      });
      const gbifTileLayer = L.tileLayer(
        `${Drupal.settings.gm3.settings.eol_gbif_maps.tile_host}/v2/map/occurrence/density/{z}/{x}/{y}{r}.png?taxonKey={taxonId}&${gbifOptions}`,
        {
          taxonId: settings.taxon_id,
          // Overrides the default tileLayer's value for {r} (retina string)
          r: L.Browser.retina ? '@2x' : '@1x',
          className: 'gm3-gbif-tile'
        }
      );
      const checkboxText = Drupal.t("GBIF data");

      super(
        {},
        { [checkboxText]: gbifTileLayer },
        { collapsed: false }
      );

      this.gbifTileLayer = gbifTileLayer;
    }

    /**
     * Make sure the overlay is enabled by default
     * @param {L.Map} map The map this control is added to
     */
    onAdd(map) {
      this.gbifTileLayer.addTo(map);
      return super.onAdd(map);
    }
  }
})();
