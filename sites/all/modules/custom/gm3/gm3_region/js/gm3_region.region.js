(function($){
  "use strict";

  const getLevelFromZoom = zoom => zoom < 3 ? 1 :
                                   zoom < 5 ? 2 :
                                   zoom < 6 ? 3 :
                                   zoom < 7 ? 4 :
                                              5 ;

  const getMessageFromLevel = level => (
    {
      1: Drupal.t("Selecting by continent (Level 1)"),
      2: Drupal.t("Selecting by sub-continent (Level 2)"),
      3: Drupal.t("Selecting by country/subcountry (Level 3)"),
      5: Drupal.t("Selecting by vice county (Level 5) - UK Only")
    }[level]||
      Drupal.t("Selecting by country/subcountry (Level 4)")
  );

  if(typeof Drupal.GM3 != 'undefined') {
    /**
     * Map module for selecting and displaying geographical regions
     */
    Drupal.GM3.region = class extends L.Evented {
      constructor(map, settings) {
        super();
        this.countries = {};

        // The TDWG level to use for region selection
        this.selectingLevel = 4;

        // Array of functions to call when we deactivate
        this.teardowns = [];

        // Add Regions sent from server.
        if(settings.regions) {
          this.addPolygonsByIds(settings.regions, map, settings.editable);
        }
      }

      /**
       * Add a polygon by its region ID
       * @param {string|string[]} regionIds Region ids to add or list thereof
       * @param {gm3.map} map The leaflet map to add the polygons to
       * @param {bool} autofit True to auto zoom the map
       */
      async addPolygonsByIds(regionIds, map, editable = false, autofit = true){
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
            const polygons = shape.type === 'MultiPolygon' ? shape.coordinates :
                             shape.type === 'Polygon' ? [shape.coordinates] : [];

            for (const polygon of polygons) {
              for(const points of polygon) {
                // Todo - handle this as an event?
                // Or extend from polygon class?
                const poly = this.addPolygon(points, map);

                this.countries[regionId].push(poly);

                if (editable) {
                  poly.on('contextmenu', e => {
                    this.removePolygonsById(regionId);
                    L.DomEvent.stopPropagation(e);
                  });
                }
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
        this.fire('removeobject');
        this.updateField();
      }

      /**
       * Hook called by the parent gm3 object when this tool is being enabled
       * @param {LeafletMap} map Activating map
       */
      activate(map) {
        // We add a little text to the top left of the map to say what level
        // we will be selecting.
        this.selectingLevel = getLevelFromZoom(map.getZoom());

        const levelMessage = document.createElement('div');
        levelMessage.setAttribute('class', 'gm3_information');
        levelMessage.style.cursor = 'pointer';
        // Todo: Use something else for this to stop the tooltip from moving around
        map.getPane('tooltipPane').appendChild(levelMessage);
        const setLevelMessage = () => levelMessage.innerHTML = `<p>${getMessageFromLevel(this.selectingLevel)}</p>`;
        setLevelMessage();

        levelMessage.addEventListener('click', () => {
          // We reduce the level by one, unless we're on one, then we set it as
          // 5
          this.selectingLevel = (this.selectingLevel - 1) % 5 || 5;
          setLevelMessage();
        });

        this.teardowns.push(() => levelMessage.remove());

        // Todo: Fire event for this
        // map.setOptions({ draggableCursor: 'pointer' });

        // Todo: Factor this out into subclass
        // Add tool functionality
        this.listeners = {
          click: e => this.selectRegion(e.latlng, map),
          contextmenu: e => this.selfDisable(),
          zoom: e => {
            this.selectingLevel = getLevelFromZoom(map.getZoom());
            setLevelMessage();
          }
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
        // Todo: Factor out this addobject code
        const options = {
          cancelled: false
        };
        // Todo: This shouldn't actually add the object until after we complete
        this.fire('addobject', options);

        if (options.cancelled) {
          return;
        }

        const geocodeUrl = ({ lat, lng }) => `https://nominatim.openstreetmap.org/reverse?format=json&lat=${lat}&lon=${lng}`;
        // Todo: Handle errors here
        const res = await fetch(geocodeUrl(latLng));
        const result = await res.json();
        const regionCode = result.address ? result.address.country_code : 'UNKNOWN';
        const res2 = await fetch(`${Drupal.settings.gm3_region.callback2}/${latLng.lat}, ${latLng.lng}/${regionCode}/${this.selectingLevel}`);
        const polygonId = await res2.json();

        if(polygonId) {
          if (this.countries[polygonId]) {
            this.fire('message', { message: 'Region already selected' });
          } else {
            this.addPolygonsByIds(polygonId, map, true);
            this.updateField();
          }
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
        // Todo: Factor out codes
        const regions = Object.keys(this.countries).filter(k => this.countries[k]);

        this.fire('update', { cls: id => `.${id}-region`, value: regions });
      }
    }
  }
})(jQuery);
