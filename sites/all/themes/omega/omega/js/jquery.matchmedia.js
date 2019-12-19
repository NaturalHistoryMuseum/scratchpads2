(function ($, window) {

  'use strict';

  /**
   * Check if the given media query currently applies.
   *
   * @param query
   *   The media query to check for.
   *
   * @deprecated
   *   Use window.matchMedia() instead.
   */
  $.matchmedia = function (query) {
    return window.matchMedia(query);
  };

  /**
   * Special event for listening to media query changes.
   *
   * @deprecated
   *   Use window.matchMedia(query).addListener(callback) instead.
   */
  var event = $.event.special.mediaquery = {
    objects: {},

    handler: function (handler) {
      return function (mql) {
        mql.applies = mql.matches;
        handler.call(mql, mql);
      };
    },

    add: function (handleObj) {
      event.objects[handleObj.guid] = window.matchMedia(handleObj.data);
      event.objects[handleObj.guid].addListener(event.handler(handleObj.handler));
    },

    remove: function (handleObj) {
      event.objects[handleObj.guid].removeListener(event.handler(handleObj.handler));
    }
  };

  /**
   * Event shortcut.
   *
   * @deprecated
   *   Use window.matchMedia(query).addListener(callback) instead.
   */
  $.fn.mediaquery = function (query, callback) {
    return $(this).bind('mediaquery', query, callback);
  };

})(jQuery, window);
