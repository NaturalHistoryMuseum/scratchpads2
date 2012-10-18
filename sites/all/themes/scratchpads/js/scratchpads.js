/**
 * Scratchpad theme javascript
 */

/**
 * Attaches the debugging behavior.
 */
(function($){
  Drupal.behaviors.scratchpads = {
    attach: function(context){
    // Slide toggles
    $('.scratchpads-slide-toggle', context).once().click(function(){      
      var body = $(this).parent('.scratchpads-slide-toggle-container').find('.scratchpads-slide-toggle-body');
      if (body.length > 0) {
        $(this).parents('.zone-wrapper').find('form:visible, .scratchpads-slide-toggle-body:visible').not(body).slideToggle();
        body.slideToggle();
      } else {
        $(this).parents('.zone-wrapper').find('.scratchpads-slide-toggle-body:visible').slideToggle();
        $(this).parents('.zone-wrapper').find('form').slideToggle();
      }
      return false;
    });
  }};
})(jQuery);