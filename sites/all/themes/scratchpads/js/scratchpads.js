/**
 * Scratchpad theme javascript
 */

/**
 * Attaches the debugging behavior.
 */
(function($){
  Drupal.behaviors.scratchpads = {attach: function(context){
    // Slide toggles
    $('.scratchpads-slide-toggle', context).once().click(function(){
      var body = $(this).parent('.scratchpads-slide-toggle-container').find('.scratchpads-slide-toggle-body');
      if(body.length > 0) {
        $(this).parents('.zone-wrapper').find('form:visible, .scratchpads-slide-toggle-body:visible').not(body).slideToggle();
        body.slideToggle();
      } else {
        $(this).parents('.zone-wrapper').find('.scratchpads-slide-toggle-body:visible').slideToggle();
        $(this).parents('.zone-wrapper').find('form').slideToggle();
      }
      return false;
    });
  }};
  /**
   * Override this core function so that we set the minWidth, and not the width.
   */
  Drupal.jsAC.prototype.populatePopup = function(){
    var $input = $(this.input);
    var position = $input.position();
    // Show popup.
    if(this.popup) {
      $(this.popup).remove();
    }
    this.selected = false;
    this.popup = $('<div id="autocomplete"></div>')[0];
    this.popup.owner = this;
    $(this.popup).css({top: parseInt(position.top + this.input.offsetHeight, 10) + 'px', left: parseInt(position.left, 10) + 'px', minWidth: $input.innerWidth() + 'px', display: 'none'});
    $input.before(this.popup);

    // Do search.
    this.db.owner = this;
    this.db.search(this.input.value);
  };
})(jQuery);