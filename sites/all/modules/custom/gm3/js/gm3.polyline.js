(function(){
  "use strict";
  // Todo: This shares a lot of code with the polygon module.
  // Make them inherit the reusable code
  Drupal.GM3.polyline = class extends Drupal.GM3.Shape {
    static get name () { return 'polyline'; }
    constructor(settings, listeners) {
      console.log(settings);
      // Add Polylines sent from server.
      const shapes = settings.polylines;

      super({ shapes }, listeners);
      // Polyline object.
      // Editing lines
      this.polylineEnd = null;
    }
    /**
     * Generates the coordinates for the followLine
     * @param {L.latlng} mousePosition The current mouse position
     */
    getFollowLineCoords(mousePosition) {
      return this.polylineEnd ? [this.polylineEnd, mousePosition] : []
    }
    /**
     * Return true if the current shape has enough points
     */
    isShapeValid(){
      return this.currentShape.getLatLngs().length > 1;
    }
    /**
     * Called when the tool is deactivated
     */
    deactivate(){
      super.deactivate();

      this.polylineEnd = null;
    }
    /**
     * Add a new polyline object to the map
     * @param {L.latLng[]} points Points on the line to add
     */
    constructShape(points = [], polyOptions){
      const pathPoints = [];
      for(const point of points) {
        const pointArray = point.lat ? [point.lat, point.lng] : String(point).split(",").reverse();

        pathPoints.push(L.latLng(...pointArray));
      }

      return L.polyline(
        pathPoints,
        polyOptions
      );
    }
    /**
     * Update the form field with the new value
     */
    updateField() {
      // Todo: Move this into a change event listener on currentShape
      const line = this.currentShape && this.currentShape.getLatLngs();

      this.polylineEnd = line && line.length >= 1 ? line[line.length - 1] : null;

      super.updateField();
    }
    /**
     * Gets the text field value representation of the shapes
     */
    getValue() {
      // Update the field.
      const polygons = [];
      for(const line of this.objects) {
        const path = line.getLatLngs();
        if(path.length > 1) {
          polygons.push(`POLYGON ((${
            path.map(({ lng, lat }) => `${lng} ${lat}`).join(',')
          }))`);
        }
      }
      return polygons.join('\n');
    }
  }
})();
