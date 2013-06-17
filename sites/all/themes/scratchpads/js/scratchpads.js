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
  if(Drupal.jsAC) {
    /**
     * Override this core function so that we set the minWidth, and not the
     * width.
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
    /**
     * Because we're using minWidth, we need to override this to allow us to
     * alter the position if the box is too big.
     */
    Drupal.jsAC.prototype.found = function(matches){
      // If no value in the textfield, do not show the popup.
      if(!this.input.value.length) {
        return false;
      }
      // Prepare matches.
      var ul = $('<ul></ul>');
      var ac = this;
      for(key in matches) {
        $('<li></li>').html($('<div></div>').html(matches[key])).mousedown(function(){
          ac.select(this);
        }).mouseover(function(){
          ac.highlight(this);
        }).mouseout(function(){
          ac.unhighlight(this);
        }).data('autocompleteValue', key).appendTo(ul);
      }
      // Show popup with matches, if any.
      if(this.popup) {
        if(ul.children().length) {
          $(this.popup).empty().append(ul).show();
          var offset = $(this.popup).offset();
          if((offset.left + $(this.popup).width()) > $(window).width()) {
            var left_move = (offset.left + $(this.popup).width() + 5) - $(window).width();
            if(left_move > offset.left) {
              left_move = offset.left;
            }
            $(this.popup).css('left', '-' + left_move + 'px');
          }
          $(this.ariaLive).html(Drupal.t('Autocomplete popup'));
        } else {
          $(this.popup).css({visibility: 'hidden'});
          this.hidePopup();
        }
      }
    };
  }
})(jQuery);