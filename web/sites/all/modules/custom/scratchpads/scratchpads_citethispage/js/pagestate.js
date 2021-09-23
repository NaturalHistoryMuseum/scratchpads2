(function($) {
  /**
   * saveState
   *
   * This function stores the state of the page
   * (slickgrid, google map and tiny tax block)
   * and saves in the corresponding fields in the given form
   */
  function saveState($form) {
    // Parse the tinytax position
    var open_tids = [];
    if (typeof Drupal.behaviors.tinytax !== 'undefined' &&
        typeof Drupal.behaviors.tinytax.getStatus === 'function') {
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
      $('input[name=citethispage_slick_state]', $form).attr('value', json);
    }
    // Get google maps state
    if (typeof Drupal.settings.gm3 !== 'undefined') {
      var gmapstate = {};
      var json = '';
      for (var key in Drupal.settings.gm3.mapInstances) {
        gmapstate[key] = Drupal.settings.gm3.mapInstances[key].getBounds();
      }
      if (typeof $.toJSON === 'function') {
        json = $.toJSON(gmapstate);
      } else if (typeof JSON.stringify === 'function') {
        json = JSON.stringify(gmapstate);
      }
      $('input[name=citethispage_gmap_state]', $form).attr('value', json);
    }
  }
  /**
   * restoreState
   *
   * This functions restore the state as stored in settings.
   * Note that the tinytax state is restored server side.
   */
  function restoreState(context) {
    // Restore slickgrid
    if (typeof slickgrid !== 'undefined' && typeof Drupal.settings.scratchpads_citethispage.slick_state !== 'undefined') {
      slickgrid.setGridState(Drupal.settings.scratchpads_citethispage.slick_state);
    }
    // Restore gmap
    if (typeof Drupal.settings.gm3.mapInstances !== 'undefined' && typeof Drupal.settings.scratchpads_citethispage.gmap_state !== 'undefined') {
      for (var key in Drupal.settings.gm3.mapInstances) {
        if (typeof Drupal.settings.scratchpads_citethispage.gmap_state[key] !== 'undefined') {
          Drupal.settings.gm3.maps[key].setBounds(Drupal.settings.scratchpads_citethispage.gmap_state[key]);
        }
      }
    }
  }
  Drupal.behaviors.scrachpads_citethispage = {
    attach: function(context, settings) {
      // Handler to save the state
      var $form = $('#citethispage-backend-selection-form', context);
      $('input[type=submit]', $form).mouseenter(function() {
        // We do this on mousenter as it has be done before mousedown
        saveState($form);
      });
      // If required, restore the state
      if (typeof Drupal.settings.scratchpads_citethispage !== 'undefined') {
        setTimeout(function() {
          restoreState(context);
        }, 1000);
      }
    }
  }
})(jQuery);
