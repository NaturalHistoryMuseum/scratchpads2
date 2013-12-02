(function($){
  Drupal.behaviors.slickgrid_delete = {attach: function(context, settings){
    if(Drupal.ajax.slickgrid_delete_form){
      Drupal.ajax.slickgrid_delete_form.beforeSend = function(xmlhttprequest, opts){
        opts.data = $.param({entity_type:options['entity_type'], entity_ids:slickgrid.getEntityIDs()}) + '&' + opts.data;
      }
    }
  }}
  function SlickGridDelete(grid, container){
    grid.onSelectedRowsChanged.subscribe(function(){
      if(grid.getSelectedRows().length) {
        $(container).children().children().addClass('enabled');
      } else {
        $(container).children().children().removeClass('enabled');
      }
    });
  }
  $.extend(true, window, {Slick: {Controls: {Delete: SlickGridDelete}}});
})(jQuery);