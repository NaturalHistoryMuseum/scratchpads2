
/**
 * @file
 * Provides the FlowPlayer Drupal behavior.
 */

/**
 * The FlowPlayer Drupal behavior that creates the set of FlowPlayer elements from settings.flowplayer.
 */
(function ($) {
  Drupal.behaviors.flowplayer = {
    attach: function(context, settings) {
      /**
       * Called when the Flowplayer is initialized.
       * Had to move from Drupal.behaviors.flowplayeradmin as this is different namespace
       */
      var flowplayerAdminInit = function () {
        var player = $f('flowplayer-preview');
        // Colour the text boxes their value color.
        $('#flowplayer-color input:text').each(function(index, object) {
          var target = $(object).attr('rel');
          var color = $(object).val();
          if (target && color) {
            player.getControls().css(target, color);
          }
        });

        // Controlbar button toggles
        var buttonToggles = $('#flowplayer-styling input:checkbox');
        buttonToggles.change(function() {
          var params = {};
          buttonToggles.each(function(index, object) {
            params[this.value] = this.checked;
          });
          player.getControls().widgets(params);
        });
        buttonToggles.change(); // Update the player to reflect the settings.

        // Border radius
        $('#edit-flowplayer-border-radius').change(function() {
          player.getControls().css("borderRadius", $(this).val());
        });
        $('#edit-flowplayer-border-radius').change();

        // Background gradient
        $("#edit-flowplayer-background-gradient").change(function() {
          player.getControls().css("backgroundGradient", $(this).val());
        });
        $("#edit-flowplayer-background-gradient").change();
      }
      // onload
      jQuery.each(settings.flowplayer, function(selector, config) {

        // Convert any player object events to JavaScript calls.
        var playerEvents = [
        'onBeforeClick',
        'onLoad',
        'onBeforeLoad',
        'onUnload',
        'onBeforeUnload',
        'onMouseOver',
        'onMouseOut',
        'onKeypress',
        'onBeforeKeypress',
        'onVolume',
        'onBeforeVolume',
        'onMute',
        'onBeforeMute',
        'onUnmute',
        'onBeforeUnmute',
        'onFullscreen',
        'onBeforeFullscreen',
        'onFullscreenExit',
        'onPlaylistReplace',
        'onError'
        ];
        jQuery.each(playerEvents, function(index, event) {
          if (typeof(config[event]) == 'string') {
            config[event] = eval(config[event]);
          }
        });

        // Convert any clip object events to JavaScript events.
        var clipEvents = [
        'onBegin',
        'onBeforeBegin',
        'onMetaData',
        'onStart',
        'onPause',
        'onBeforePause',
        'onResume',
        'onBeforeResume',
        'onSeek',
        'onBeforeSeek',
        'onStop',
        'onBeforeStop',
        'onFinish',
        'onBeforeFinish',
        'onLastSecond',
        'onUpdate'
        ];
        if (config['clip']) {
          jQuery.each(clipEvents, function(index, event) {
            if (typeof(config['clip'][event]) == 'string') {
              config['clip'][event] = eval(config['clip'][event]);
            }
          });
        }

        // Register the playlist clip events.
        if (config['playlist']) {
          jQuery.each(config['playlist'], function(index, clip) {
            jQuery.each(clipEvents, function(e_index, event) {
              if (typeof(config['playlist'][index][event]) == 'string') {
                config['playlist'][index][event] = eval(config['playlist'][index][event]);
              }
            });
          });
        }

        // Register the onCuepoint callback.
        if (config['clip'] && config['clip']['onCuepoint'] && typeof(config['clip']['onCuepoint'][1]) == 'string') {
          config['clip']['onCuepoint'][1] = eval(config['clip']['onCuepoint'][1]);
        }

        // Create the flowplayer element on the non-processed elements.
        $(selector + ':not(.flowplayer-processed)').addClass('flowplayer-processed').flowplayer(settings.basePath + settings.flowplayerSwf, config);
      });
    }
  };
})(jQuery);
