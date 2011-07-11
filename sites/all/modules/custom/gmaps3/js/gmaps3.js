(function($){
  Drupal.gmaps3 = new Object;
  Drupal.gmaps3.maps = new Array();
  Drupal.behaviors.gmaps3 = {
    attach: function(context, settings){
      console.log('There');
      $('div.gmaps3').each(function(){
        Drupal.gmaps3.maps[Drupal.gmaps3.maps.length] = $(this).get(0);
        $(this).css('height', '500px');
        var myLatlng = new google.maps.LatLng(51.397, 0);
        var myOptions = {
          zoom: 8,
          center: myLatlng,
          mapTypeId: google.maps.MapTypeId.HYBRID
        }
        var map = new google.maps.Map($(this).get(0), myOptions);
      });
    }
  }
})(jQuery);