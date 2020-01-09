(function(){
  "use strict";

  // Helper for adding units to a unitless number
  const sizeUnit = n => isFinite(n) ? `${n}px` : n;

  // Helper for observing when an element's visibility changes
  const observeVisibility = (element, callback) => {
    const options = {
      root: document.documentElement
    }

    // Track visibility so we don't fire too many callbacks
    let visible = null;

    const observer = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        const newVisibility = entry.intersectionRatio > 0;

        // Ensure visibility has actually changed
        if (visible !== newVisibility) {
          visible = newVisibility;
          callback(visible);
        }
      });
    }, options);

    observer.observe(element);
  };

  Drupal.GM3 = class {
    constructor (map) {
      // Create the map tileset, which depends on the user's settings
      const tileLayer =
        map.settings.mapStyle === 'translated' ?
          L.mapboxGL({
            attribution: '<a href="https://www.maptiler.com/copyright/" target="_blank">© MapTiler</a> <a href="https://www.openstreetmap.org/copyright" target="_blank">© OpenStreetMap contributors</a>',
            accessToken: 'not-needed',
            style: 'https://api.maptiler.com/maps/basic/style.json?key=' + map.settings.mapTilerKey,
            pane: 'tilePane'
          }).on('add', event =>
            event.target._glMap.autodetectLanguage()
          )
        : map.settings.mapStyle === 'satellite' ?
          L.mapboxGL({
            attribution: '<a href="https://www.maptiler.com/copyright/" target="_blank">© MapTiler</a> <a href="https://www.openstreetmap.org/copyright" target="_blank">© OpenStreetMap contributors</a>',
            accessToken: 'not-needed',
            style: 'https://api.maptiler.com/maps/hybrid/style.json?key=' + map.settings.mapTilerKey,
            pane: 'tilePane'
          }).on('add', event =>
            event.target._glMap.autodetectLanguage()
          )
        : // mapStyle === 'default'
          L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a>',
            subdomains: ['a','b','c']
          })
        ;

      if (map instanceof Drupal.GM3) {
        return map;
      }

      const mapId = this.id = map.id;
      const settings = map.settings;

      // The maximum number of objects (points, etc) allowed on the map
      const maxObjects = parseInt(settings.max_objects || map.max_objects, 10);
      this.maxObjects = isNaN(maxObjects) ? Infinity : maxObjects;

      const leafletOptions = {
        center: [settings.center.latitude, settings.center.longitude],
        layers: [tileLayer],
        editable: true,
      };

      leafletOptions.zoom = settings.zoom = parseInt(settings.zoom, 10) || 5;

      // How far in/out user is allowed to zoom
      leafletOptions.maxZoom = parseInt(settings.maxZoom, 10) || 24;
      leafletOptions.minZoom = parseInt(settings.minZoom, 10) || 0;

      // The currently selected tool (left sidebar)
      this.activeClass = 'default';

      // The list of gm3 plugins to add to the map
      // (Polygons, points, overlays, etc)
      this.children = {};

      const mapNode = document.getElementById(mapId);

      // Set element size:
      mapNode.style.height = sizeUnit(settings.height);
      mapNode.style.width = sizeUnit(settings.width);

      // Make sure the parent wrapper is large enough
      if (mapNode.parentNode.offsetWidth > mapNode.offsetWidth) {
        mapNode.parent.style.height = mapNode.style.height;
      }

      // Prevent users from panning up or down too far
      const southWest = L.latLng(-89.98155760646617, -Infinity);
      const northEast = L.latLng(89.99346179538875, Infinity);
      leafletOptions.maxBounds = L.latLngBounds(southWest, northEast);

      // Create the actual map
      const leafletMap = L.map(mapNode, leafletOptions);

      // Add a scale bar to the map
      L.control.scale().addTo(leafletMap);

      // If the map starts as hidden it will not render properly.
      // Once it becomes visible we must re-render it.
      observeVisibility(mapNode, visible => {
        if(visible) {
          leafletMap.invalidateSize();
        }
      });

      this.leafletMap = leafletMap;
      this.mapNode = mapNode;

      // Add some event listeners to the map for the libraries we're about to add
      leafletMap.on({
        beforeaddobject: e => {
          if(!this.beforeAddObject()) { e.cancel(); }
        },
        deactivate: e => this.deactivateActiveLibrary(),
        update: ({ value, layer }) => this.updateField(layer, value),
        message: ({ message, type, delay }) => this.message(message, type, delay)
      });

      // Add libraries
      for(const id in map.libraries) {
        const LibClass = Drupal.GM3[id];
        if(LibClass) {
          const child = this.children[id] = new LibClass(map.libraries[id]);

          // Classes will extend L.Layer or L.Control, which both have addTo()
          child.addTo(this.leafletMap);

          // If there is a field for this library, watch for changes on keypress
          this.observeFieldChanges(id);
        }
      }

      const toolbar = document.getElementById(`toolbar-${mapId}`);
      this.toolbar = toolbar;

      // Add listeners
      this.addToolbarListeners(
        toolbar
      );

      // Set the active class to default
      // This is the active tool/setting in the toolbar
      this.setActiveClass('default');

      // Automatically zoom to fit all points in map, but don't zoom in further than the default zoom level
      this.autozoom({
        maxZoom: settings.zoom
      });
    }

    /**
     * If there's a form field for the given library, listen for
     * changes to the field and update the map to reflect the changes
     * @param {string} id The id of the library to set up
     */
    observeFieldChanges(id){
      const child = this.children[id];
      const field = this.getFieldForLayer(id);

      // Don't bother doing this if the layer can't handle `setValue` calls
      if(child.setValue && field) {
        let timeout;
        field.addEventListener('keyup', (e) => {
          const field = e.target;
          clearTimeout(timeout);
          child.setValue(field.value);
          // After a couple of seconds, update the field value to make sure
          // the format is correct
          timeout = setTimeout(() => field.value = child.getValue(), 2000);
        });
      }
    }

    /**
     * The number of data objects on the map
     */
    get numObjects() {
      let num = 0;
      this.leafletMap.eachLayer(layer => {
        num += layer.objects ? layer.objects.length : 0;
      })
      return num;
    }

    /**
     * Get the input field for the given map layer's key
     * @param {String} layerId The ID of the layer to get the field for
     */
    getFieldForLayer(layerId) {
      return document.querySelector(`.${this.id}-${layerId}`);
    }

    /**
     * Update a class's field, if it exists
     * @param {function} cls Generates the field's query selector given the map ID
     * @param {string} value The value to set the field to
     */
    updateField(layer, value) {
      const layerId = Object.keys(this.children).find(id => this.children[id] === layer);
      const field = this.getFieldForLayer(layerId);

      if (field){
        // If this is a combo box (e.g. region select), we have to select the individual options
        if (field.multiple && Array.isArray(value)) {
          field.value = null;
          for (const item of value) {
            const option = field.querySelector(`option[value="${item}"]`) ||
                           field.querySelector(`option[value="${item}:"]`)
            if(option) {
              option.selected = true;
            } else {
              this.message(`Could not set the field value ${item}`, 'error');
            }
          }
        } else {
          field.value = value;
        }
      }
    }

    /**
     * Adds a new Leaflet UI layer to the map
     * @param {L.Layer} object Leaflet layer to add to the leaflet map
     */
    addLayer(object) {
      object && object.addTo(this.leafletMap);
    }

    /**
     * Return true if it's possible to add another object to the map
     * Else put up a a message
     */
    beforeAddObject() {
      if(this.maxObjects === -1 || this.numObjects < this.maxObjects) {
        return true;
      } else {
        this.message(Drupal.t('Please delete an object from the map before adding another'), 'warning');
        return false;
      }
    }

    // Automatically zoom to fit all points in on the map
    async autozoom(options){
      // A rectangle containing all markers on the map
      const bounds = L.latLngBounds();
      const pendingLoads = [];

      // Find layers we know how to get the bounds of
      this.leafletMap.eachLayer(l => {
        if(l instanceof Drupal.GM3.Library) {
          bounds.extend(l.getBounds());

          // Region map actually loads async, and hasn't finished
          // looking up its polygons by the time we get here.
          // Maybe in future we can get the polgons server side so
          // we don't have to do this.
          if(l.initialLoad) {
            pendingLoads.push(l.initialLoad);
          }
        }
      });

      if(bounds.isValid()) {
        // Pad extends the area slightly to make sure all points fit comfortably
        this.leafletMap.fitBounds(bounds.pad(0.5), options);
      }

      // Wait for any loads to finish
      if (pendingLoads.length) {
        const layers = await Promise.all(pendingLoads)

        for(const layer of layers) {
          delete layer.initialLoad;

          bounds.extend(layer.getBounds());
        }

        // Yeah we do it a second time.
        if(bounds.isValid()) {
          this.leafletMap.fitBounds(bounds.pad(0.5), options);
        }
      }
    }

    // Add click handlers for the toolbar
    // The toolbar is the bar to the left of the left of the maps, with move/+polygon/+region etc
    addToolbarListeners(toolbar){
      if (!toolbar) {
        return;
      }

      // Put the listener on the toolbar element so it can catch all of the child events bubbling up
      // Add the button role to the menu items (or make the element a button)
      toolbar.addEventListener('click', ({ target }) => {
        // The data-gm3-class attribute value is in target.parentNode.dataset.gm3Class
        const gm3Class = target.dataset.gm3Class || target.parentNode.dataset.gm3Class;

        // Make sure the clicked element has the attribute
        if(gm3Class) {
          this.switchToLibrary(gm3Class);
        }
      });
    }

    /**
     * Deactivate the currently active library and activate a new one
     * @param {string} library The name of the child gm3 library to activate
     */
    switchToLibrary(library) {
      if (this.activeClass === library) {
        return;
      }

      const activeLibrary = this.children[this.activeClass];

      if (activeLibrary && activeLibrary.deactivate) {
        // Set active class to null so we ignore the next 'deactivate' event
        this.activeClass = null;
        activeLibrary.deactivate();
      }

      this.setActiveClass(library);
    }

    /**
     * Deactivate the currently active library and activate the default behaviours
     */
    deactivateActiveLibrary() {
      // If active class is null we're already in the middle of switching libraries
      if (this.activeClass !== null) {
        this.setActiveClass('default');
      }
    }

    // Sets the css class on an active toolbar button
    setActiveClass(activeClass){
      if (this.activeClass === activeClass) {
        return;
      }

      // All this toolbar stuff is not pretty, might be better as a child library
      // that extends L.Control
      const toolbar = this.toolbar;
      if (toolbar) {
        // Remove the gm3-clicked class from the existing clicked element and add it to the clicked one
        toolbar.querySelector(`.gm3-clicked`).classList.remove('gm3-clicked');
        toolbar.querySelector(`[data-gm3-class="${activeClass}"]`).parentNode.classList.add('gm3-clicked');
      }

      this.activeClass = activeClass;
      const mapNode = document.getElementById(this.id);

      if(activeClass == 'default') {
        // Set the default settings
        mapNode.classList.remove('gm3-tool-active');

      } else {
        const activeChild = this.children[activeClass];

        if(!activeChild) {
          throw new Error(`There are no tools called ${activeClass} for this map.`);
        }

        // Find the active child and call its "active" function
        if(activeChild.activate) {
          activeChild.activate(this.leafletMap);
          mapNode.classList.add('gm3-tool-active');
        }
      }
    }

    message(message, type = 'status', delay = 4000){
      // Display an alert message which disappears after a short time. This is
      // intended as an alternative to the JavaScript alert function.
      // type can be one of: "status", "warning", "error" as supported by Drupal.
      const status = document.createElement('div');
      status.classList.add('gm3_message');
      status.classList.add('messages');
      status.classList.add(type);
      status.innerHTML = message;
      this.mapNode.parentNode.prepend(status);

      setTimeout(() => status.remove(), delay);
    }

    /**
     * These functions are used by scratchpads_citethispage to make sure the generated pdf
     * is viewing the same map area as the user who clicks Create Citation
     */
    getBounds() {
      return this.leafletMap.getBounds();
    }
    setBounds(bounds) {
      return this.leafletMap.fitBounds(bounds);
    }
  }

  // Entry point. Add a map to a page. This should hopefully work via AJAX.
  Drupal.behaviors.gm3 = {
    attach(context, settings){
      // We run all the other behaviors before this one so that we've got the
      // shizzle (vertical tabs).
      for(const i in Drupal.behaviors) {
        if(i !== 'gm3' && typeof Drupal.behaviors[i].attach === 'function') {
          Drupal.behaviors[i].attach(context, settings);
        }
      }

      // Handy in case we want to debug an individual map
      Drupal.settings.gm3.mapInstances = Drupal.settings.gm3.mapInstances || {};

      // Jquery object
      if(context[0]) {
        context = context[0];
      }

      for(const mapId in Drupal.settings.gm3.maps) {
        if(context.querySelector('#' + mapId)) {
          // Create the new GM3 map object.
          Drupal.settings.gm3.mapInstances[mapId] = new Drupal.GM3(Drupal.settings.gm3.maps[mapId]);
        }
      }
    }
  };
})();
