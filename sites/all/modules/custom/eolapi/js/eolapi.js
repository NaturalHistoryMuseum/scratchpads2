(function($){
  // EOLAPI object.
  Drupal.eolapi = Drupal.eolapi || {};
  // Attach behaviors
  Drupal.behaviors.eolapi = {attach: function(context, settings){
    if($('.eolapi-empty').length) {
      $('.eolapi-empty').each(function(){
        var original_this = this;
        $.ajax({async: false, data: $(original_this).data(), type: 'POST', url: Drupal.settings.eolapi.callback, success: function(data, textStatus, jqXHR){
          if(data.length) {
            $(original_this).html(data);
          } else {
            $(original_this).html('No results.');
          }
          $(original_this).removeClass('eolapi-empty');
        }, error: function(data, textStatus, jqXHR){
          $(original_this).html('No results.');
          $(original_this).removeClass('eolapi-empty');
        }});
      })
    }
  }};
})(jQuery);