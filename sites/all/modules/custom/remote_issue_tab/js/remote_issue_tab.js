(function($){
  Drupal.behaviors.remote_issues_block = {
    attach: function(context, settings){
      var timeoutid = false;
      $('#remote-issue-tab .items>ul>li', context).mousemove(function(){
        var parentthis = this;    
        if(timeoutid){
          window.clearTimeout(timeoutid);    
        }
        timeoutid = window.setTimeout(function(){
          $(parentthis).children().children('.remote_issue_item').slideDown(200);
          $(parentthis).siblings().each(function(){
            $(this).children().children('.remote_issue_item').slideUp(1000);
          });          
        }, 500);
      });
    }
  }
})(jQuery);