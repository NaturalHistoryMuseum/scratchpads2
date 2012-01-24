(function($){
  $(document).ready(function(){
    try {
      $('a[href^="' + Drupal.settings.basePath + 'file/"]').colorbox({rel: 'gallery', fastIframe: false, data: 'colorbox'});
    } catch(err) {}
  });
})(jQuery);