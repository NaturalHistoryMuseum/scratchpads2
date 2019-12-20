(function(){
  "use strict";

  /**
   * Create a tabs element given an array of { content, title } objects
   * @param {Array} content Array of page objects of the form { content, title }
   * @returns {HTMLElement} Tabset element with given pages
   */
  function makePagedContent(content){
    // Element is a div containing 1. list of tabs, 2. current page
    const pagedContent = document.createElement('div');
    const contentArea = document.createElement('div');
    const tabList = document.createElement('div');

    pagedContent.appendChild(tabList);
    pagedContent.appendChild(contentArea);

    // Add all of the tabs
    for(let index = 1, page = content[0]; index <= content.length; page = content[index++]) {
      const tabButton = document.createElement('button');
      tabButton.classList.add('gm3-info-tab');
      tabButton.innerText = page.title || index;

      // On tab click, replace the contents of the `content` div
      tabButton.addEventListener('click', () => {
        contentArea.innerHTML = page.content;

        // Remove the active class from the active tab and set it on the selected tab
        if(tabList.activeTab){
          tabList.activeTab.classList.remove('gm3-info-tab-active');
        }
        tabButton.classList.add('gm3-info-tab-active');

        // Save active tab reference for next click
        tabList.activeTab = tabButton;
      });

      tabList.appendChild(tabButton);
    }

    // Activate the first page
    tabList.childNodes[0].click();

    return pagedContent;
  }

  // Define some different coloured markers
  const colours = [
    'blue',
    'aqua',
    'brown',
    'gold',
    'green',
    'olive',
    'orange',
    'purple',
    'red',
    'violet',
    'yellow'
  ];
  // Use grey and black as special secret colours
  colours[-1] = 'grey';
  colours[-2] = 'black';

  Drupal.GM3.point = class extends Drupal.GM3.Library {
    constructor(settings) {
      super();

      const options = {
        disableClusteringAtZoom: 12,
        maxClusterRadius: settings.enableClustering ? 35 : 0
      };

      this.cluster = L.markerClusterGroup(options);
      this.addLayer(this.cluster)

      // Add points sent from server.
      if(settings.points) {
        for(const point of settings.points) {
          this.addMarker(
            L.latLng(point.latitude, point.longitude),
            'editable' in point ? point.editable : settings.editable,
            point.colour,
            point.title,
            point.content
          );
        }
      }
    }

    /**
     * Make sure objects get added to the cluster, not the layergroup
     */
    get objectLayer() {
      return this.cluster;
    }

    /**
     * Create a a new marker and add it to the map
     * @param {L.LatLng} latLng The coordinate to put the marker
     * @param {bool} editable Can the user edit this?
     * @param {string} colour The colour for the marker
     * @param {string} title The title for the marker
     * @param {string} content The content text for the marker's popup;
                               might be an array of { title, content } objects
     */
    addMarker(latLng, editable = true, colour=0, title = '', content = ''){
      if(!this.canAddObject()) {
        return;
      }

      title = title ? `${title} : ${latLng.toString()}` : '';

      const point = L.marker(
        latLng,
        {
          draggable: editable,
          title,
          icon: new L.Icon.Default({
            iconUrl: `${colours[colour||0]}.png`,
            imagePath: Drupal.settings.gm3.markerDirectory
          })
        }
      );
      this.addObject(point);

      point.on({
        // If the point is moved, save the new location
        dragend: () => this.updateField(),

        // If the point is clicked during edit mode, show the location
        click: e => {
          if (this.active) {
            this.setMessage(e.latlng.toString(), 'status', 10000);
          }
        },

        // Right-click on the point during edit mode deletes the point
        contextmenu: e => {
          if (this.active) {
            this.removeObject(point);
          }
        }
      });

      const popupContent = this.getPopupContent(title, content);

      if(popupContent) {
        point.bindPopup(popupContent, { className: 'gm3_infobubble' });
      }
    }

    /**
     * Generate the content to be bound to a marker popup.
     *
     * @param {string} title Title of the popup
     * @param {string} content Body of the popup
     */
    getPopupContent(title, content) {
      if(!content) {
        if(!title) {
          return;
        }
        content = title;
        title = null;
      }

      if(Array.isArray(content)) {
        if(content.length > 1) {
          return makePagedContent(content);
        }

        title = content[0].title;
        content = content[0].content;
      }

      if(title) {
        content = `<h3>${title}</h3>\n${content}`;
      }

      return content;
    }

    /**
     * Return the object of listeners to add when the map is active
     */
    getActiveListeners(){
      return {
        click: e => {
          this.addMarker(e.latlng, true)
        }
      }
    }

    /**
     * Called when the underlying field updates (if there is one)
     * @param {string} value The field's value
     */
    setValue(value) {
      this.disableUpdates();
      const coords = value.match(/\([^)]+\)/g) || [];
      const latLngs = coords.map(
        coord => coord.match(/[+-]?[0-9]*\.?[0-9]+/g).map(parseFloat)
      );

      // Update the existing latLngs
      const len = Math.min(latLngs.length, this.objects.length);
      for(let i = 0; i < len; i++) {
        this.objects[i].setLatLng(latLngs[i]);
      }

      // Add new latLngs
      const addList = latLngs.slice(this.objects.length);
      for(const latlng of addList) {
        this.addMarker(latlng, true);
      }

      // Remove old latlngs
      const removeList = this.objects.slice(latLngs.length);
      for(const object of removeList) {
        this.removeObject(object);
      }

      this.enableUpdates();
    }

    /**
     * Set the value of the underlying field
     */
    getValue() {
      return this.objects.map(point => {
        const { lat, lng } = point.getLatLng();
        return `(${lat}, ${lng})`
      }).join('|');
    }
  }
})();
