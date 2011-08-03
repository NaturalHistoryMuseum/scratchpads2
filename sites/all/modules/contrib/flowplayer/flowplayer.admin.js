
/**
 * @file
 * Provides the Drupal behavior for the Flowplayer administration page.
 */

/**
 * The Flowplayer Drupal administration behavior.
 */
(function ($) {
  Drupal.behaviors.flowplayeradmin = {
    attach: function(context, settings) {
      // Update both the Flowplayer preview and the textbox background whenever a textbox gets changed.
      var updateTextBox = function(color, object) {
        $(object).css({
          'backgroundColor': color,
          'color': settings.flowplayerAdminFarbtastic.RGBToHSL(settings.flowplayerAdminFarbtastic.unpack(color))[2] > 0.5 ? '#000' : '#fff'
        });

        var target = $(object).attr('rel');
        var player = $f('flowplayer-preview');
        if (player) {
          player.getControls().css(target, color);
        }
      }
      
      // Create the Farbtastic color picker
      settings.flowplayerAdminFarbtastic = $.farbtastic('#flowplayer-color-picker', function(color) {
        $(settings.flowplayerAdminTextbox).val(color);
        updateTextBox(color, settings.flowplayerAdminTextbox);
      });

      // Make the focus of the textbox change the input box we're acting on.
      $('#flowplayer-color input:text', context).focus(function() {
        settings.flowplayerAdminTextbox = this;
      });

      // Colour the text boxes their value color.
      $('#flowplayer-color input:text').each(function(index, object) {
        var value = $(object).val();
        if (value) {
          updateTextBox($(object).val(), object);
        }
      });
    }
  };
})(jQuery);