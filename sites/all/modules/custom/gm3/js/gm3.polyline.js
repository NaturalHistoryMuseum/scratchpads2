(function(){
  "use strict";
  // Todo: This shares a lot of code with the polygon module.
  // Make them inherit the reusable code
  Drupal.GM3.polyline = class extends Drupal.GM3.Library {
    constructor(map, settings) {
      super();
      // Polyline object.
      // Editing lines
      this.polylineEnd = null;
      this.createFollowLine(() => this.polylineEnd);

      // Polylines.
      this.polylines = [];
      // Add Polylines sent from server.
      if(settings.polylines) {
        for(const polyline of settings.polylines) {
          const cfg = polyline.polyline ? [polyline.polyline, polyline.editable, polyline.content || ''] : [polyline];
          this.addPolyline(...cfg).addTo(map);
        }
      }
    }
    /**
     * Called when the tool is activated
     * @param {L.map} map The map this tool is attached to
     */
    activate(map){
      super.activate(map, {
        click: e => this.addPolyPoint(e.latlng)
      });

      const polyline = L.polyline([], {
        color: this.getLineColour(),
        opacity: 0.4,
        weight: 3
      });
      polyline.addTo(map);

      this.polylines.push(polyline);
    }
    /**
     * Called when the tool is deactivated
     */
    deactivate(){
      super.deactivate();

      // Disable the editor
      const activePolyline = this.polylines[this.polylines.length - 1];
      activePolyline.disableEdit();

      // Remove the polygon if it wasn't actually used
      if(activePolyline.getLatLngs().length === 0) {
        this.polylines.pop().remove();
      }
    }
    /**
     * Add a new polyline object to the map
     * @param {L.latLng[]} points Points on the line to add
     * @param {Bool} editable Can the user edit this line?
     * @param {string} content Content for the popup tooltip
     * @param {string} title Title from the popup tooltip
     */
    addPolyline(points, editable=true, content = '', title = ''){
      // Todo: Refactor this class to share common code with polygon
      const pathPoints = [];
      for(const point of points) {
        const pointArray = point.lat ? [point.lat, point.lng] : String(point).split(",").reverse();

        pathPoints.push(L.latLng(...pointArray));
      }

      const polyline = L.polyline(
        pathPoints,
        {
          color: editable ? this.getLineColour() : '#000000',
          opacity: 0.4,
          weight: editable ? 3 : 1,
        }
      );
      this.polylines.push(polyline);

      // Add some listeners so users can edit polylines
      if (editable) {
        polyline.on('click', e => {
          if(!this.active) {
            e.target.enableEdit();
          }
        });

        polyline.on('editable:editing', e => {
          this.updateField();
        });
      }

      // We don't add a popup to an editable polyline.
      if(!editable && content) {
        this.setPopup(polyline, content, title);
      }
      // Return the polyline so that it can be saved elsewhere.
      return polyline;
    }
    /**
     * Add a new point to the polyline
     * @param {L.latLng} latlng The point to add
     */
    addPolyPoint(latlng){
      const line = this.polylines[this.polylines.length - 1];
      if(line.getLatLngs().length < 1) {
        if(!this.addObject()){
          return;
        }
      }

      line.disableEdit();
      line.addLatLng(latlng);
      line.enableEdit();

      this.updateField();
    }
    /**
     * Get the colour for the next polygon
     */
    getLineColour(){
      return [
        '#ff0000',
        '#00ff00',
        '#0000ff',
        '#ffff00',
        '#ff00ff',
        '#00ffff',
        '#000000',
        '#ffffff'
      ][this.polylines.length % 8];
    }
    /**
     * Update the form field with the new value
     */
    updateField() {
      const line = this.polylines[this.polylines.length - 1].getLatLngs();

      this.polylineEnd = line.length >= 1 ? line[line.length - 1] : null;

      // Update the field.
      const polygons = [];
      for(const line of this.polylines) {
        const path = line.getLatLngs();
        if(path.length > 1) {
          polygons.push(`POLYGON ((${
            path.map(({ lng, lat }) => `${lng} ${lat}`).join(',')
          }))`);
        }
      }
      super.updateField(id => `.${id}-polyline`, polygons.join('\n'));
    }
  }
})();
