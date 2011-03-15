(function($){
  Drupal.behaviors.remote_issues_block = {
    attach: function(context, settings){
      $('.items li').bind('mouseenter', function(){
        $(this).children().children('p').slideDown(500);
        $(this).siblings().each(function(){
          $(this).children().children('p').slideUp(500);
        });
      });
    }
  }
})(jQuery);
