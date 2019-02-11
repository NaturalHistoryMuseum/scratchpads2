(function(){
  "use strict";

  Drupal.GM3.point = class extends Drupal.GM3.Library {
    constructor(map, settings) {
      super();

      // Todo: Is this neede?
      //map.on('viewreset', e => this.clusterer.repaint());

      this.points = new Array();
      this.markers = new Array();
      // FIXME - Add a way of setting this image.
      this.markerImages = []
      /*
      for(let i = 0; i < 8; i++) {
        this.markerImages[i] = L.icon({
          iconUrl: Drupal.settings.gm3.settings.images.sprite,
          iconSize: [18, 25],
          // Use class name/css with background-position set.
          //origin:  new google.maps.Point(11 + (i * 18), 0),
          iconAnchor: [9, 25]
        });
      }*/

      this.clusterer = L.markerClusterGroup({
        disableClusteringAtZoom: 12
      });
      map.addLayer(this.clusterer);

      // Add points sent from server.
      if(settings.points) {
        for(const point of settings.points) {
          // Default editable to false
          const editable = !!point.editable;
          this.addMarker(
            L.latLng(point.latitude, point.longitude),
            editable,
            point.colour,
            point.title,
            point.content
          );
        }
      }
    }

    /**
     * Remove a marker from the map
     * @param {L.Marker} point The marker to remove
     */
    removeMarker(point) {
      this.points = this.points.filter(p => p !== point);
      this.clusterer.removeLayer(point);
      point.remove();
      this.removeObject();
    }

    /**
     * Create a a new marker and add it to the map
     * @param {L.LatLng} latLng The coordinate to put the marker
     * @param {bool} editable Can the user edit this?
     * @param {string} colour The colour for the marker
     * @param {string} title The title for the marker
     * @param {string} content The content text for the marker's popup
     */
    addMarker(latLng, editable, colour, title = '', content = ''){
      this.addObject(() => {
        title = title ? `${title} : ${latLng.toString()}` : '';

        if(!colour) {
          colour = this.points.length % 8;
        }

        const point = L.marker(
          latLng,
          {
            draggable: editable,
            title
            /*, icon: this.marker_images[colour] */
          }
        );
        this.points.push(point);

        point.on('dragend', () => this.updateField());
        point.on('click', e => {
          if (this.active) {
            this.message(e.position.toString(), 'status', 10000);
          }
        });
        point.on('contextmenu', e => {
          if (this.active) {
            this.removeMarker(e.target);
          }
        });

        if(content) {
          this.setPopup(point, content, title);
        }

        this.clusterer.addLayer(point);
      });
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
      // Bit of a hack... we only do this for single-point fields at the moment
      if (this.points.length === 1) {
        const [lat, lng] = value.replace(/[()]/g, "").split(", ").map(parseFloat);
        if(isNaN(lat) || isNaN(lng)) {
          return;
        }
        this.points[0].setLatLng([lat, lng]);
        return [lat, lng];
      }
    }

    /**
     * Get the css selector for the field associated with this library, for a given map
     * @param {string} mapId The ID of the map
     */
    static getFieldSelector(mapId) {
      return `.${mapId}-point`;
    }

    /**
     * Set the value of the underlying field
     */
    updateField() {
      const newValue = this.points.map(point => {
        const { lat, lng } = point.getLatLng();
        return `(${lat}, ${lng})`
      }).join('|');

      super.updateField(this.constructor.getFieldSelector, newValue);
    }
  }
})();
