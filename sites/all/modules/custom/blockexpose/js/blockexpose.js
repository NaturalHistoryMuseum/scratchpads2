(function($){
Drupal.behaviors.blockexpose = {
 attach: function (context, settings){
  var blockexpose_hidden = true;
  $('.blockexpose .open-close', context).click(function(){
    if(blockexpose_hidden){
      $('.blockexpose').animate({right:0}, 1000);
      blockexpose_hidden = false;
    } else {
      $('.blockexpose').animate({right:-375}, 1000);
      blockexpose_hidden = true;      
    }
  });
 }
}
})(jQuery);