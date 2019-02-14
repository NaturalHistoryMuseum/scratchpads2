(function(){
  "use strict";

  // Todo: Remove or move some of these helpers into another script
  // Then move the class out of the strict mode wrapper.

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

  // Create the OSM tileset
  const osmTileLayer = L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a>',
      subdomains: ['a','b','c']
  });

  Drupal.GM3 = class {
    constructor (map) {
      if (map instanceof Drupal.GM3) {
        return map;
      }

      // Todo: Rename these in php code
      const {
        id: mapId,
        settings,
        minZoom
      } = map;

      // Todo: Remove this
      this.id = mapId;

      // The maximum number of objects (points, etc) allowed on the map
      const maxObjects = parseInt(map.max_objects, 10);
      this.maxObjects = isNaN(maxObjects) ? Infinity : maxObjects;

      // The current number of objects on the map
      this.numObjects = 0;

      // How far out user is allowed to zoom
      this.minZoom = parseInt(minZoom, 10);

      // Todo: rename this to something better
      this.activeClass = 'default';
      // Todo: rename this to something better
      this.children = {};

      // Collection of popups currently on the map
      this.popups = [];

      // The instance of the bubble class
      this.infoBubble = null;

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
      const maxBounds = L.latLngBounds(southWest, northEast);

      // Create the actual map
      const leafletMap = L.map(mapNode, {
        center: [settings.center.latitude, settings.center.longitude],
        zoom: settings.zoom,
        layers: [osmTileLayer],
        maxBounds,
        editable: true
      });

      // If the map starts as hidden it will not render properly.
      // Once it becomes visible we must re-render it.
      observeVisibility(mapNode, visible => {
        if(visible) {
          leafletMap.invalidateSize();
        }
      });

      // A rectangle containing all markers that we can add to and reference later
      this.coverageArea = L.latLngBounds();

      this.leafletMap = leafletMap;
      this.mapNode = mapNode;

      // Any time something gets added to the map, add that item to the zoom area
      leafletMap.on('layeradd', e => {
        this.addLatLng(e.layer);
      });

      // Add libraries
      // Todo: refactor
      for(const id in map.libraries) {
        const LibClass = Drupal.GM3[id];
        if(LibClass) {
          const child = this.children[id] = new LibClass(
             map.libraries[id],
            {
              addlayer: e => this.addLayer(e.layer),
              // Todo: Use L.DomObject.preventDefault(e) instead?
              beforeaddobject: e => {
                if(!this.beforeAddObject()) { e.cancel(); }
              },
              addobject: e => this.addObject(),
              removeobject: e => this.removeObject(),
              deactivate: e => this.deactivateActiveLibrary(),
              popup: ({ layer, content, title }) => this.addPopup(layer, content, title || ''),
              update: ({ cls, value }) => this.updateField(cls, value),
              message: ({ message }) => this.message(message)
            }
          );

          // Todo: can we just use `id` instead of requiring `LibClass` to have `name` set?
          const field = document.querySelector(`.${id}-${LibClass.name}`);
          if(field) {
            field.addEventListener('keyup', (e) => {
              const position = child.setValue && child.setValue(e.target.value);
              if (position) {
                this.addLatLng(L.latLng(position));
                this.autozoom();
              }
            });
          }
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

      // Automatically zoom to fit all points in map
      this.autozoom(leafletMap);
    }

    /**
     * Update a class's field, if it exists
     * @param {function} cls Generates the field's query selector given the map ID
     * @param {string} value The value to set the field to
     */
    updateField(cls, value) {
      const field = document.querySelector(cls(this.id));

      if (field){
        if (field.multiple && Array.isArray(value)) {
          field.value = null;
          for (const item of value) {
            const option = field.querySelector(`option[value="${item}"]`) ||
                           field.querySelector(`option[value="${item}:"]`) // Fixme: Hack to make region selection work properly;
            if(option) {
              option.selected = true;
            }
            // Todo: Error if there's no option?
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

    /**
     * Increase the count of objects on the map by 1
     */
    addObject() {
      this.numObjects++;
    }

    /**
     * Decrease the count of objects on the map by 1
     */
    removeObject() {
      this.numObjects--;
    }

    // Automatically zoom to fit all points in on the map
    autozoom(){
      if(this.coverageArea.isValid()) {
        // Pad extends the area slightly to make sure all points fit comfortably
        const bounds = this.coverageArea.pad(0.5);
        this.leafletMap.fitBounds(bounds);
      }
    }

    // Add a new coörd point to the coverage area
    addLatLng(latLng){
      latLng = Array.isArray(latLng) ? L.latLngBounds(latLng) :
               latLng.getLatLng ? latLng.getLatLng() :
               latLng.getLatLngs ? latLng.getLatLngs() :
               latLng;

      // Todo: Make sure the coord is within bounds/wraps correctly?
      this.coverageArea.extend(latLng);
    }

    // Add a tooltip/popup
    // Target is the object that, when clicked on, launches the popup
    // Content is a string containing content to display OR
    // an array of { title, content }, each of which gets added as a separate tab
    // Todo: Can this be refactored to make it better?
    //       maybe into a child library?
    addPopup(target, content){
      // There appears to be a small bug with the infobubble code that calculates
      // the height/width of the content before it is added as a child of the
      // "backgroundClassName" resulting in incorrect results.
      if(Array.isArray(content)) {
        content = content.map(content => `<div class="gm3_infobubble">${content}</div>`);
      } else {
        content = `<div class="gm3_infobubble">${content}</div>`;
      }

      target.bindPopup(content);

      // Todo: Remove this from the prototype
      this.popups.push({ object: target, content });

      // Todo: Match the infobubble styles
      // Todo: Add tabbed infobubble
      /*
        // Todo: Make this work with leaflet
        const infoWindow = new InfoBubble({
          map: this.leafletMap,
          position: event.latlng,
          disableAutoPan: true,
          borderRadius: 4,
          borderWidth: 2,
          backgroundColor: '#f5f5f5',
          borderColor: '#6261d8',
          arrowStyle: 0
        });

        const infoBubbleClass = "gm3_infobubble"

        if(Array.isArray(content)) {
          for(const page of content) {
            infoWindow.addTab(page.title, `<div class="${infoBubbleClass}">${page.content}</div>`);
          }
        } else {
          infoWindow.setContent(`<div class="${infoBubbleClass}">${content}</div>`);
        }

        infoWindow.open();

        // Todo: Remove from prototype
        this.infoWindow = infoWindow;
      });
      */
    }

    // Add click handlers for the toolbar
    // The toolbar is the bar to the left of the left of the maps, with move/+polygon/+region etc
    addToolbarListeners(toolbar){
      if (!toolbar) {
        return;
      }

      // Put the listener on the toolbar element so it can catch all of the child events bubbling up
      // Todo: Add the button role to the menu items (or make the element a button)
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

      // Todo: Can this toolbar stuff be split off into a toolbar module?
      const toolbar = this.toolbar;
      if (toolbar) {
        // Remove the gm3-clicked class from the existing clicked element and add it to the clicked one
        toolbar.querySelector(`.gm3-clicked`).classList.remove('gm3-clicked');

        // Todo: Get the target from the actual event
        toolbar.querySelector(`[data-gm3-class="${activeClass}"]`).parentNode.classList.add('gm3-clicked');
      }

      this.activeClass = activeClass;

      if(activeClass == 'default') {
        // Set the default settings
        // Todo: Set the cursor to "pointer"
      } else {
        const activeChild = this.children[activeClass];
        // Find the active child and call its "active" function
        if(activeChild.activate) {
          activeChild.activate(this.leafletMap);
        }
      }
    }

    message(message, type = 'status', delay = 4000){
      // Todo: clean this up
      // Display an alert message which disappears after a short time. This is
      // intended as an alternative to the JavaScript alert function.
      // type can be one of: "status", "warning", "error" as supported by Drupal.
      const status = document.createElement('div');
      status.classList.add('gm3_message');
      status.classList.add('messages');
      status.classList.add(type);
      status.innerHTML = message;
      this.mapNode.parentNode.prepend(status);
      // Todo: Add slideup animation 1s on remove
      setTimeout(() => status.remove(), delay);
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
      Drupal.settings.gm3.mapInstances = {};

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
