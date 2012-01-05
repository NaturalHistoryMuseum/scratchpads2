/**
 * Scratchpad theme javascript
 */
 
 /**
  * Attaches the debugging behavior.
  */
 (function($) {
   Drupal.behaviors.scratchpads = {
     attach: function (context) {     
       // Slide toggles  
       $('.scratchpads-slide-toggle', context).click(function() {          
          $(this).parents('.zone-wrapper').find('form').slideToggle();
           return false;
       });
     }   
   };
 })(jQuery);