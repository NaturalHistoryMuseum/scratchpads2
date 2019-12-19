(function ($, Drupal) {
  /**
   * Toggle show/hide links for off canvas layout.
   */
  Drupal.behaviors.omegaOffCanvasLayout = {
    attach: function (context) {
      $('#off-canvas').click(function(e) {
        if (!$(this).hasClass('is-visible')) {
          $(this).addClass('is-visible');
          e.preventDefault();
          e.stopPropagation();
        }
      });

      $('#off-canvas-hide').click(function(e) {
        $(this).parent().removeClass('is-visible');
        e.preventDefault();
        e.stopPropagation();
      });

      $('.l-page').click(function(e) {
        if($('#off-canvas').hasClass('is-visible') && $(e.target).closest('#off-canvas').length === 0) {
          $('#off-canvas').removeClass('is-visible');
          e.stopPropagation();
        }
      });
    }
  };

})(jQuery, Drupal);
