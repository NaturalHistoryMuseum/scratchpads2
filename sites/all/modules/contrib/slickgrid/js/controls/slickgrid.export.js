/**
 * Controller for exporting items
 */
(function($){
  function SlickGridExport(grid, $container){
    var $form;
    function init(){
      constructUI();
    }
    function confirmDialog(){
      var $dialog;
      $dialog = $('<div />');
      $('<h6 class="error">Export data</h6>').appendTo($dialog);
      $("<input type='hidden' name='display_id' value='" + slickgrid.getViewDisplayID() + "' />").appendTo($form);
      // If we are allowing users to export selected rows & rows are selected,
      // add the option
      if(options['export_selected_rows'] && grid.getSelectedRows().length && $('#export-selected-rows', $form).length == 0) {
        $('<input type="checkbox" name="export_selected_rows" id="export-selected-rows" value="1" /><label for="export-selected-rows">Export selected rows only</label>').prependTo($form);
      }
      $form.appendTo($dialog).show();
      slickgrid.openDialog($container, $dialog);
    }
    function constructUI(){
      $container.empty();
      $control = $("<span title='Export' class='slickgrid-control-button enabled' />").click(confirmDialog).appendTo($container);
      $form = $('<form method="post" />');
      $('a', $('.feed-icon')).each(function(){
        $("<input type='image' src='" + $(this).children().eq(0).attr("src") + "' action='" + $(this).eq(0).attr('href') + "' />").click(onSubmit).appendTo($form);
      })
    }
    function onSubmit(){
      var $form = $(this.form);
      if($('#export-selected-rows').is(':checked')) {
        var entityIDs = slickgrid.getEntityIDs();
        $(entityIDs).each(function(i, entityID){
          $('<input type="hidden" name="entity_ids[]" value="' + entityID + '" />').appendTo($form);
        });
      }
      $form.attr('action', $(this).attr('action'));
    }
    init();
  }
  // Slick.Controls.Undo
  $.extend(true, window, {Slick: {Controls: {Export: SlickGridExport}}});
})(jQuery);