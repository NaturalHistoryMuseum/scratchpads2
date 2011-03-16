(function($){
  Drupal.behaviors.remote_issues_block = {
    attach: function(context, settings){
      $('#remote-issue-tab .items li').bind('mousemove', function(){
        $(this).children().children('p').slideDown(200);
        $(this).siblings().each(function(){
          $(this).children().children('p').slideUp(1000);
        });
      });
    }
  }
})(jQuery);