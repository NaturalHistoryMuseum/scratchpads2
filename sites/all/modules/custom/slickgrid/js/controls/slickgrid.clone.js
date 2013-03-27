(function($){
  Drupal.behaviors.slickgrid_clone = {attach: function(context, settings){
    Drupal.ajax['slickgrid_clone_form'].beforeSend = function(xmlhttprequest, opts){
      opts.data = $.param({entity_type: options['entity_type'], entity_ids: slickgrid.getEntityIDs()}) + '&' + opts.data;
    }
  }}
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