(function($){
  Drupal.behaviors.noclickety = {attach: function(context){
    $('.noclickety').removeAttr('disabled').removeClass('noclickety');
    $('input').once().change(function(){
      $('.noclickety').removeAttr('disabled').removeClass('noclickety');
    });
    $('form').once().submit(function(){
      $('input[type="submit"][disabled!="disabled"]', $(this)).attr('disabled', 'disabled').addClass('noclickety');
      return true;
    });
  }};
})(jQuery);