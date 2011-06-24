/**
 * Controller for undoing updates
 */
 
 (function($) {
     function SlickGridUndo($container)
     {
         
         var $control;
         var commandQueue = [];
         var enabled;
         
         function init()
         {
             constructUI();
             // Add the command handler
             options['editCommandHandler'] = queueAndExecuteCommand;
         }
         
         function enable(){
           $control.addClass('enabled').click(confirmDialog);
           enabled = true;
         }
         
         function disable(){
           $control.removeClass('enabled').unbind('click');
           enabled = false;
         }
         
         function confirmDialog(){
           
           // Get the last command
           var command = commandQueue[commandQueue.length  - 1];
           
           var count = 0;
           for (k in command.updated) count++;
           
           $dialog = $('<div />');
           $('<h6>Confirm undo</h6><p>'+ Drupal.formatPlural(count, 'Are you sure you want to undo updating this item?', 'Are you sure you want to undo updating these @count items?')+'</p>').appendTo($dialog);
           $("<button>Undo</button>")
           .click(doUndo)
           .appendTo($dialog);
           $("<button>Cancel</button>")
           .click(cancelUndo)
           .appendTo($dialog);
           slickgrid.openDialog($container, $dialog);
           
         }
         
         function doUndo(){
           
           slickgrid.closeDialog();
           
           var command = commandQueue.pop();
               if (command && Slick.GlobalEditorLock.cancelCurrentEdit()) {
                   
                   // Undo is handled via revisions so post to backend all entities updated as part of this command
                   
                   // Get the column
                   var c = grid.getColumns()[command.cell];
                   
                   var data = {
                     display_id: slickgrid.getViewDisplayID(),
                     view: slickgrid.getViewName(),
                     field_name: c.field,
                     field_id: c.id,
                     updated: command.updated
                   }
                   
                   slickgrid.callback('undo', data, true);
                   grid.gotoCell(command.row, command.cell, false);
                   
               }
               
               if(commandQueue.length == 0){
                 disable();
               }
           
         }
         
         function cancelUndo(){
           
           slickgrid.closeDialog();
           
         }
         
         function queueCommand(items){
           
           // If the undo control isn't active, enable it
           if(!enabled){
             enable();
           }                   
           // Store all of the updated items so we know we can undo them
           commandQueue[commandQueue.length  - 1]['updated'] =  items;
           
         }
         
         function constructUI()
         {
             $container.empty();
             $control = $("<span title='Undo' class='slickgrid-control-button' />").appendTo($container);
         }
         
         function queueAndExecuteCommand(item,column,editCommand) {

             commandQueue.push(editCommand);
             editCommand.execute();
             
         }
         
         //// Public API ////
         $.extend(this, {
            // Methods
            "queueAndExecuteCommand":     queueAndExecuteCommand,
            "queueCommand":               queueCommand,
         });

         init();
     }

     // Slick.Controls.Undo
     $.extend(true, window, { Slick: { Controls: { Undo: SlickGridUndo }}});
 })(jQuery);
