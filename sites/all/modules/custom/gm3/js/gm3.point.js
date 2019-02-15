(function(){
  "use strict";

  Drupal.GM3.point = class extends Drupal.GM3.Library {
    constructor(settings) {
      super();

      this.cluster = L.markerClusterGroup({
        disableClusteringAtZoom: 12
      });
      this.addLayer(this.cluster)

      // Add points sent from server.
      if(settings.points) {
        for(const point of settings.points) {
          this.addMarker(
            L.latLng(point.latitude, point.longitude),
            point.editable,
            point.colour,
            point.title,
            point.content
          );
        }
      }
    }

    /**
     * Make sure objects get added to the cluster, not the layergroup
     */
    get objectLayer() {
      return this.cluster;
    }

    /**
     * Create a a new marker and add it to the map
     * @param {L.LatLng} latLng The coordinate to put the marker
     * @param {bool} editable Can the user edit this?
     * @param {string} colour The colour for the marker
     * @param {string} title The title for the marker
     * @param {string} content The content text for the marker's popup
     */
    addMarker(latLng, editable = true, colour, title = '', content = ''){
      if(!this.canAddObject()) {
        return;
      }

      title = title ? `${title} : ${latLng.toString()}` : '';

      const point = L.marker(
        latLng,
        {
          draggable: editable,
          title
        }
      );
      this.addObject(point);

      point.on({
        dragend: () => this.updateField(),
        click: e => {
          if (this.active) {
            this.setMessage(e.latlng.toString(), 'status', 10000);
          }
        },
        contextmenu: e => {
          if (this.active) {
            this.removeObject(point);
          }
        }
      });

      if(content) {
        // Todo: Include title?
        point.bindPopup(content, { className: 'gm3_infobubble' });
      }
    }

    /**
     * Return the object of listeners to add when the map is active
     */
    getActiveListeners(){
      return {
        click: e => {
          this.addMarker(e.latlng, true)
        }
      }
    }

    /**
     * Called when the underlying field updates (if there is one)
     * @param {string} value The field's value
     */
    setValue(value) {
      this.disableUpdates();
      const coords = value.match(/\([^)]+\)/g) || [];
      const latLngs = coords.map(
        coord => coord.match(/[+-]?[0-9]*\.?[0-9]+/g).map(parseFloat)
      );

      // Update the existing latLngs
      const len = Math.min(latLngs.length, this.objects.length);
      for(let i = 0; i < len; i++) {
        this.objects[i].setLatLng(latLngs[i]);
      }

      // Add new latLngs
      const addList = latLngs.slice(this.objects.length);
      for(const latlng of addList) {
        this.addMarker(latlng, true);
      }

      // Remove old latlngs
      const removeList = this.objects.slice(latLngs.length);
      for(const object of removeList) {
        this.removeObject(object);
      }

      this.enableUpdates();
    }

    /**
     * Set the value of the underlying field
     */
    getValue() {
      return this.objects.map(point => {
        const { lat, lng } = point.getLatLng();
        return `(${lat}, ${lng})`
      }).join('|');
    }
  }
})();
