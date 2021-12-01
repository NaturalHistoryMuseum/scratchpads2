/**
 * @file media.browser.edit.js
 * Special behavior when displaying the file edit form in the media browser iframe.
 */

(function ($) {
  Drupal.behaviors.mediaBrowserEdit = {
    attach: function (context) {
      var $iframe = $('iframe.media-modal-frame', top.document);

      // Enable iframe scrolling and remove padding.
      $iframe
        .attr('scrolling', 'auto')
        .css({'padding': 0, 'margin': 0});

      // Add document padding.
      $('#media-browser-page-wrapper')
        .css({
          'padding': '1em 1em 0 1em',
          'margin-bottom': '1em',
        });

      // Adjust the iframe height.
      var height = $iframe.height();
      var content_height = $('body').height();
      if (content_height > height) {
        var window_height = $(top.window).height();
        var margin_top = parseInt($iframe.parent().css('top')) - $(top).scrollTop();
        height = Math.min(content_height, window_height - margin_top - 50);
        $iframe.animate({height: height + 'px'});
      }
    }
  };
})(jQuery);
