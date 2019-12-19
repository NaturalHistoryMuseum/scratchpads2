(function ($) {

  'use strict';

  /**
   * Renders a widget for displaying the current width of the browser.
   */
  Drupal.behaviors.omegaBrowserWidth = {
    attach: function (context) {
      $('body', context).once('omega-browser-width', function () {
        var $indicator = $('<div class="omega-browser-width" />').appendTo(this);

        // Bind to the window.resize event to continuously update the width.
        $(window).bind('resize.omega-browser-width', function () {
          $indicator.text($(this).width() + 'px');
        }).trigger('resize.omega-browser-width');
      });
    }
  };

})(jQuery);
