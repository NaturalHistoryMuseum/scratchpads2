/**
 * Controller for undoing updates
 */
(function($){
  var commandQueue = [];
  // Tweak what is sent to the server when the button is clicked.
  Drupal.behaviors.slickgrid_undo = {attach: function(context, settings){
    Drupal.ajax['slickgrid_undo_form'].beforeSend = function(xmlhttprequest, opts){
      // We send the last command to the server to get a proper form!
      // This will be shifted from the end of the array once we have a return
      // success.
      var command = commandQueue[commandQueue.length - 1];
      opts.data = $.param({entity_type: options['entity_type'], command: command}) + '&' + opts.data;
    }
  }}
  // Extend the jQuery object so that we can easily pop commands from the undo
  // log.
  $.prototype.slickgrid_undo_pop = function(messages){
    if(commandQueue.length) {
      commandQueue.pop();
    }
    if(!commandQueue.length) {
      undoControl.disable();
    }
  }
  // The Undo button.
  function SlickGridUndo(container){
    var enabled;
    function init(){
      // Add the command handler
      options['editCommandHandler'] = queueAndExecuteCommand;
    }
    function enable(){
      $(container).children().children().addClass('enabled');
      enabled = true;
    }
    function disable(){
      $(container).children().children().removeClass('enabled');
      enabled = false;
    }
    function queueCommand(items){
      // If the undo control isn't active, enable it
      if(!enabled) {
        enable();
      }
      // Store all of the updated items so we know we can undo them
      commandQueue[commandQueue.length] = items
    }
    function queueAndExecuteCommand(item, column, editCommand){
      commandQueue.push(editCommand);
      editCommand.execute();
    }
    $.extend(this, {"queueAndExecuteCommand": queueAndExecuteCommand, "queueCommand": queueCommand, "disable": disable});
    init();
  }
  // Slick.Controls.Undo
  $.extend(true, window, {Slick: {Controls: {Undo: SlickGridUndo}}});
})(jQuery);
