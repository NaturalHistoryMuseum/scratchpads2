(function(){
  "use strict";
  // Todo: This shares a lot of code with the polyline module.
  // Make them inherit the reusable code
  Drupal.GM3.polygon = class extends Drupal.GM3.Library {
    constructor(map, settings) {
      super();

      // Create 2 editing lines that follow the user's mouse when editing a polygon
      this.polygonStart = this.polygonEnd = null;
      this.createFollowLine(() => this.polygonStart)
      this.createFollowLine(() => this.polygonEnd)

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
    /**
     * Called when the tool is activated
     * @param {L.map} map The map this tool is attached to
     */
    activate(map){
      // Add tool functionality
      super.activate(map, {
        click: e => this.addPolyPoint(e.latlng)
      });

      // Create a new polygon and add it to the map
      this.addPolygon().addTo(map);
    }
    /**
     * Called when the tool is deactivated
     */
    deactivate(){
      super.deactivate();

      // Disable the editor
      this.getLastPolygon().disableEdit();

      // Remove the polygon if it wasn't actually used
      if(this.getPolygonPath().length === 0) {
        this.polygons.pop().remove();
      }
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
        this.setPopup(polygon, content, title);
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
    /**
     * Add a new point to the polyline
     * @param {L.latLng} latlng The point to add
     */
    addPolyPoint(latlng){
      const polygon = this.getLastPolygon();
      const polyLine = this.getPolygonPath();
      // If this is the first point on the shape, try to fire addObject and see if it gets cancelled
      if (polyLine.length == 0) {
        if (!this.addObject()) {
          return;
        }
      }
      polygon.disableEdit();
      polygon.addLatLng(latlng);
      polygon.enableEdit();

      // Save the change
      if(this.updateField) {
        this.updateField();
      }
    }
    /**
     * Get the colour for the next polygon
     */
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
    /**
     * Update the form field with the new value
     */
    updateField(){
      const polygonPath = this.getPolygonPath();
      const pathLength = polygonPath.length;

      if(pathLength >= 1) {
        this.polygonStart = polygonPath[0];
        this.polygonEnd = polygonPath[pathLength - 1];
      } else {
        this.polygonStart = null;
        this.polygonEnd = null;
      }

      // Update the field.
      const newValue = this.polygons.map(
        polygon => polygon.getLatLngs().map((path) => {
          // Only continue if the path has three or more points.
          if(path.length <= 2) {
            return null;
          }

          const closedPath = [...path, path[0]];

          return `POLYGON ((${
            closedPath.map(({ lat, lng }) => `${lng} ${lat}`).join(',')
          }))`;
        }).filter(x=>x).join(' ')
      ).join('\n');
      super.updateField(id => `.${id}-polygon`, newValue );
    }
  }
})();
