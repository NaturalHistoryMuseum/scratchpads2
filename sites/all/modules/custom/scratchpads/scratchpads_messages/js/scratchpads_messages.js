(function($){
  $.prototype.body_resize_toolbar = function(){
    console.log(Drupal.toolbar.height());
    $('body').css('paddingTop', Drupal.toolbar.height()); 
  }
})(jQuery);