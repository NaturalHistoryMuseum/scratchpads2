(function($){
  // Attach behaviours.
  Drupal.behaviors.bhl = {attach: function(context, settings){
    $('ul.bhl ul').toggle();
    $('ul.bhl>li>a').click(function(){
      $(this).parent().find('ul').toggle();
      return false;
    });
  }}
})(jQuery);