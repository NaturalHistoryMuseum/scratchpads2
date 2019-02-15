(function(){
  "use strict";
  Drupal.GM3.polygon = class extends Drupal.GM3.Shape {
    constructor(settings) {
      // Add Polygons sent from server.
      super(settings.polygons);

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
     * Update the shape with the new latlng
     */
    updateShape(latlng){
      super.updateShape(latlng);

      const polygonPath = this.getPolygonPath();
      const pathLength = polygonPath.length;

      if(!this.polygonStart) {
        this.polygonStart = latlng;
      }
      this.polygonEnd = polygonPath[pathLength - 1];
    }
  }
})();
