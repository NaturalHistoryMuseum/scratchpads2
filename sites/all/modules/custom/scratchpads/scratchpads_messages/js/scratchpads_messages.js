(function($){
  $.prototype.body_resize_toolbar = function(){
    if(Drupal.toolbar) {
      $(this).css('paddingTop', Drupal.toolbar.height());
    }
    $(this).find('.scratchpads_messages').each(function(){
      if(!$(this).find('li').length){
        $(this).remove();
      }
    });
  }
})(jQuery);
