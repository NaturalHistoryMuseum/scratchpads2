(function($) {
  Drupal.behaviors.vocabulariesSummary = {
    attach : function(context) {
      $('fieldset.scratchpads-vocabularies-fieldset', context)
          .drupalSetSummary(function(context) {
            return $('.scratchpads-vocabularies-summary', context).html();
          });
    }
  }
})(jQuery);