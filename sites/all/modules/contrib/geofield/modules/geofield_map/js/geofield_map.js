(function ($) {
  Drupal.behaviors.geofieldMap = {
    attach: function(context) {
      var settings = Drupal.settings.geofieldMap;
      
      var myOptions = {
        zoom: 8,
        mapTypeId: google.maps.MapTypeId.ROADMAP
      };

      $('.geofieldMap:not(.processed)').each(function(index, element) {
        var data = undefined;
        var pointCount = 0;
        for (var i in settings) {
          if (settings[i].map_id == $(element).attr('id')) {
            data = settings[i].data;
            break;
          }
        }

        if (data != undefined) {
          var markers = [];

          var map = new google.maps.Map(document.getElementById($(element).attr('id')), myOptions);
          
          var range = new google.maps.LatLngBounds();
      
          var infowindow = new google.maps.InfoWindow({
            content: ''
          });
          
          for (var i in data) {
            switch (data[i].type) {
              case 'point':
                var point = new google.maps.LatLng(data[i].points[0]['lat'], data[i].points[0]['lon']);
                range.extend(point);
                pointCount++;
                
                var marker = new google.maps.Marker({
                  position: point,
                  map: map,
                  title: "test"
                });
      
                if (data[i].icon != undefined) {
                  marker.setIcon(data[i].icon);
                }
                marker.setValues({'data_id': i});
            
                google.maps.event.addListener(marker, 'click', function() {
                  if (data[this.data_id].points[0].text) {
                    infowindow.setContent(data[this.data_id].points[0].text);
                    infowindow.open(map, this);
                  }
                });
                
              break;
              case 'linestring':
                var linestring = [];
                for (var j in data[i].points) {
                  var point = new google.maps.LatLng(data[i].points[j]['lat'], data[i].points[j]['lon']);
                  range.extend(point);
                  pointCount++;
                  linestring.push(point);
                }
                var linestringObject = new google.maps.Polyline({
                  path: linestring
                });

                linestringObject.setMap(map);
              break;
              case 'polygon':
                var polygon = [];
                for (var j in data[i].points) {
                  var point = new google.maps.LatLng(data[i].points[j]['lat'], data[i].points[j]['lon']);
                  range.extend(point);
                  pointCount++;
                  polygon.push(point);
                }
                var polygonObject = new google.maps.Polygon({
                  paths: polygon
                });

                polygonObject.setMap(map);
              break;
            }
          }
          
          if (pointCount == 0) {
            
          }
          else if (pointCount > 1) {
            map.fitBounds(range);
          } else {
            map.setCenter(range.getCenter());
          }
        }
        
        $(element).addClass('processed');
      });
    }
  }
})(jQuery);
