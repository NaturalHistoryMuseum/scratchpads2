/**
 * Species pages javascript
 */
 
(function($) {
 Drupal.behaviors.scratchpadsSpecies = {
   attach: function (context) {  
     // Apply exposed filters via link 
     $('a.view-filter', context).click(function() {    
       var $form = $(this).parents('.view').find('form');
       var filters = this.hash.substring(1).split('=');
       $('[name='+filters[0]+']', $form).val(filters[1]);
       $('.form-submit', $form).trigger('click');
        return false;
     });
   }   
 };
})(jQuery);