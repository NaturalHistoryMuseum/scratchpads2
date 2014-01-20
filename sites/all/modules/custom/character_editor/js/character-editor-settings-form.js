(function($){
  /**
   * Drupal behaviour
   */
  Drupal.behaviors.characterEditorSettingsForm = {
    attach: function(context, settings){
      // Hide the 'Id' label
      $('table#field-char-proj-flags-values thead th.field-label label:contains(Id)', context).css('display', 'none');
      // Hide the 'remove' button for special columns
      $('.character-editor-flag-flag-fixed', context).closest('tr').find('input[type=submit]').remove();
      // React when the flag is changed (autocompletion)
      $('.character-editor-flag-flag input', context).change(function(){
        var $abbr = $(this).closest('tr').find('.character-editor-flag-abbr input');
        var $id = $(this).closest('tr').find('.character-editor-flag-id input');
        if ($abbr.val() == ''){
          $abbr.val($(this).val().substr(0, 1));
        }
        // If we have an id change it. Ids that shouldn't be changed are not sent to the form.
        if ($id.length > 0){
          // Remove the existing id value from the list
          if ($id.val() != ''){
            var existing = $.inArray($id.val(), Drupal.settings.characterEditorSettingsForm.existingIds);
            if (existing >= 0){
              Drupal.settings.characterEditorSettingsForm.existingIds.splice(existing, 1);
            }
          }
          // Find a unique id
          var flag = $(this).val();
          var count = 1;
          var new_id = flag;
          while($.inArray(new_id, Drupal.settings.characterEditorSettingsForm.existingIds) >= 0){
            new_id = flag + "-" + count.toString();
            count++;
          }
          // Set it, and add it to the list of unique ids
          $id.val(new_id);
          Drupal.settings.characterEditorSettingsForm.existingIds.push(new_id);
        }
      });
    }
  }
})(jQuery);