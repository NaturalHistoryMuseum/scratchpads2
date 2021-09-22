(function($){
  Drupal.behaviors.scratchpads_user = {attach: function(context){
    var mail = '';
    var can_login = false;
    if($('#edit-create-user-account').is(':checked')) {
      can_login = true;
    }
    var required_span = '<span class="form-required" title="This field is required.">*</span>';
    var fields_to_toggle = ['field-user-given-names', 'field-user-title', 'field-user-country'];
    var required_fields = ['field-user-given-names', 'field-user-country'];
    $('input[name="create_user_account"]').change(function(){
      for( var i = 0; i < fields_to_toggle.length; i++) {
        $('#edit-' + fields_to_toggle[i] + ' .form-required').toggle();
      }
      // We toggle the variable each time the 'create_user_account' checkbox is clicked
      can_login = !can_login;
      if(can_login) {
        // Keep the old email, incase we toggle back to being login-less
        mail = $('input[name="mail"]').val();
        // Remove the auto-generated email from view
        $('input[name="mail"]').val('');
        // Add the HTML and css for extra required fields
        for( var i = 0; i < required_fields.length; i++) {
          if(!$('#edit-' + required_fields[i] + ' :input').hasClass("required")) {
            $(required_span).appendTo('#edit-' + required_fields[i] + ' label');
          }
          $('#edit-' + required_fields[i] + ' :input').addClass("required");
        }
      } else {
        // Put the email back, as the user is login-less again
        $('input[name="mail"]').val(mail);
        // Remove the HTML and css for extra required fields
        for( var i = 0; i < required_fields.length; i++) {
          $('#edit-' + required_fields[i] + ' :input').removeClass("required");
          $('#edit-' + required_fields[i] + ' .form-required').remove();
        }
      }
    })
  }}
})(jQuery);