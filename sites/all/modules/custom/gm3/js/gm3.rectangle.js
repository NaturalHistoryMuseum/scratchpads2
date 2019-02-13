(function(){
  Drupal.GM3.rectangle = class extends Drupal.GM3.Shape {
    static get name() { return 'rectangle'; }
    constructor(settings, listeners) {
      // Add Rectangles sent from server.
      const shapes = settings.rectangles;

      super({ shapes }, listeners);
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
    deactivate(){
      super.deactivate();
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
      }

      this.currentShape.setBounds([this.firstClick, latlng]);

      this.updateField();
    }
  }
})();
