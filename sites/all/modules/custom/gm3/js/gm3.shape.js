(function(){
  "use strict";

  /**
   * Class Drupal.GM3.Shape is a base class for any map tool
   * that generates multi-point shapes, requiring multiple
   * clicks before a shape is complete.
   */
  Drupal.GM3.Shape = class extends Drupal.GM3.Library {
    constructor(shapes = []) {
      super();

      // The shape that is currently being edited
      this.currentShape = null;
      this.followLine = this.createFollowLine();

      for(const shape of shapes) {
        this.addShape(shape);
      }
    }

    /**
     * Creates a line that connects one point of a polygon or polyline to the mouse position
     * to show how the next click will affect the shape
     */
    createFollowLine() {
      return L.polyline([], {
        color: '#787878',
        opacity: 1,
        weight: 2,
        interactive: false
      });
    }

    /**
     * Return the coordinates for the follow line
     * @param {L.latlng} mousePosition Where the cursor is
     */
    getFollowLineCoords(mousePosition){
      throw new Error('Please implement getFollowLineCoords in the child class');
    }

    /**
     * Gets the default event listeners on tool activate
     */
    getActiveListeners(){
      const defaultListeners = super.getActiveListeners();
      defaultListeners.mousemove = e => this.setFollowLine(e.latlng);
      return defaultListeners;
    }

    /**
     * Set the coordinates for the follow lines
     * @param {L.latLng} mousePosition The coordinate the mouse is at
     */
    setFollowLine(mousePosition) {
      // Todo: Do we have to call getCoordinate all the time?
      // Can't we rely on polyline events to know when this changes?
      this.followLine.setLatLngs(
        this.getFollowLineCoords(mousePosition) || []
      );
    }

    /**
     * Called when the tool is activated
     * @param {L.map} map The map this tool is attached to
     */
    activate(map){
      // Add tool functionality
      super.activate(map, {
        click: e => this.addLatLng(e.latlng)
      });

      const line = this.followLine;
      line.setLatLngs([]);
      line.addTo(map);

      this.addTeardown(() => this.followLine.remove());
    }

    /**
     * Called when the tool is deactivated
     */
    deactivate(){
      super.deactivate();

      if(this.currentShape) {
        if(!this.isShapeValid()) {
          this.removeObject(this.currentShape);
        }

        // Disable the editor
        this.currentShape.disableEdit();
        this.currentShape = null;
      }
    }
    /**
     * Returns a new L.Polygon object to add to the map
     * @param {L.latlng[]} latlngs The points on the shape
     * @param {*} options The options for the shape
     */
    constructShape(latlngs, options) {
      // E.g. return L.polygon(latlngs, options);
      throw new Error('Please implement constructShape');
    }
    /**
     * Adds a polygon to the map
     * @param {Array} points Latlng points to add to the polygon
     * @param {bool} editable true if the user can edit this shape
     * @param {string} content Content for the popup to add
     * @param {string} title Title for the popup to add
     */
    addShape(options = {}, canEdit = true){
      const defaults = { editable: canEdit, content: '', title: '' };
      const { editable, content, title, ...shape } = Object.assign(defaults, options);

      // Options might be an array of points, or a settings object
      // We don't know what the key is that contains the path points as it varies
      // for each library - but we can be fairly sure it's the only array property of the options
      const pathPoints = Array.isArray(options) ? options : Object.values(shape).filter(Array.isArray).flat();

      const polyOptions = {
        color: editable ? this.getLineColour() : '#000000',
        opacity: 0.4,
        weight: editable ? 4 : 1
      };

      const polygon = this.constructShape(pathPoints, polyOptions);

      // Add some listeners so users can edit polygons
      if (editable) {
        polygon.on({
          click : e => (!this.active) && e.target.enableEdit(),
          contextmenu: e => this.active && this.removeObject(polygon),
          'editable:editing': e => this.updateField()
        });
      } else if(content) {
        // We don't add a popup to an editable polygon.
        this.setPopup(polygon, content, title);
      }

      this.addObject(polygon);

      return polygon;
    }
    /**
     * Add anew latlng to the current shape
     * @param {L.LatLng} latlng The coordinate to add
     */
    addLatLng(latlng){
      if(!this.currentShape) {
        if(!this.canAddObject()) {
          return false;
        }

        // Create a new polygon and add it to the map
        this.currentShape = this.addShape();
      }

      this.updateShape(latlng);
      this.updateField();
    }

    /**
     * Add a new point to the polyline
     * @param {L.latLng} latlng The point to add
     */
    updateShape(latlng){
      this.currentShape.addLatLng(latlng);
      return true;
    }

    /**
     * Set the latlngs on a given shape
     * @param {Number} index The index of the object to set the latlngs for
     * @param {L.LatLng[]} latLngs The points to set
     */
    setShapeLatLngs(index, latLngs){
      this.objects[index].setLatLngs(latLngs);
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

      return colours[this.objects.length % 8];
    }

    /**
     * Called when the underlying field's value changes
     * @param {String} value The field value
     */
    setValue(value){
      const polygons = value.match(/\(\([^)]+\)\)/g).map(
        line => line.substring(2, line.length - 2).split(',').map(
          coord => coord.trim().split(' ').reduce((lng, lat) => ({ lat, lng }))
        )
      );

      // Update the existing latLngs
      const len = Math.min(polygons.length, this.objects.length);
      for(let i = 0; i < len; i++) {
        this.setShapeLatLngs(i, polygons[i]);
      }

      // Add new polygons
      const addList = polygons.slice(this.objects.length);
      for(const polygon of addList) {
        this.addShape(polygon, true);
      }

      // Remove old latlngs
      const removeList = this.objects.slice(polygons.length);
      for(const object of removeList) {
        this.removeObject(object);
      }
    }

    /**
     * Turn the shape into a string representation for use in form fields
     */
    getValue() {
      // Update the field.
      const polygons = [];
      for(const shape of this.objects) {
        let path = shape.getLatLngs();
        // If first element is an array, we have a multipolygon
        if(Array.isArray(path[0])) {
          path = path[0];
        }
        // Only continue if the path has three or more points.
        if(path.length > 2) {
          const closedPath = [...path, path[0]];

          polygons.push(`POLYGON ((${
            closedPath.map(({ lat, lng }) => `${lng} ${lat}`).join(',')
          }))`);

        }
      }
      return polygons.join('\n');
    }
  }
})();
