//$Id$
/**
 * @file
 * cck_select_other javascript file 
 */

(function ($) {
  Drupal.behaviors.cckSelectOther = {
    attach: function (context, settings) {

      // Prevent errors
      if (typeof settings.CCKSelectOther != 'object') return;

      $.each(settings.CCKSelectOther, function(n,MyCCKSelectOther){

        // Prevent errors
        if (typeof MyCCKSelectOther.field_id == 'undefined') return;

        var field_id = new String(MyCCKSelectOther.field_id);
        var select_id = new String(field_id + '-select-other-list');
        var text_id = new String(field_id + '-select-other-text-input');

        var ActionBind = (($.browser.msie == true) ? 'click' : 'change');

        $(document).ready( function() {
          // We need to go up further up the element chain to work around 'add another item'
          $('select#edit-' + select_id).bind(ActionBind,function() {
            // Add parent() to hide input wrapper
            $('input#edit-' + text_id).parent().css('display', ($(this).val() == "other") ? 'block' : 'none');
          }).trigger(ActionBind);
        });
      });
    }
  }
})(jQuery);
