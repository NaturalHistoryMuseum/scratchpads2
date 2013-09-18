(function($) {
  Drupal.behaviors.scrachpads_citethispage = {
    attach: function(context, settings) {
      var $form = $('#citethispage-backend-selection-form', context);
      // We do this on mouseenter because it has to be done before mousedown
      $('input[type=submit]', $form).mouseenter(function() {
        // Parse the tinytax position
        var open_tids = [];
        if (typeof Drupal.behaviors.tinytax.getStatus === 'function') {
          var result = Drupal.behaviors.tinytax.getStatus();
          open_tids = result.open_tids;
          $('input[name=citethispage_open_tids]', $form).attr('value', open_tids.join(','));
        }
        // Get Slickgrid state
        if (typeof slickgrid !== 'undefined') {
          var state = slickgrid.getGridState();
          var json = '';
          if (typeof $.toJSON === 'function') {
            json = $.toJSON(state);
          } else if (typeof JSON.stringify === 'function') {
            json = JSON.stringify(state);
          }
          console.log(json);
          $('input[name=citethispage_slick_state]', $form).attr('value', json);
        }
      });
    }
  }
})(jQuery);
