(function(){
  "use strict";

  /**
   * Class Drupal.GM3.Shape is a base class for any map tool
   * that generates multi-point shapes, requiring multiple
   * clicks before a shape is complete.
   */
  Drupal.GM3.Shape = class extends Drupal.GM3.Library {
    constructor(settings, listeners) {
      super(settings, listeners);

      // The collection of polygons.
      this.currentShape = null;
      this.followLine = this.createFollowLine();

      const shapes = settings.shapes || [];
      const libName = this.constructor.name;
      for(const shape of shapes) {
        const args = shape[libName] ? [shape[libName], shape.editable, shape.content || '', shape.title || ''] : [shape];
        this.addShape(...args);
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
    getDefaultListeners(){
      const defaultListeners = super.getDefaultListeners();
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
        click: e => this.updateShape(e.latlng, map)
      });

      const line = this.followLine;
      line.setLatLngs([]);
      line.addTo(map);

      // Create a new polygon and add it to the map
      this.currentShape = this.addShape();

      this.addTeardown(() => this.followLine.remove());
    }

    /**
     * Called when the tool is deactivated
     */
    deactivate(){
      super.deactivate();

      if(this.currentShape && !this.isShapeValid()) {
        this.removeObject(this.currentShape);
      }

      // Disable the editor
      this.currentShape.disableEdit();
      this.currentShape = null;
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
    addShape(pathPoints = [], editable = true, content, title = ''){
      // To do: This breaks activation if the map is full.
      // This check should be done on first click after addShape
      if (!this.canAddObject()) {
        return false;
      }

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
     * Add a new point to the polyline
     * @param {L.latLng} latlng The point to add
     */
    updateShape(latlng){
      this.currentShape.addLatLng(latlng);

      // Save the change
      this.updateField();
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
