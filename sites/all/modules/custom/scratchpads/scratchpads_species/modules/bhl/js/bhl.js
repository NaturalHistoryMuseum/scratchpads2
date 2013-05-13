(function($){
  // Attach behaviours.
  Drupal.behaviors.bhl = {attach: function(context, settings){
    $('ul.bhl ul').toggle();
    $('ul.bhl>li>a').click(function(){
      if($(this).parent().find('ul:hidden').length) {
        $(this).parent().find('ul:hidden').toggle();
        return false;
      } else {
        $(this).parent().find('ul').toggle();
      }
    });
    $("#bhl-viewer").elevateZoom({zoomType: "lens", lensShape: "square", lensSize: 200});
  }}
})(jQuery);