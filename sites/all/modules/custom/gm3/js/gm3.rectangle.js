(function(){
  Drupal.GM3.rectangle = class extends Drupal.GM3.Shape {
    constructor(settings) {
      // Add Rectangles sent from server.

      super(settings.rectangles);
      this.firstClick = null;
    }
    /**
     * Adds a polygon to the map
     * @param {Array} points Latlng points to add to the polygon
     */
    constructShape(points = [[0,0]], options) {
      // Make sure we map these points correctly if they're array pairs,
      // as the notation we use is inverse from the shorthand that leaflet uses
      const pathPoints = points.map(point => Array.isArray(points) ? L.latLng([point[1], point[0]]) : L.latLng(points));

      if(pathPoints.length === 0) {
        pathPoints.push([0,0]);
      }

      // Rectangle shape takes a bounds object, rather than latlongs
      return L.rectangle(L.latLngBounds(pathPoints), options);
    }
    /**
     * Called when the tool is deactivated
     */
    completeShape(){
      super.completeShape();
      this.firstClick = null;
    }
    /**
     * Returns true if the rectangle is bigger than 0,0,0,0
     */
    isShapeValid(){
      return !this.currentShape.getBounds().equals([[0,0], [0,0]]);
    }
    /**
     * Generates the coordinates for the followLine
     * @param {L.latlng} mousePosition The current mouse position
     */
    getFollowLineCoords(latlng){
      return this.firstClick && [
        this.firstClick,
        [this.firstClick.lat, latlng.lng],
        latlng,
        [latlng.lat, this.firstClick.lng],
        this.firstClick
      ];
    }
    /**
     * Updates the rectangle shape to wherever the user clicked
     * @param {L.latlng} latlng The point on the map that was clicked
     */
    updateShape(latlng){
      if(!this.firstClick) {
        this.firstClick = latlng;
        return;
      }

      this.currentShape.setBounds([this.firstClick, latlng]);
      this.completeShape();
    }

    /**
     * Override base setShapeLatLngs so the rectangle continues to be rectangular
     * @param {Number} index The index of the object to set the latlngs for
     * @param {L.LatLng[]} latLngs The points to set
     */
    setShapeLatLngs(index, latLngs){
      this.objects[index].setBounds(L.latLngBounds(latLngs));
    }
  }
})();
