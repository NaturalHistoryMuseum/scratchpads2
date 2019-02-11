(function(){
  "use strict";

  Drupal.GM3.Library = class extends L.Evented {
    constructor() {
      super();

      // Array of functions to call when we deactivate
      this.teardowns = [];
      this.active = false;
    }
    activate(map, listeners){
      if (this.active) {
        throw new Error('This map library is already active');
      }

      this.active = true;

      // Merge listeners with default listeners
      listeners = Object.assign(
        {
          contextmenu: e => this.deactivate()
        },
        listeners
      );

      // Add tool functionality
      map.on(listeners);

      // Register a function to remove the listeners
      this.addTeardown(() => map.off(listeners))
    }
    deactivate(){
      if (!this.active) {
        return;
      }

      this.fire('deactivate');

      this.active = false;

      // Remove event listeners
      this.teardowns.forEach(t => t());
      this.teardowns = [];
    }
    addTeardown(fn){
      this.teardowns.push(fn);
    }
    addObject(fn) {
      const addSuccess = () => {
        this.fire('addobject');
        this.updateField();
      }

      const options = {
        cancelled: false
      };

      this.fire('beforeaddobject', options);

      if (options.cancelled) {
        return false;
      }

      if(fn) {
        return fn().then(success => {
          if (success !== false) {
            addSuccess();
          }
        });
      }

      addSuccess();

      return true;
    }
    removeObject() {
      this.fire('removeobject');
      this.updateField();
    }
    setPopup(layer, content, title){
      this.fire('popup', { layer, content, title })
    }
    setMessage(message){
      this.fire('message', { message });
    }
    updateField(cls, value){
      this.fire('update', { cls, value });
    }
  }
})();
