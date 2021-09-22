// jQuery $ thingy
(function($){

Drupal.hidenodeoptions = Drupal.hidenodeoptions || {};

Drupal.behaviors.hidenodeoptions = {
 attach: function (context, settings){
  $('.hidenodeoptions-tabs', context).toggle();
  $('.hidenodeoptions-link', context).click(function(){
   $(this).toggleClass('add-shortcut');
   $(this).toggleClass('remove-shortcut');
   $(this).siblings('.hidenodeoptions-tabs').toggle();
   if($(this).find('.hidenodeoptions-text').html() == settings.hidenodeoptions.show){
    $(this).find('.hidenodeoptions-text').html(settings.hidenodeoptions.hide);
   } else {
    $(this).find('.hidenodeoptions-text').html(settings.hidenodeoptions.show);	   
   }   
   return false;
  })
 }
}

// jQuery $ thingy
})(jQuery);