(function(){
  "use strict";
  // Todo: This shares a lot of code with the polyline module.
  // Make them inherit the reusable code
  Drupal.GM3.polygon = class extends Drupal.GM3.Shape {
    static get name() { return 'polygon'; }
    constructor(settings, listeners) {
      // Add Polygons sent from server.
      const shapes = settings.polygons;

      super({ shapes }, listeners);

      // Create 2 editing lines that follow the user's mouse when editing a polygon
      this.polygonStart = this.polygonEnd = null;
    }
    /**
     * Generates the coordinates for the followLine
     * @param {L.latlng} mousePosition The current mouse position
     */
    getFollowLineCoords(mousePosition){
      return this.polygonStart ? [this.polygonStart, mousePosition, this.polygonEnd] : [];
    }
    /**
     * Called when the tool is deactivated
     */
    deactivate(){
      this.polygonEnd = this.polygonStart = null;
      super.deactivate();
    }
    /**
     * Return true if the shape is valid
     */
    isShapeValid(){
      return this.getPolygonPath().length > 2;
    }
    /**
     * Adds a polygon to the map
     * @param {Array} points Latlng points to add to the polygon
     */
    constructShape(points, polyOptions){
      // Make sure we map these points correctly if they're array pairs,
      // as the notation we use is inverse from the shorthand that leaflet uses
      const pathPoints = points.map(point => Array.isArray(points) ? L.latLng([point[1], point[0]]) : L.latLng(points));
      return L.polygon(pathPoints, polyOptions);
    }
    /**
     * Return the path latlongs for the current polygon
     */
    getPolygonPath(){
      const polygonPath = this.currentShape.getLatLngs();
      // If the first child of the lastLngs collection is an array,
      // we have a "multipolygon" object, and have to dive one layer down
      if (Array.isArray(polygonPath[0])) {
        return polygonPath[0];
      } else {
        return polygonPath;
      }
    }

    /**
     * Update the form field with the new value
     */
    updateField(){
      // Todo: This should really be on an event listener
      // that fires when the currentShape changes
      if(this.currentShape) {
        const polygonPath = this.getPolygonPath();
        const pathLength = polygonPath.length;

        if(pathLength >= 1) {
          this.polygonStart = polygonPath[0];
          this.polygonEnd = polygonPath[pathLength - 1];
        } else {
          this.polygonStart = null;
          this.polygonEnd = null;
        }
      }
      super.updateField();
    }
  }
})();
