(function($){
  $(document).ready(function(){
    $('a[href^="'+Drupal.settings.basePath+'file/"]').colorbox();
  });
})(jQuery);