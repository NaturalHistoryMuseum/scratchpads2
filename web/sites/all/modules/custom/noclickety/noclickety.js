(function($){
  Drupal.behaviors.noclickety = {attach: function(context){
    $('.noclickety').removeAttr('disabled').removeClass('noclickety');
    $('input').once().change(function(){
      $('.noclickety').removeAttr('disabled').removeClass('noclickety');
    });
    $('form').once().submit(function(e){
      //$(this).append('<input type="hidden" name="' + $(e.originalEvent.explicitOriginalTarget).attr('name') + '" value="' + $(e.originalEvent.explicitOriginalTarget).val() + '" />');
      //$('input[type="submit"][disabled!="disabled"]', $(this)).attr('disabled', 'disabled').addClass('noclickety');
    });
  }};
})(jQuery);