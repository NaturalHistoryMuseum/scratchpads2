/**
 * Colorbox JS
 */
(function($){
  Drupal.behaviors.ScratchpadsColorBox = {attach: function(context){
    var pathparts = $(location).attr('pathname').split('/');
    if(pathparts[pathparts.length-1]!='delete'){
      try {
        $('a[href^="' + Drupal.settings.basePath + 'file/"]', context).each(function(){
          if(!$(this).attr('href').match('file/[0-9]*/(.*)') && !$(this).attr('href').match('file/add') && $(this).colorbox) {
            $(this).attr('href', $(this).attr('href').replace(Drupal.settings.basePath + 'file/', Drupal.settings.basePath + 'file-colorboxed/'))
            $(this).colorbox($.extend({rel: 'gallery'}, Drupal.settings.colorbox));
          }
        });
      } catch(err) {}
    }
  }}
})(jQuery);