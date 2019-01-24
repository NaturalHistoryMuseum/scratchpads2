(function(){
  "use strict";

  // Todo: Remove or move some of these helpers into another script
  // Then move the class out of the strict mode wrapper.

  // Generate a listener callback for use in addListenersHelper
  const makeEventDispatcher = (target, eventName) => function(event){
    target.dispatchEvent(eventName, event, this);
  }

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

  // These are the Map class event types we forward to child libraries
  const mapForwardEvents = [
    'click' ,
    'dblclick',
    'mousemove',
    'contextmenu',
    'zoom', //zoomend
    'move' //moveend
  ];

  // These are other class event types we forward to child libraries
  const miscForwardEvents = [
    'click',
    'dblclick',
    'contextmenu',
    // Todo: add move/mousemove
  ];

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

      leafletMap.on('editable:vertex:dragend', e => {
        this.dispatchEvent('')
        console.log(en, e);
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

      // Add libraries
      // Todo: refactor
      for(const id in map.libraries) {
        if(Drupal.GM3[id]) {
          this.children[id] = new Drupal.GM3[id](this, map.libraries[id]);
        }
      }

      const toolbar = document.getElementById(`toolbar-${mapId}`);

      // Add listeners
      // Todo: Refactor
      this.addToolbarListeners(
        toolbar
      );

      // Set the active class to default
      // This is the active tool/setting in the toolbar
      this.setActiveClass('default', toolbar);

      // Automatically zoom to fit all points in map
      this.autozoom(leafletMap);
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

      // Todo: Remove this from the prototype
      this.popups.push({ object: target, content });

      // When the target is clicked, open the popup
      target.addListener("click", event => {
        // Todo: Remove this from the prototype
        if(this.infoWindow) {
          this.infoWindow.close();
        }

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
          this.setActiveClass(gm3Class, toolbar);
        }
      });
    }

    // Called when the default toolbar button is selected
    // Sets the draggableCursor to pointer and removes the gm3_information block
    // Todo: Refactor references to this
    active(){
      // Todo: Set the cursor to "pointer"
      // Remove the information block (currently only used by the region module).
      const gm3Info = this.mapNode.querySelector('.gm3_information');
      if(gm3Info) {
        gm3Info.remove();
      }
    }

    // Sets the css class on an active toolbar button
    setActiveClass(activeClass, toolbar){
      // Todo: Can this toolbar stuff be split off into a toolbar module?
      if (toolbar) {
        // Remove the gm3-clicked class from the existing clicked element and add it to the clicked one
        toolbar.querySelector(`.gm3-clicked`).classList.remove('gm3-clicked');

        // Todo: Get the target from the actual event
        toolbar.querySelector(`[data-gm3-class="${activeClass}"]`).parentNode.classList.add('gm3-clicked');
      }

      this.activeClass = activeClass;

      // Let all the children add their listeners, forward events to them
      // Todo: Why are we calling this on all children?
      this.addListeners();

      if(activeClass == 'default') {
        // Set the default settings
        this.active();
      } else {
        // Find the active child and call its "active" function
        if(this.children[activeClass] && this.children[activeClass].active) {
          this.children[activeClass].active();
        }
      }
    }

    subscribeTo(eventName) {
      miscForwardEvents.push(eventName);
    }

    // Go through all the children and call addListeners and addTransferListeners
    // For some reason
    addListeners(){
      for(const id in this.children) {
        // Add transfer listeners for each library
        if(this.children[id].addTransferListeners) {
          this.children[id].addTransferListeners();
        }
        // Add listeners for each library (if they define one).
        if(this.children[id].addListeners) {
          this.children[id].addListeners();
        }
      }
      // Add listeners to the map. These will in turn execute the callbacks for
      // the currently active class (or default).
      this.addListenersHelper();
    }

    // Dispatches the event to the active library if it's not "default",
    // otherwise delegates to the first library who wants it
    dispatchEvent(eventName, event, thisValue) {
      if(this.activeClass == "default"){
        this.dispatchEventToLibraries(eventName, event, thisValue);
      } else {
        this.dispatchEventToActiveLibrary(eventName, event, thisValue);
      }
    };

    // Dispatches the given event to the currently active library
    dispatchEventToActiveLibrary(eventName, event, thisValue) {
      const activeLibrary = this.children[this.activeClass];
      if (activeLibrary.event) {
        activeLibrary.event(eventName, event, thisValue);
      }
    }

    // Dispatches the event to child libraries' event functions; stops at the first to return true
    // if none return true, the event function on this own class will handle it
    dispatchEventToLibraries(eventName, event, thisValue) {
      // Get a list of the libraries, add the main gm3 module to the end
      const libraries = Object.values(this.children).concat(this);

      // Call event handler on all of the libraries until one returns true
      libraries.some(l => (l.event && l.event(eventName, event, thisValue)));
    }

    // Sets up events on the map and forwards them to the active library
    addListenersHelper(mapObject){
      // Goes through the mapEvents or otherEvents arrays
      // if the event is not zoom
      // maps.event.clearListeners(mapObject, event);

      const map = mapObject || this.leafletMap;
      // Add additional listeners to the Map
      const eventsArray = map instanceof L.Map ? mapForwardEvents : miscForwardEvents;

      for(const eventName of eventsArray) {
        const delegateEvent = makeEventDispatcher(this, eventName);
        // Todo: Check - is there a better way?
        map.removeEventListener(eventName);
        map.addEventListener(eventName, delegateEvent);
      }
    }

    // Clears listeners and transfer listeners on children, removes handlers for event forwarding
    clearListeners(){
      // Clear listeners from the map.
      this.leafletMap.removeEventListener("click");
      this.leafletMap.removeEventListener("mousemove");
      this.leafletMap.removeEventListener("rightclick");

      for(const lib of this.children) {
        // Clear transfer listeners for each library (mostly not needed).
        if(lib.clearTransferListeners) {
          lib.clearTransferListeners();
        }
        // Clear listeners for each library (if they define one).
        if(lib.clearListeners) {
          lib.clearListeners();
        }
      }
      // Add listeners to the map. These will in turn execute the callbacks for
      // the currently active class (or default).
      this.clearListenersHelper();
    }

    // Removes handlers for forwarding events to children
    clearListenersHelper(mapObject){
      mapObject = mapObject || this.leafletMap;
      const eventsArray = map instanceof L.Map ? mapForwardEvents : miscForwardEvents;

      for(const eventName of eventsArray) {
        mapObject.removeEventListener(eventName);
      }
    }

    message(message, type, delay){
      // Display an alert message which disappears after a short time. This is
      // intended as an alternative to the JavaScript alert function.
      // type can be one of: "status", "warning", "error" as supported by Drupal.
      if(typeof type == 'undefined') {
        type = 'status';
      }
      if(typeof delay == 'undefined') {
        delay = 4000;
      }
      $('#' + this.id).parent().prepend('<div class="gm3_message messages ' + type + '">' + message + '</div>');
      $('.gm3_message').delay(delay).slideUp(1000, function(){
        $('.gm3_message').remove();
      });
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
      for(const mapId in Drupal.settings.gm3.maps) {
        if(context.getElementById(mapId)) {
          // Create the new GM3 map object.
          Drupal.settings.gm3.maps[mapId] = new Drupal.GM3(Drupal.settings.gm3.maps[mapId]);
        }
      }
    }
  };
})();
