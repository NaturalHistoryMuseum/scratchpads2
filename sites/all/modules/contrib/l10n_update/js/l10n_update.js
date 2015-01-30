(function ($) {

Drupal.behaviors.l10nUpdateCollapse = {
  attach: function (context, settings) {
    $('.l10n-update .l10n-update-wrapper', context).once('l10nupdatecollapse', function () {
      var wrapper = $(this);

      // Turn the project title into a clickable link.
      // Add an event to toggle the content visibiltiy.
      var $legend = $('.project-title', this);
      var $link = $('<a href="#"></a>')
        .prepend($legend.contents())
        .appendTo($legend)
        .click(function () {
          Drupal.toggleFieldset(wrapper);
          return false;
        });
    });
  }
};

})(jQuery);
