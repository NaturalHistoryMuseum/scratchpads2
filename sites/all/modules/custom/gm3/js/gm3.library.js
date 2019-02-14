(function(){
  "use strict";

  // Todo: Maybe this can actually extend L.Layer,
  // then we can just add it to the map directly
  Drupal.GM3.Library = class extends L.Evented {
    constructor(settings, listeners) {
      super();

      this.on(listeners);

      this.layerGroup = this.getLayerGroup();
      this.fire('addlayer', { layer: this.layerGroup });

      // Array of functions to call when we deactivate
      this.teardowns = [];
      this.active = false;
    }
    /**
     * Creates the layer to keep all of the library's shapes on
     */
    getLayerGroup(){
      return L.layerGroup([]);
    }
    /**
     * Returns the items added to the tool's layer group
     */
    get objects() {
      return this.layerGroup.getLayers();
    }
    /**
     * Return an object of the listeners to add when this tool is activated
     */
    getActiveListeners() {
      return {};
    }
    /**
     * Todo: Combine with above
     */
    getDefaultListeners(){
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

      // Todo:      this.GM3.google_map.setOptions({draggableCursor: 'pointer'});

      this.active = true;

      const defaultListeners = this.getDefaultListeners();

      // Merge listeners with default listeners
      listeners = Object.assign(
        defaultListeners,
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

      this.fire('deactivate');

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

      this.fire('beforeaddobject', { cancel });

      return !cancelled;
    }

    /**
     * Adds layers to the map and increases the object count by one
     * @param {L.Layer[]} layers Array of items to add to this tool's layer group
     */
    addObject(layers) {
      layers = Array.isArray(layers) ? layers : [layers];

      this.fire('addobject');

      for(const layer of layers) {
        this.layerGroup.addLayer(layer);
      }

      this.updateField();
    }

    /**
     * Removes the given objects from the map and decreases the object count
     * @param {L.Layer[]} layers
     */
    removeObject(layers) {
      for(const object of layers){
        object.remove();
      }
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
     */
    updateField(){
      const cls = id => `.${id}-${this.constructor.name}`;
      const value = this.getValue();
      this.fire('update', { cls, value });
    }
  }
})();
