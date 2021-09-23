
/**
 *  @file
 *  jQuery attachment to Styles UI admin pages.
 */
(function ($) {

  /**
   *  Change the preview on radio change.
   */
  Drupal.behaviors.stylesUI = {
    attach: function(context, settings) {
      $('.styles-ui-preset', context).once('stylesUI', function () {
        $(this).bind('change', function() {
          $preset = $(this);
          if ($preset.val()) {
            $.getJSON(Drupal.settings.stylesUI.url + '/' + Drupal.settings.stylesUI.fieldType + '/' + $preset.attr('rel') + '/' + $preset.val(), function(data){
              // @TODO: Check for errors.
              $(data.id).html(data.preview);
            });
          }
        });
      });
    }
  }
// end of closure
})(jQuery);
