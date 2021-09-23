(function($){
  Drupal.behaviors.blockexpose = {attach: function(context, settings){
    $('.blockexpose').each(function(){
      var blockexpose_this = $(this);
      $('*', $(this)).each(function(){
        if($(this).width() > 368) {
          var change = $(this).width() - 368;
          $(blockexpose_this).width(395 + change);
          $('.subcontent', $(blockexpose_this)).width(368 + change);
          if($(blockexpose_this).hasClass('right')) {
            $(blockexpose_this).css('right',-375-change);
          } else {
            $(blockexpose_this).css('left',-375-change);
          }
        }
      })
    });
    $('.blockexpose .open-close.click', context).click(function(){
      if($(this).parent().parent().hasClass('right')) {
        if($(this).parent().parent().css('right') == '0px') {
          $(this).parent().parent().animate({right: -($(this).parent().parent().width()-20)}, 1000);
        } else {
          $(this).parent().parent().animate({right: 0}, 1000);
        }
      } else {
        if($(this).parent().parent().css('left') == '0px') {
          $(this).parent().parent().animate({left: -($(this).parent().parent().width()-20)}, 1000);
        } else {
          $(this).parent().parent().animate({left: 0}, 1000);
        }
      }
    });
    $('.blockexpose .open-close.hover', context).mouseover(function(){
      if($(this).parent().parent().hasClass('right')) {
        if($(this).parent().parent().css('right') != '0px') {
          $(this).parent().parent().animate({right: 0}, 200);
        }
      } else if($(this).parent().parent().hasClass('left')) {
        if($(this).parent().parent().css('left') != '0px') {
          $(this).parent().parent().animate({left: 0}, 200);
        }
      }
    });
    $('.blockexpose .open-close.hover', context).parent().parent().mouseleave(function(){
      if($(this).hasClass('right')) {
        if($(this).css('right') == '0px' || $(this).css('right') == '0') {
          $(this).animate({right: -($(this).width()-20)}, 1000);
        }
      } else if($(this).hasClass('left')) {
        if($(this).css('left') == '0px' || $(this).css('left') == '0') {
          $(this).animate({left: -($(this).width()-20)}, 1000);
        }
      }
    });
  }}
})(jQuery);