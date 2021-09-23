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
    if(typeof $('#bhl-viewer').get(0) != 'undefined') {
      if($('#bhl-viewer').get(0).naturalHeight > 1.5 * $('#bhl-viewer').height()) {
        $("#bhl-viewer").elevateZoom({zoomType: "lens", lensShape: "square", lensSize: 200});
      }
    }
  }}
})(jQuery);