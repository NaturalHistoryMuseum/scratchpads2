(function($){
  Drupal.GM3.filter_helper = function(map){
    google.maps.event.addListener(map.google_map, "zoom_changed", function(){
      $('.gm3-generator-zoom').val(map.google_map.getZoom());
    });
    google.maps.event.addListener(map.google_map, "center_changed", function(){
      $('.gm3-generator-zoom').val(map.google_map.getCenter().toString());
    });
    google.maps.event.addListener(map.google_map, "drag", function(){
      $('.gm3-generator-center').val(map.google_map.getCenter().toString());
    });
    google.maps.event.addListener(map.google_map, "maptypeid_changed", function(){
      $('.gm3-generator-maptypeid').val(map.google_map.getMapTypeId());
    });
  }
})(jQuery);