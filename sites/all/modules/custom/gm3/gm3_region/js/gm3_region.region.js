(function($){
  if(typeof Drupal.GM3 != 'undefined') {
    /**
     * Map module for selecting and displaying geographical regions
     */
    Drupal.GM3.region = class extends L.Evented {
      constructor(map, settings) {
        super();
        this.countries = {};


        // Array of functions to call when we deactivate
        this.teardowns = [];

        // Add Regions sent from server.
        if(settings.regions) {
          this.addPolygonsByIds(settings.regions, map);
        }
      }

      /**
       * Add a polygon by its region ID
       * @param {string|string[]} regionIds Region ids to add or list thereof
       * @param {gm3.map} map The leaflet map to add the polygons to
       * @param {bool} autofit True to auto zoom the map
       */
      async addPolygonsByIds(regionIds, map, autofit = true){
        if (typeof regionIds === 'string') {
          regionIds = [regionIds];
        }
        if (!Array.isArray(regionIds)) {
          throw new TypeError(`Expected regionIds to be array or string; got ${ typeof regionIds } (${regionIds})`);
        }

        // Execute the callback to get the Polygon. This Polygon should then
        // be added to the map, but without it being editable.
        const regionIdsToAdd = [];
        for (const regionId of regionIds) {
          if(!this.countries[regionId]) {
            // If this is a new region, add it to the countries array and mark to be added
            this.countries[regionId] = [];
            regionIdsToAdd.push(regionId);
          } else if(!regionIdsToAdd.includes(regionId)) {
            // If this region is not in the list of regions to add, remove it from the map (???)
            this.removePolygonsById(regionId);
          }
        }

        // We need to do this x regions at a time, else the server will complain
        // that the URL is too long
        const batchSize = 10;
        const regionBatches = [];
        let sliceStart = 0;

        while(sliceStart < regionIdsToAdd.length) {
          const sliceEnd = sliceStart + batchSize;
          regionBatches.push(
            regionIdsToAdd.slice(sliceStart, sliceEnd)
          );
          sliceStart = sliceEnd;
        }

        for(const batch of regionBatches) {
          const regionDataUrl = Drupal.settings.gm3_region.callback;
          const res = await fetch(`${regionDataUrl}/${batch.join(',')}`);
          const data = await res.json();

          for (const region of data) {
            const regionId = region.regionId || Object.keys(region)[0];
            const shape = region.shape || region[regionId].shape;

            for (const coordinate of shape.coordinates) {
              if(shape.type === 'MultiPolygon') {
                for(const points of coordinate) {
                  // We have a region with multiple shapes.
                  this.countries[regionId].push(
                    // Todo - handle this as an event?
                    // Or extend from polygon class?
                    this.addPolygon(points, map)
                  );
                }
              } else if(shape.type === 'Polygon') {
                this.countries[regionId].push(
                  // Todo - handle this as an event?
                  // Or extend from polygon class?
                  this.addPolygon(coordinate, map)
                );
              }
            }
          }
        }

        if(typeof autofit !== 'undefined' && autofit && this.GM3) {
          // Todo: Fire this as event to parent
          if(this.GM3.max_lat) {
            this.GM3.google_map.fitBounds(
              new google.maps.LatLngBounds(
                new google.maps.LatLng(
                  this.GM3.min_lat, this.GM3.min_lng
                ),
                new google.maps.LatLng(
                  self.GM3.max_lat, self.GM3.max_lng
                )
              )
            );
          }
        }
      }

      /**
       * Add a polygon to the map
       * @param {LatLng} points The points to use to construct the polygon
       * @param {Leaflet} map The map to add the polygon to
       */
      addPolygon(points, map) {
        // Todo: Refactor redundant code
        const pathPoints = points.map(point => Array.isArray(points) ? L.latLng([point[1], point[0]]) : L.latLng(points));

        const polyOptions = {
          color: '#000000',
          opacity: 0.4,
          weight: 1
        };

        const poly = L.polygon(pathPoints, polyOptions);
        poly.addTo(map);
        return poly;
      }

      /**
       * Remove a region by its region ID
       * @param {string} regionId Region ID to remove
       */
      removePolygonsById(regionId) {
        for(const region of this.countries[regionId]) {
          region.remove();
        }
        this.countries[regionId] = null;
      }

      /**
       * Hook called by the parent gm3 object when this tool is being enabled
       * @param {LeafletMap} map Activating map
       */
      activate(map) {
        // Todo: Fire event for this
        // map.setOptions({ draggableCursor: 'pointer' });

        // Todo: Factor this out into subclass
        // Add tool functionality
        this.listeners = {
          click: e => this.selectRegion(e.latlng, map),
          contextmenu: e => this.selfDisable()
        }

        map.on(this.listeners);

        // Register a function to remove the listeners
        this.teardowns.push(() => map.off(this.listeners))
      }

      /**
       * Disable this tool
       */
      selfDisable(){
        // Todo: Factor out redundant code
        this.fire('deactivate');
      }

      /**
       * Given a point, highlight the region on the map
       * @param {LatLng} latLng The point on the map selected by the user
       * @param {LeafletMap} map The map that was selected
       */
      async selectRegion (latLng, map) {
        const geocodeUrl = ({ lat, lng }) => `https://nominatim.openstreetmap.org/reverse?format=json&lat=${lat}&lon=${lng}`;
        // Todo: Handle errors here
        const res = await fetch(geocodeUrl(latLng));
        const result = await res.json();
        const regionCode = result.address.country_code
        const res2 = await fetch(Drupal.settings.gm3_region.callback2 + "/" + latLng.lat + ', ' + latLng.lng + "/" + regionCode);
        const polygonIds = await res2.json();

        if(polygonIds) {
          this.addPolygonsByIds(polygonIds, map);
          this.updateField();
        }
      }

      /**
       * Hook called by the parent gm3 object when deactivating this tool
       */
      deactivate(){
        // Todo: Factor this stuff out
        this.active = false;

        // Remove event listeners
        this.teardowns.forEach(t => t());
        this.teardowns = [];
      }

      /**
       * Calculates the new field value and fires the update event
       */
      updateField() {
        const regions = Object.keys(this.countries).filter(k => this.countries[k]);

        this.fire('update', { cls: id => `.${id}-region`, value: regions });
      }
    }
  }
})(jQuery);
