(function($){
  Drupal.behaviors.tinytax = {
    attach: function(context, settings){
      $('img.click', context).click(function(){
        var img_clicked = $(this);
        // If we're clicking a plus
        if(img_clicked.attr('src') == Drupal.settings.tinytax.plus){
          if(img_clicked.parent('li').children('ul').length){
            img_clicked.parent('li').children('ul').show();
            img_clicked.attr('src', Drupal.settings.tinytax.minus);
          } else {
            // Check if we already have the children data, if so, we just show it
            img_clicked.attr('src', Drupal.settings.tinytax.load);
            $.getJSON(Drupal.settings.tinytax.callback+"/"+$(this).attr('id'), function(data){
              img_clicked.parent('li').append(data[1]['data']);
              Drupal.attachBehaviors(img_clicked.parent('li').children('ul'));
              img_clicked.attr('src', Drupal.settings.tinytax.minus);
            });
          }
        }
        // If we're clicking a minus
        else if(img_clicked.attr('src') == Drupal.settings.tinytax.minus){
          img_clicked.parent('li').children('ul').hide();
          img_clicked.attr('src', Drupal.settings.tinytax.plus);
        }
      });
      $('.tinytax-search-field', context).keypress(function(event){
        var keyCode = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;
        if (keyCode == 13) {
          // Check to see if the aria-live thing is empty, if so, we search
          if(!$(this).siblings('[aria-live="assertive"]').html()){
            var vid = $(this).attr('id').replace('tinytax-search-field-','');
            var tinytax_element = $(this).closest('form').siblings('.tinytax');
            $.getJSON(Drupal.settings.tinytax.search_callback+"/"+vid+"/"+encodeURI($(this).val()), function(data){
              $(tinytax_element).html(data[1]['data']);
              Drupal.attachBehaviors($(tinytax_element));
            });
          }
          return false;
        }
        return true;
      });
    }
  }
})(jQuery);