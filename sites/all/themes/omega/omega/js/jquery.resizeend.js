(function ($) {

  'use strict';

  /**
   * Container for the resizeend timeout.
   */
  var resizeTimeout;

  /**
   * Throttled resize event. Fires only once after the resize ended.
   */
  var event = $.event.special.resizeend = {
    setup: function () {
      $(this).bind('resize', event.handler);
    },

    teardown: function () {
      $(this).unbind('resize', event.handler);
    },

    handler: function (e) {
      var context = this;
      if (resizeTimeout) {
        clearTimeout(resizeTimeout);
      }

      resizeTimeout = setTimeout(function () {
        // Set correct event type
        e.type = 'resizeend';
        $(context).trigger(e);
      }, 150);
    }
  };

  /**
   * Wrapper for the resizeend event.
   */
  $.fn.resizeend = function (handler) {
    return $(this).bind('resizeend', handler);
  };

})(jQuery);
