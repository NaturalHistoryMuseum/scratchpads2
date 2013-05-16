(function($){
  Drupal.behaviors.scratchpads_messages = {attach: function(context, settings){
    $('.scratchpads_messages.tips').each(function(){
      $(this).show();
      $(this).animate({left: '100px', right: '100px', top: '80px'}, 1000)
    });
    $('.scratchpads_messages a').each(function(){
      $(this).attr('target', '_blank');
    })
  }};
  $.prototype.body_resize_toolbar = function(){
    if(Drupal.toolbar) {
      $(this).css('paddingTop', Drupal.toolbar.height());
    }
    $(this).find('.scratchpads_messages').each(function(){
      if(!$(this).find('li').length) {
        $(this).remove();
      }
    });
  }
})(jQuery);