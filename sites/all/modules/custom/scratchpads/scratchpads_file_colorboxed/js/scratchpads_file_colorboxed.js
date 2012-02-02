/**
 * Colorbox JS
 */
(function ($) {
  Drupal.behaviors.ScratchpadsColorBox = {
   attach: function(context) {     
     try {
       $('a[href^="' + Drupal.settings.basePath + 'file/"]', context).each(function(){
         if(!$(this).attr('href').match('/file/[0-9]*/(.*)') && $(this).colorbox){
           $(this).attr('href', $(this).attr('href').replace(Drupal.settings.basePath + 'file/', Drupal.settings.basePath + 'file-colorboxed/'))
           $(this).colorbox({rel: 'gallery'});
         }
       });
     } catch(err) {}
   }
  }
})(jQuery);