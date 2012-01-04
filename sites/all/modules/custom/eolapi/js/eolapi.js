(function($){
  // EOLAPI object.
  Drupal.eolapi = Drupal.eolapi || {};
  // Attach behaviors
  Drupal.behaviors.eolapi = {attach: function(context, settings){
    $('.eolapi-empty').each(function(){
      var original_this = this;
      $.ajax({data: $(this).data(), type: 'POST', url: Drupal.settings.eolapi.callback, success: function(data, textStatus, jqXHR){
        $(original_this).html(data);
      }, error: function(data, textStatus, jqXHR){
        $(original_this).html('No results.');
      }});
    });
  }};
})(jQuery);