(function ($) {
  Drupal.behaviors.BiblioHighlight = {
    attach: function (context, settings) {
      $('input#biblio-highlight', context).click(function(e) {
        $("div.suspect").toggleClass('biblio-highlight');
      });
    }
  };
}(jQuery));