(function(){
  "use strict";

  const getLevelFromZoom = zoom => zoom < 3 ? 1 :
                                   zoom < 5 ? 2 :
                                   zoom < 6 ? 3 :
                                   zoom < 7 ? 4 :
                                              5 ;

  // We add a little text to the top left of the map to say what level
  // we will be selecting.
  const LevelControl = class extends L.Control {
    constructor(...args){
      super(...args);

      // The TDWG level to use for region selection
      this.selectingLevel = 4;
    }
    onAdd(map) {
      const levelMessage = document.createElement('button');

      this.selectingLevel = getLevelFromZoom(map.getZoom());

      map.on('zoom', e => {
        this.selectingLevel = getLevelFromZoom(map.getZoom());
        this.updateMessage(levelMessage);
      });

      levelMessage.addEventListener('click', e => {
        e.stopPropagation();
        this.changeSelectingLevel()
      });
      levelMessage.setAttribute('class', 'leaflet-control gm3-region-level-control');
      levelMessage.setAttribute('type', 'button');
      this.updateMessage(levelMessage);

      return levelMessage;
    }
    updateMessage(element) {
      const level = this.selectingLevel;
      const message = {
        1: Drupal.t("Selecting by continent (Level 1)"),
        2: Drupal.t("Selecting by sub-continent (Level 2)"),
        3: Drupal.t("Selecting by country/subcountry (Level 3)"),
        5: Drupal.t("Selecting by vice county (Level 5) - UK Only")
      }[level]|| Drupal.t("Selecting by country/subcountry (Level 4)")

      element.innerHTML = `<p>${message}</p>`;
    }
    changeSelectingLevel() {
      this.selectingLevel = (this.selectingLevel - 1) % 5 || 5;
      this.updateMessage(this.getContainer());
    }
    onRemove(map){}
  }

  if(typeof Drupal.GM3 != 'undefined') {
    /**
     * Map module for selecting and displaying geographical regions
     */
    Drupal.GM3.region = class extends Drupal.GM3.Library {
      constructor(settings) {
        super();
        this.countries = {};

        this.levelControl = new LevelControl({ position: 'topleft' })

        // Add Regions sent from server.
        if(settings.regions) {
          // Store the promise for the initial load
          // This lets the map autozoom after the polygons have been downloaded
          // Though ideally we'd get the polygons on the server side
          this.initialLoad = this.addPolygonsByIds(settings.regions, settings.editable);
        }
      }

      /**
       * Add a polygon by its region ID
       * @param {string|string[]} regionIds Region ids to add or list thereof
       * @param {gm3.map} map The leaflet map to add the polygons to
       * @param {bool} autofit True to auto zoom the map
       */
      async addPolygonsByIds(regions, editable = false){
        if (typeof regions === 'string') {
          regions = [regions];
        }
        if (!Array.isArray(regions)) {
          throw new TypeError(`Expected regionIds to be array or string; got ${ typeof regionIds } (${regionIds})`);
        }

        // Execute the callback to get the Polygon. This Polygon should then
        // be added to the map, but without it being editable.
        const regionIdsToAdd = [];
        for (const region of regions) {
          // Region might be an object with a region_id (if data is from database)
          // or just a string (if data is from drupal form)
          const regionId = region.region_id || region;

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
        const batchSize = Number(Drupal.settings.gm3_region.callback_size) || 1;
        const regionBatches = [];
        let sliceStart = 0;

        while(sliceStart < regionIdsToAdd.length) {
          const sliceEnd = sliceStart + batchSize;
          regionBatches.push(
            regionIdsToAdd.slice(sliceStart, sliceEnd)
          );
          sliceStart = sliceEnd;
        }

        const regionDataUrl = Drupal.settings.gm3_region.callback;

        await Promise.all(regionBatches.map(async batch => {
          try {
            const res = await fetch(`${regionDataUrl}/${batch.join(',')}`);
            const data = await res.json();

            for (const region of data) {
              this.addRegion(region, editable);
            }
          } catch(e) {
            console.error(e);
          }
        }));

        return this;
      }

      /**
      *  Add a region object to the map
       * @param {Object} region Region object to add
       * @param {Boolean} editable True to make it editable
       */
      addRegion(region, editable) {
        const regionId = region.regionId || Object.keys(region)[0];
        const shape = region.shape || region[regionId].shape;
        const polygons = shape.type === 'MultiPolygon' ? shape.coordinates :
                         shape.type === 'Polygon' ? [shape.coordinates] : [];
        const regionParts = [];

        for (const polygon of polygons) {
          for(const points of polygon) {
            regionParts.push(this.addPolygon(points));
          }
        }

        this.countries[regionId] = this.addObject(regionParts);

        if (editable) {
          this.countries[regionId].on('contextmenu', e => {
            this.removePolygonsById(regionId);
            L.DomEvent.stopPropagation(e);
          });
        }
      }

      /**
       * Add a polygon to the map
       * @param {LatLng} points The points to use to construct the polygon
       */
      addPolygon(points) {
        const pathPoints = points.map(point => Array.isArray(points) ? L.latLng([point[1], point[0]]) : L.latLng(points));

        const polyOptions = {
          color: '#000000',
          opacity: 0.4,
          weight: 1
        };

        return L.polygon(pathPoints, polyOptions);
      }

      /**
       * Remove a region by its region ID
       * @param {string} regionId Region ID to remove
       */
      removePolygonsById(regionId) {
        // Important to remove countries[regionId] before calling removeObject,
        // as removeObject will trigger an update event, which uses the countries
        // object as its list of current selected values.
        const object = this.countries[regionId];
        delete this.countries[regionId];
        this.removeObject(object);
      }

      /**
       * Hook called by the parent gm3 object when this tool is being enabled
       * @param {LeafletMap} map Activating map
       */
      activate(map) {
        map.addControl(this.levelControl);
        this.addTeardown(() => this.levelControl.remove());

        // Add tool functionality
        super.activate(map, {
          click: e => this.selectRegion(e.latlng)
        });
      }

      /**
       * Given a point, highlight the region on the map
       * @param {LatLng} latLng The point on the map selected by the user
       */
      async selectRegion (latLng) {
        if(!this.canAddObject()) {
          return;
        }

        try {
          const polygonId = await this.getPolygonId(latLng);

          if(polygonId) {
            if (this.countries[polygonId]) {
              this.setMessage('Region already selected');
            } else {
              this.addPolygonsByIds(polygonId, true);
            }
          }
        } catch(e) {
          this.setMessage(`An error occurred while selecting the region; ${e}`, 'error');
        }
      }

      /**
       * Get the polygon coordinates for the given region, using the osm & scratchpads apis
       * @param {L.LatLng} latLng The point in the region to select
       */
      async getPolygonId(latLng) {
        const geocodeUrl = ({ lat, lng }) => `https://nominatim.openstreetmap.org/reverse?format=json&lat=${lat}&lon=${lng}`;
        const res = await fetch(geocodeUrl(latLng));
        if(!res.ok) {
          throw new Error(`Geocoder returned status ${res.status} (${res.statusText})`);
        }
        const result = await res.json();
        const regionCode = result.address ? result.address.country_code : 'UNKNOWN';
        const res2 = await fetch(`${Drupal.settings.gm3_region.callback2}/${latLng.lat}, ${latLng.lng}/${regionCode}/${this.levelControl.selectingLevel}`);
        if(!res2.ok) {
          throw new Error(`Scratchpads polygon callback returned status ${res.status} (${res.statusText})`);
        }
        return await res2.json();
      }

      /**
       * Calculates the new field value and fires the update event
       */
      getValue() {
        return Object.keys(this.countries).filter(k => this.countries[k]);
      }
    }
  }
})(jQuery);
