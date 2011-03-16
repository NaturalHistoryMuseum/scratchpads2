(function($){
  Drupal.behaviors.remote_issues_block = {
    attach: function(context, settings){
      var timeoutid = false;
      $('#remote-issue-tab .items li').mousemove(function(){
        var parentthis = this;    
        if(timeoutid){
          window.clearTimeout(timeoutid);    
        }
        timeoutid = window.setTimeout(function(){
          $(parentthis).children().children('p').slideDown(200);
          $(parentthis).siblings().each(function(){
            $(this).children().children('p').slideUp(1000);
          });          
        }, 500);
      });
    }
  }
})(jQuery);