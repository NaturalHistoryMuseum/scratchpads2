(function($){
Drupal.behaviors.blockexpose = {
 attach: function (context, settings){
  $('.blockexpose .open-close', context).click(function(){
    if($(this).parent().parent().hasClass('right')){
      if($(this).parent().parent().css('right') == '-375px'){
        $(this).parent().parent().animate({right:0}, 1000);
        blockexpose_hidden = false;
      } else {
        $(this).parent().parent().animate({right:-375}, 1000);
        blockexpose_hidden = true;      
      }
    }else{
      if($(this).parent().parent().css('left') == '-375px'){
        $(this).parent().parent().animate({left:0}, 1000);
        blockexpose_hidden = false;
      } else {
        $(this).parent().parent().animate({left:-375}, 1000);
        blockexpose_hidden = true;      
      }      
    }
  });
 }
}
})(jQuery);