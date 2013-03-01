(function($){
  Drupal.behaviors.scratchpads_user = {attach: function(context){
    var fields_to_toggle = ['field-user-given-names', 'field-user-title', 'field-user-country'];
    $('input[name="create_user_account"]').change(function(){
      for( var i = 0; i < fields_to_toggle.length; i++) {
        $('#edit-' + fields_to_toggle[i] + ' .form-required').toggle();
      }
    })
  }}
})(jQuery);