/**
 * Pensoft JS tweaks
 */
 (function($){

   Drupal.behaviors.pensoft = {

     attach: function(context){
    
      function init(){
        
         var element_settings = {
           url: '/pensoft/ajax',
           event: 'selectItem',
         }
        
        $('#edit-relation-select-endpoints:not(.pensoft-processed)').each(function(){        
          $(this).bind("selectItem", selectItem).bind("deselectItem", deselectItem);
          Drupal.ajax['pensoft'] = new Drupal.ajax('pensoft', $(this), element_settings);    
        }).addClass('pensoft-processed');
      
      }
    
      function selectItem(e, id, entitydata){
        Drupal.ajax['pensoft']['submit']['spm_entity'] = entitydata;      
      }
      
      function deselectItem(e, id, entitydata){
        $('#edit-fields').empty();     
      } 
      
      init();
            
     }
     
     
    
  };
})(jQuery);  