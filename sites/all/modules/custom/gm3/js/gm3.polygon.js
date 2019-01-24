// Tests:
// Create polygon

// Todo:
//  Make polygon editable
//  Register points being draggable
//  Check handlers for dblclick, contextmenu etc work

(function(){
  "use strict";

  Drupal.GM3.polygon = class {
    constructor(map, settings) {
      this.GM3 = map;
      // Polygon object.
      // We don't currently support geodesic shapes, mainly due to the library
      // we're using being a little buggy in its support for it. For this
      // reason,
      // please avoid loading the geometry library.
      // Todo: can we get rid of this?
      this.geodesic = false;
      // Editing lines
      // Todo: Check whether these settings apply
      this.followline1 = L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
      this.followline2 = L.polyline({geodesic: this.geodesic, clickable: false, path: [], strokeColor: '#787878', strokeOpacity: 1, strokeWeight: 2});
      // Polygons.
      this.polygons = [];
      // Add Polygons sent from server.
      if(settings.polygons) {
        for(const polygon of settings.polygons) {
          if(!polygon.polygon) {
            this.addPolygon(polygon);
          } else {
            this.addPolygon(
              polygon.polygon,
              polygon.editable,
              polygon.content || ''
            );
          }
        }
      }
      this.addTransferListeners();
    }
    active(){
      // Todo: Set cursor to pointer
      // Todo: Construct options:
      // {geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3, path: []}
      const polygon = L.polygon([]);
      polygon.on('editable:vertex:dragend', e => {
        console.log('editpoly1', e);
      });
      polygon.addTo(this.GM3.leafletMap);
      this.polygons.push(polygon);
      this.followline1.setLatLngs([]);
      this.followline2.setLatLngs([]);
      this.followline1.addTo(this.GM3.leafletMap);
      this.followline2.addTo(this.GM3.leafletMap);
    }
    addPolygon(points, editable = true, content, title){
      const pathPoints = new Array(points.length);
      for(let i = 0; i < points.length; i++) {
        pathPoints[i] = Array.isArray(points[i]) ? L.latLng([points[i][1], points[i][0]]) : L.latLng(points[i]);
        this.GM3.addLatLng(pathPoints[i]);
      }

      if(editable) {
        // We don't add a popup to an editable polygon.
        // Todo: Set options: {geodesic: this.geodesic, strokeColor: this.get_line_colour(), strokeOpacity: 0.4, strokeWeight: 3,}
        const p = L.polygon(pathPoints);
        p.addTo(this.GM3.leafletMap);
        this.polygons.push(p);
        p.on('editable:vertex:dragend', e => {
          console.log('editpoly2', e);
        });
      } else {
        // Todo: add options {geodesic: this.geodesic, map: this.GM3.google_map, strokeColor: '#000000', strokeOpacity: 0.4, strokeWeight: 1, path: pathPoints}
        const polygon = L.polygon(pathPoints);
        polygon.on('editable:vertex:dragend', e => {
          console.log('editpoly3', e);
        });
        polygon.addTo(this.GM3.leafletMap);
        // Todo: Why are we calling this?
        this.GM3.addListenersHelper(polygon);
        if(content) {
          // Add the popup also if we have content!
          this.GM3.addPopup(polygon, content, title || '');
        }
        // Return the polygon so that it can be saved elsewhere.
        return polygon;
      }
    }
    event(eventType, event, eventObject){
      // Todo: Refactor this pls
      switch(this.GM3.activeClass){
        case 'polygon':
          switch(eventType){
            case 'click':
              if(this.polygons[this.polygons.length - 1].getLatLngs().length == 0) {
                // Todo: This really ought to call a function on the parent class
                if(this.GM3.maxObjects === -1 || this.GM3.num_objects < this.GM3.maxObjects) {
                  this.GM3.numObjects++;
                } else {
                  this.GM3.message(Drupal.t('Please delete an object from the map before adding another'), 'warning');
                  break;
                }
              }
              this.polygons[this.polygons.length - 1].disableEdit();
              this.polygons[this.polygons.length - 1].addLatLng(event.latlng);
              this.polygons[this.polygons.length - 1].enableEdit();
              if(this.updateField) {
                this.updateField();
              }
              break;
            case 'mousemove':
              const lastPolygon = this.polygons[this.polygons.length - 1];
              const polygonPath = lastPolygon.getLatLngs();
              const pathLength = polygonPath.length;
              if(pathLength >= 1) {
                const startingPoint1 = polygonPath[pathLength - 1];
                const { latlng } = event;
                const followCoordinates1 = [startingPoint1, latlng];
                this.followline1.setLatLngs(followCoordinates1);
                const startingPoint2 = polygonPath[0];
                const followCoordinates2 = [startingPoint2, latlng];
                this.followline2.setLatLngs(followCoordinates2);
              }
              break;
            case 'rightclick':
              this.GM3.setActiveClass('default');
              this.followline1.setMap(null);
              this.followline2.setMap(null);
              if(this.updateField) {
                this.updateField();
              }
              break;
          }
          break;
        case 'default':
          switch(eventType){
            case 'click':
              if(eventObject.getClass && eventObject.getClass() == 'Polygon') {
                // Once clicked, stop editing other polygons
                for( const j = 0; j < this.polygons.length; j++) {
                  this.polygons[j].stopEdit();
                }
                // We need to check this object is one of ours. Else we simply
                // ignore it
                for( const i = 0; i < this.polygons.length; i++) {
                  if(eventObject == this.polygons[i]) {
                    this.polygons[i].runEdit();
                  }
                }
              } else {
                // Clicked elsewhere, stop editing.
                for( const j = 0; j < this.polygons.length; j++) {
                  this.polygons[j].stopEdit();
                }
              }
              if(this.updateField) {
                this.updateField();
              }
              break;
            case 'rightclick':
              if(eventObject.getClass && eventObject.getClass() != 'Polygon') {
                // Once clicked, stop editing other polygons
                for( const j = 0; j < this.polygons.length; j++) {
                  this.polygons[j].stopEdit();
                }
              }
              if(this.updateField) {
                this.updateField();
              }
              break;
          }
          break;
      }
    }
    addTransferListeners(){
      // What does this actually mean?
      for(const polygon of this.polygons) {
        // Todo: Why are we doing this both here and in the parent?
        this.GM3.addListenersHelper(polygon);
      }
    }
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

      return colours[this.polygons.length % 8];
    }
  }
})();
