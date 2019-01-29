// Tests:
// Create polygon

(function(){
  "use strict";

  /**
   * Creates two lines that connect the open ends of the polygon to the mouse position to show how the completed polygon will look
   */
  const createFollowLine = () => L.polyline([], {
    color: '#787878',
    opacity: 1,
    weight: 2,
    interactive: false
  });

  Drupal.GM3.polygon = class extends L.Evented {
    constructor(map, settings) {
      super();

      // Create 2 editing lines that follow the user's mouse when editing a polygon
      this.followLines = [
        createFollowLine(),
        createFollowLine()
      ];

      // Array of functions to call when we deactivate
      this.teardowns = [];

      // Add the followlines
      for (const line of this.followLines) {
        line.setLatLngs([]);
        line.addTo(map);
      }

      // The collection of polygons.
      this.polygons = [];

      // Add Polygons sent from server.
      if(settings.polygons) {
        for(const polygon of settings.polygons) {
          const args = polygon.polygon ? [polygon.polygon, polygon.editable, polygon.content, polygon.title] : [polygon];
          this.addPolygon(...args).addTo(map);
        }
      }
    }
    activate(map){
      this.active = true;

      // Create a new polygon and add it to the map
      this.addPolygon().addTo(map);

      // Add tool functionality
      this.listeners = {
        click: e => this.addPolyPoint(e.latlng),
        mousemove: e => this.setFollowLines(e.latlng || e),
        contextmenu: e => this.selfDisable()
      }

      map.on(this.listeners);

      // Register a function to remove the listeners
      this.teardowns.push(() => map.off(this.listeners))
    }
    deactivate(){
      this.active = false;

      // Remove the polygon if it wasn't actually used
      if(this.getPolygonPath().length === 0) {
        this.polygons.pop().remove();
      }

      // Remove event listeners
      this.teardowns.forEach(t => t());
      this.teardowns = [];

      // Remove polylines
      this.followLines[0].remove();
      this.followLines[1].remove();

      // Disable the editor
      this.getLastPolygon().disableEdit();
    }
    /**
     * Adds a polygon to the map
     * @param {Array} points Latlng points to add to the polygon
     * @param {bool} editable true if the user can edit this shape
     * @param {string} content Content for the popup to add
     * @param {string} title Title for the popup to add
     */
    addPolygon(points = [], editable = true, content, title = ''){
      // Make sure we map these points correctly if they're array pairs,
      // as the notation we use is inverse from the shorthand that leaflet uses
      const pathPoints = points.map(point => Array.isArray(points) ? L.latLng([point[1], point[0]]) : L.latLng(points));

      const polyOptions = {
        color: editable ? this.getLineColour() : '#000000',
        opacity: 0.4,
        weight: editable ? 4 : 1
      };

      const polygon = L.polygon(pathPoints, polyOptions);
      this.polygons.push(polygon);

      // Add some listeners so users can edit polygons
      if (editable) {
        polygon.on('click', e => {
          if(!this.active) {
            e.target.enableEdit();
          }
        });

        polygon.on('editable:editing', e => {
          this.updateField && this.updateField();
        });
      }

      if(content && !editable) {
        // We don't add a popup to an editable polygon.
        this.fire('popup', { layer: polygon, content, title })
      }

      return polygon;
    }
    getLastPolygon() {
      return this.polygons[this.polygons.length - 1];
    }
    getPolygonPath(){
      const polygonPath = this.getLastPolygon().getLatLngs();
      // If the first child of the lastLngs collection is an array,
      // we have a "multipolygon" object, and have to dive one layer down
      if (Array.isArray(polygonPath[0])) {
        return polygonPath[0];
      } else {
        return polygonPath;
      }
    }
    addPolyPoint(latlng){
      const polygon = this.getLastPolygon();
      const polyLine = this.getPolygonPath();
      // If this is the first point on the shape, try to fire addObject and see if it gets cancelled
      if (polyLine.length == 0) {
        const options = {
          cancelled: false
        };
        this.fire('addobject', options);

        if (options.cancelled) {
          return
        }
      }
      polygon.disableEdit();
      polygon.addLatLng(latlng);
      polygon.enableEdit();

      // Disable the follow lines
      this.followLines[0].setLatLngs([]);
      this.followLines[1].setLatLngs([]);

      // Save the change
      if(this.updateField) {
        this.updateField();
      }
    }
    setFollowLines(mousePosition) {
      const polygonPath = this.getPolygonPath();
      const pathLength = polygonPath.length;

      if(pathLength >= 1) {
        const polygonStart = polygonPath[0];
        const polygonEnd = polygonPath[pathLength - 1];

        this.followLines[0].setLatLngs([polygonEnd, mousePosition]);
        this.followLines[1].setLatLngs([mousePosition, polygonStart]);
      }
    }
    selfDisable(){
      // Todo: Fire this as an event
      this.fire('deactivate');
    }
    getLineColour(){
      const colours = [
        '#ff0000',
        '#00ff00',
        '#0000ff',
        '#ffff00',
        '#ff00ff',
        '#00ffff',
        '#000000',
        '#ffffff'
      ];

      return colours[this.polygons.length % 8];
    }
  }
})();
