(function(){
  "use strict";

  Drupal.GM3.Library = class extends L.Evented {
    constructor() {
      super();

      // Array of functions to call when we deactivate
      this.teardowns = [];
      this.active = false;
      this.followLines = [];
      this.followLineGetters = [];
    }

  /**
   * Creates a line that connects one point of a polygon or polyline to the mouse position
   * to show how the next click will affect the shape
   * @param {function} getCoordinate Return the coordinate to connect the followline to
   */
    createFollowLine(getCoordinate) {
      const line = L.polyline([], {
        color: '#787878',
        opacity: 1,
        weight: 2,
        interactive: false
      });

      this.followLines.push({
        line,
        getCoordinate
      });

      return line;
    }
    /**
     * Set the coordinates for the follow lines
     * @param {L.latLng} mousePosition The coordinate the mouse is at
     */
    setFollowLines(mousePosition) {
      for(const { line, getCoordinate } of this.followLines) {
        // Todo: Do we have to call set coordinate all the time?
        // Can't we rely on polyline events to know when this changes?
        const coord = getCoordinate();
        line.setLatLngs(coord ? [coord, mousePosition] : []);
      }
    }
    /**
     * Return an object of the listeners to add when this tool is activated
     */
    getActiveListeners() {
      return {};
    }
    /**
     * Called when the tool is activated
     * @param {L.Map} map The leaflet map this tool is added to
     * @param {Object} listeners Dict of listeners to add on activation
     */
    activate(map, listeners){
      if (this.active) {
        throw new Error('This map library is already active');
      }

      // Todo:      this.GM3.google_map.setOptions({draggableCursor: 'pointer'});

      this.active = true;

      // Merge listeners with default listeners
      listeners = Object.assign(
        {
          contextmenu: e => this.deactivate(),
          mousemove: this.followLines.length > 0 ? e => this.setFollowLines(e.latlng) : null
        },
        this.getActiveListeners(),
        listeners
      );

      // Add tool functionality
      map.on(listeners);

      for (const { line } of this.followLines) {
        line.setLatLngs([]);
        line.addTo(map);
      }

      // Register a function to remove the listeners
      this.addTeardown(() => map.off(listeners))
    }

    /**
     * Deactivate the tool
     */
    deactivate(){
      if (!this.active) {
        return;
      }

      this.active = false;

      this.fire('deactivate');

      // Remove polylines
      for(const { line } of this.followLines) {
        line.remove();
      }

      // Remove event listeners
      this.teardowns.forEach(t => t());
      this.teardowns = [];
    }

    /**
     * Add a function that gets called when the tool is deactivated
     * @param {function} fn The function to add
     */
    addTeardown(fn){
      this.teardowns.push(fn);
    }

    /**
     * Check whether we can add an object to the map, and then add that object if so
     * @param {function} fn The function that adds the object to the map
     */
    addObject(fn) {
      const addSuccess = () => {
        this.fire('addobject');
        this.updateField();
      }

      let cancelled = false;
      const cancel = () => cancelled = true;

      this.fire('beforeaddobject', { cancel });

      if (cancelled) {
        return false;
      }

      if(fn) {
        const rtn = fn();
        if(!rtn || !rtn.then) {
          addSuccess();
          return rtn;
        }

        return fn().then(success => {
          if (success !== false) {
            addSuccess();
          }
        });
      }

      addSuccess();

      return true;
    }

    /**
     * Remove an object from the map
     */
    removeObject() {
      this.fire('removeobject');
      this.updateField();
    }

    /**
     * Set a popup message
     */
    setPopup(layer, content, title){
      this.fire('popup', { layer, content, title })
    }

    /**
     * Set a user error/info message
     */
    setMessage(message){
      this.fire('message', { message });
    }

    /**
     * Update the underlying data field for this tool
     * @param {function} cls Return the selector for the field, given the map id
     * @param {*} value
     */
    updateField(cls, value){
      this.fire('update', { cls, value });
    }
  }
})();
