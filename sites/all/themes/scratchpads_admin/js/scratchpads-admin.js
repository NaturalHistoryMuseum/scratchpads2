/**
 * Javascript for the scratchpads admin theme
 */
(function($){
  // These two functions should be namespaced!
  var textboxToFocus = {};
  function SetCaretAtEnd(elem){
    var elemLen = elem.value.length;
    // For IE Only
    if(document.selection) {
      // Set focus
      elem.focus();
      // Use IE Ranges
      var oSel = document.selection.createRange();
      // Reset position to 0 & then set at end
      oSel.moveStart('character', -elemLen);
      oSel.moveStart('character', elemLen);
      oSel.moveEnd('character', 0);
      oSel.select();
    } else if(elem.selectionStart || elem.selectionStart == '0') {
      // Firefox/Chrome
      elem.selectionStart = elemLen;
      elem.selectionEnd = elemLen;
      elem.focus();
    } // if
  } // SetCaretAtEnd()
  $(document).ajaxComplete(function(event, request, settings){
    if(typeof textboxToFocus.formid !== 'undefined') {
      var textBox = $('#' + textboxToFocus.formid + ' input:text[name="' + textboxToFocus.name + '"]');
      textBox.val(textboxToFocus.value);
      SetCaretAtEnd(textBox[0]);
      addFocusReminder(textBox);
      // textboxToFocus = {}; // if you have other auto-submitted inputs as well
    }
  });
  Drupal.behaviors.scratchpadsAdmin = {};
  Drupal.behaviors.scratchpadsAdmin.attach = function(context){
    $('.view-filters input:text.ctools-auto-submit-processed').bind('keypress keyup', function(e){
      textboxToFocus.formid = $(this).closest('form').attr('id');
      textboxToFocus.name = $(this).attr('name');

      if(e.type == 'keypress') {
        if(e.keyCode != 8) { // everything except return
          textboxToFocus.value = $(this).val() + String.fromCharCode(e.charCode);
        } else {
          textboxToFocus.value = $(this).val().substr(0, $(this).val().length - 1)
        }
      } else { // keyup
        textboxToFocus.value = $(this).val();
      }
    });
    // Attach the help slider behaviour
    $('.help-shortcut a:not(.scratchpads-admin-processed)', context).click(function(){
      $('.region-help').slideToggle();
      return false;
    }).addClass('scratchpads-admin-processed');

    $('a.toggler:not(.rubik-processed)', context).each(function(){
      var id = $(this).attr('href').split('#')[1];
      // Target exists, add click handler.
      if($('#' + id).size() > 0) {
        $(this).click(function(){
          toggleable = $('#' + id);
          toggleable.toggle();
          $(this).toggleClass('toggler-active');
          return false;
        });
      }
      // Target does not exist, remove click handler.
      else {
        $(this).addClass('toggler-disabled');
        $(this).click(function(){
          return false;
        });
      }
      // Mark as processed.
      $(this).addClass('rubik-processed');
    });
  };
  if(Drupal.media && Drupal.media.browser){
    if(Drupal.media.browser.resizeIframe) {
      Drupal.media.browser.resizeIframe = function(event){
        // Add an extra 20 pixels to prevent the scroll bar from thinking it is
        // actually required.
        var h = $('body').height() + 20;
        $(parent.window.document).find('#mediaBrowser').height(h);
      };
    }
  }
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