/**
 * Colour switching javascript
 */
  (function($) {
    Drupal.behaviors.scratchpadsColour = {
      attach: function (context) {     
        // Colour choose
        $('.scratchpad-color-option', context).click(function() { 
          // Update body class
          var body = $("body");
          body.removeClass (function (index, css) {
              return (css.match (/\bscratchpad-colour\S+/g) || []).join(' ');
          });
          body.addClass('scratchpad-colour-'+this.id);
          // Change the radio button
          $(":radio[value="+this.id+"]").attr('checked',true);
           return false;
        });
      }   
    };
  })(jQuery);