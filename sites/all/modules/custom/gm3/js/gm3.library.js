(function(){
  "use strict";

  /**
   * Any map tool that can hold data objects (points, polygons, etc)
   */
  Drupal.GM3.Library = class extends L.LayerGroup {
    constructor(options = {}) {
      super(Object.assign({ interactive: true }, options));

      // Array of functions to call when we deactivate
      this.teardowns = [];
      this.active = false;
      this.updatesDisabled = false;
    }
    /**
     * Returns the items added to the tool's layer group
     */
    get objects() {
      return this.objectLayer.getLayers();
    }
    /**
     * Get the layer that should contain all of the data objects
     */
    get objectLayer(){
      return this;
    }
    /**
     * Returns a latLngBounds object that contains all of
     * the objects on the layer
     */
    getBounds() {
      const bounds = L.latLngBounds([]);

      for (const object of this.objects) {
        const b = object instanceof L.Marker ? object.getLatLng() :
                  object instanceof L.Polyline ? object.getBounds() : null;

        if (b) {
          bounds.extend(b);
        }
      }

      return bounds;
    }
    /**
     * Called when this tool is added to a map.
     * Make sure we can propagate events to the map
     * @param {L.Map} map The map this tool is added to
     */
    onAdd(map){
      super.onAdd(map);
      this.addEventParent(map);
    }
    /**
     * Called when this tool is removed from a map.
     * @param {L.Map} map The map this tool is removed from
     */
    onRemove(map){
      super.onRemove(map);
      this.removeEventParent(map);
    }
    /**
     * Return an object of the listeners to add when this tool is activated
     */
    getActiveListeners() {
      return {
        contextmenu: e => this.deactivate()
      };
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

      this.active = true;

      // Merge listeners with default listeners
      listeners = Object.assign(
        this.getActiveListeners(),
        listeners
      );

      // Add tool functionality
      map.on(listeners);

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

      this.fire('deactivate', {}, true);

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
     * Returns true if it's possible to add another object to the map
     */
    canAddObject() {
      let cancelled = false;
      const cancel = () => cancelled = true;

      this.fire('beforeaddobject', { cancel }, true);

      return !cancelled;
    }

    /**
     * Adds layers to the map and increases the object count by one
     * @param {L.Layer[]} layers Array of items to add to this tool's layer group
     */
    addObject(layers) {
      layers = Array.isArray(layers) ? layers : [layers];

      for(const layer of layers) {
        this.objectLayer.addLayer(layer);
      }

      this.updateField();
    }

    /**
     * Removes the given objects from the map and decreases the object count
     * @param {L.Layer[]} layers
     */
    removeObject(layers) {
      layers = Array.isArray(layers) ? layers : [layers];
      for(const object of layers){
        this.objectLayer.removeLayer(object);
      }
      this.updateField();
    }

    /**
     * Set a user error/info message
     */
    setMessage(message){
      this.fire('message', { message }, true);
    }

    /**
     * Update the underlying data field for this tool
     */
    updateField(){
      if(this.updatesDisabled) return;

      const value = this.getValue();
      this.fire('update', { value }, true);
    }

    /**
     * Stop firing the update events
     */
    disableUpdates(){
      this.updatesDisabled = true;
    }
    /**
     * Start firing the update events
     */
    enableUpdates(){
      this.updatesDisabled = false;
    }
  }
})();
