(function($){
  // Tweak what is sent to the server when the button is clicked.
  Drupal.behaviors.slickgrid_clone = {attach: function(context, settings){
    if(Drupal.ajax.slickgrid_clone_form){
      Drupal.ajax.slickgrid_clone_form.beforeSend = function(xmlhttprequest, opts){
        opts.data = $.param({entity_type: options['entity_type'], entity_ids: slickgrid.getEntityIDs()}) + '&' + opts.data;
      }
    }
  }}
  // Enable and disable the button when rows are selected/deselected.
  function SlickGridClone(grid, container){
    grid.onSelectedRowsChanged.subscribe(function(){
      if(grid.getSelectedRows().length) {
        $(container).children().children().addClass('enabled');
      } else {
        $(container).children().children().removeClass('enabled');
      }
    });
  }
  $.extend(true, window, {Slick: {Controls: {Clone: SlickGridClone}}});
})(jQuery);