(function ($) {
  Drupal.behaviors.spm_form = {
    attach: function (context, settings) {
      $(document).ready(function() {
        $('#edit-draggable-drag-title').change(function(){
          if($('#edit-draggable-drag-title:checked')){
            $('#edit-display-title').attr('checked', 'checked');
          }
        });
      });
    }
  };

}(jQuery));