Drupal.behaviors.remote_issues_block = function(context){
  var remote_issues_block_hidden = true;
  $('#remote-issue-tab #open-close', context).click(function(){
    if(remote_issues_block_hidden){
      $('#remote-issue-tab').animate({right:0}, 1000);
      remote_issues_block_hidden = false;
    } else {
      $('#remote-issue-tab').animate({right:-375}, 1000);
      remote_issues_block_hidden = true;      
    }
  });
  $('.items li').bind('mouseenter', function(){
    $(this).children().children('p').slideDown(500);
    $(this).siblings().each(function(){
      $(this).children().children('p').slideUp(500);
    });
  });
};