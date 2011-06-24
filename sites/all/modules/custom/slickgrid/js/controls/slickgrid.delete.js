/**
 * Controller for deleting items
 */
 
 (function($) {
     function SlickGridDelete(dataView, grid, $container)
     {
         
         var $control;
         var enabled; 
         
         function init()
         {
             constructUI();

             grid.onSelectedRowsChanged.subscribe(handleSelectedRowsChanged);
             
         }
        
         
         function handleSelectedRowsChanged(){
           
           if(grid.getSelectedRows().length){
             enable();
           }else{
             disable();
           }
           
         }
         
         function enable(){
           if(!enabled){
             $control.addClass('enabled').click(confirmDialog);
             enabled = true; 
           }
         }
         
         function disable(){
           if(enabled){
             $control.removeClass('enabled').unbind('click');
             enabled = false;             
           }
         }
         
         function confirmDialog(){
          
           var $dialog;
             
           $dialog = $('<div />');
           $('<h6 class="error">Confirm delete</h6><p>'+ Drupal.formatPlural(grid.getSelectedRows().length, 'Are you sure you want to delete this item?', 'Are you sure you want to delete these @count items?')+'</p>').appendTo($dialog);
           $("<button>Delete</button>")
           .click(doDelete)
           .appendTo($dialog);
           $("<button>Cancel</button>")
           .click(cancelDelete)
           .appendTo($dialog);                           
           slickgrid.openDialog($container, $dialog);              
           
         }
         
         function doDelete(){
           
           slickgrid.closeDialog();
           
           var data = {
             entity_ids: slickgrid.getEntityIDs(),
             entity_type: options['entity_type']
           }
           slickgrid.callback('delete', data, true);           
           
         }
         
         function cancelDelete(){
           
           slickgrid.closeDialog();
           
         }
         
         function constructUI()
         {
             $container.empty();
             $control =$("<span title='Delete' class='slickgrid-control-button' />").appendTo($container);
         }

         init();
     }

     // Slick.Controls.Undo
     $.extend(true, window, { Slick: { Controls: { Delete: SlickGridDelete }}});
 })(jQuery);