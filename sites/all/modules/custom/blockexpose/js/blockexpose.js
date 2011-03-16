(function($){
  Drupal.behaviors.blockexpose = {
    attach: function(context, settings){
      $('.blockexpose .open-close.click', context).click(function(){
        if($(this).parent().parent().hasClass('right')) {
          if($(this).parent().parent().css('right') == '-375px') {
            $(this).parent().parent().animate({
              right: 0
            }, 1000);
          } else {
            $(this).parent().parent().animate({
              right: -375
            }, 1000);
          }
        } else {
          if($(this).parent().parent().css('left') == '-375px') {
            $(this).parent().parent().animate({
              left: 0
            }, 1000);
          } else {
            $(this).parent().parent().animate({
              left: -375
            }, 1000);
          }
        }
      });
      $('.blockexpose .open-close.hover', context).mouseover(function(){
        if($(this).parent().parent().hasClass('right')) {
          if($(this).parent().parent().css('right') == '-375px') {
            $(this).parent().parent().animate({
              right: 0
            }, 200);
          }
        } else if($(this).hasClass('left')) {
          if($(this).parent().parent().css('left') == '-375px') {
            $(this).parent().parent().animate({
              left: 0
            }, 200);
          }
        }
      });
      $('.blockexpose .open-close.hover', context).parent().parent()
          .mouseleave(function(){
            if($(this).hasClass('right')) {
              if($(this).css('right') == '0px' || $(this).css('right') == '0') {
                $(this).animate({
                  right: -375
                }, 200);
              }
            } else if($(this).hasClass('left')) {
              if($(this).css('left') == '0px' || $(this).css('left') == '0') {
                $(this).animate({
                  left: -375
                }, 200);
              }
            }
          });
    }
  }
})(jQuery);