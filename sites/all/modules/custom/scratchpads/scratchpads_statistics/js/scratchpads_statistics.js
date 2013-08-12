/**
 * Override the hidePopup function.
 */
(function($){
  Drupal.behaviors.scratchpads_statistics = {attach: function(context, settings){
    $('#scratchpads-statistics-facet-form-category select').change(function(){
      $('#scratchpads-statistics-facet-form-category').submit();
    });
  }}
  Drupal.jsAC.prototype.hidePopup = function(keycode){
    // Select item if the right key or mousebutton was pressed.
    if(this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
      this.input.value = $(this.selected).data('autocompleteValue');
      // The following is the only addition.
      if($(this.input.form).hasClass('autocomplete-submit')) {
        this.input.form.submit();
      }
      // End of addition
    }
    // Hide popup.
    var popup = this.popup;
    if(popup) {
      this.popup = null;
      $(popup).fadeOut('fast', function(){
        $(popup).remove();
      });
    }
    this.selected = false;
    $(this.ariaLive).empty();
  };
})(jQuery);