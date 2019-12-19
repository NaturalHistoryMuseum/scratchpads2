(function ($) {

  'use strict';

  /**
   * Custom expression for filtering for scrollable elements.
   */
  $.expr[':'].scrollable = function (elem) {
    var scrollable = true;
    // Backup the original scroll position.
    var original = $(elem).scrollTop();

    if (original === 0) {
      $(elem).scrollTop(1);
      scrollable = $(elem).scrollTop() === 1;
      $(elem).scrollTop(0);
    }

    return scrollable;
  };

})(jQuery);
