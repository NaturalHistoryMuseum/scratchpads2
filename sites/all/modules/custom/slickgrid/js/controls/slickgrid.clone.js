/**
 * Controller for cloning nodes
 */
 (function($) {
    function SlickGridClone(dataView, grid, $container)
    {

        var $control;
        var enabled;
        
        function init()
        {
            
            constructUI();

            grid.onSelectedRowsChanged.subscribe(handleSelectedRowsChanged);

        }

        function handleSelectedRowsChanged() {

            if (grid.getSelectedRows().length) {
                enable();
            } else {
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

        function confirmDialog() {

            var $dialog;

            $dialog = $('<div />');
            $('<h6 class="error">Confirm clone</h6><p>' + Drupal.formatPlural(grid.getSelectedRows().length, 'Are you sure you want to clone this item?', 'Are you sure you want to clone these @count items?') + '</p>').appendTo($dialog);
            $("<button>Clone</button>")
            .click(doClone)
            .appendTo($dialog);
            $("<button>Cancel</button>")
            .click(cancelClone)
            .appendTo($dialog);

            slickgrid.openDialog($container, $dialog);

        }

        function doClone() {

            slickgrid.closeDialog();

            var data = {
                entity_ids: slickgrid.getEntityIDs(),
                display_id: slickgrid.getViewDisplayID(),
                view: slickgrid.getViewName(),
                entity_type: options['entity_type'],
            }
            slickgrid.callback('clone', data, true);

        }

        function cancelClone() {

            slickgrid.closeDialog();

        }

        function constructUI()
        {
            $container.empty();
            $control = $("<span title='Clone' class='slickgrid-control-button' />").appendTo($container);
        }

        init();
    }

    // Slick.Controls.Clone
    $.extend(true, window, {
        Slick: {
            Controls: {
                Clone: SlickGridClone
            }
        }
    });
})(jQuery);