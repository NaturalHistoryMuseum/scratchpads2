(function($){
  $.prototype.body_resize_toolbar = function(){
    $(this).css('paddingTop', Drupal.toolbar.height()); 
  }
})(jQuery);
