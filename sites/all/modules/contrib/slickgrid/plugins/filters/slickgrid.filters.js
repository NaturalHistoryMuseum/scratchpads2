(function($){
  function textFilter(field){
    function input(){
      // Return an input as a jquery object
      return $("<input type='text'>");
    }
    function doFilter(item, search){
      // Perform the actual filtering
      if(item[field].toLowerCase().indexOf(search.toLowerCase()) === -1) {
        return false;
      }
      return true;
    }
    // Public API
    $.extend(this, {"input": input, "doFilter": doFilter});
  }
  function collapsibleFilter(field){
    function init(){
      grid.onClick.subscribe(function(e, args){
        if($(e.target).hasClass("toggle")) {
          // ensure the filters know this column is selected
          slickgrid.setColumnFilter(field, true);
          var item = dataView.getItem(args.row);
          if(item) {
            if(!item._collapsed)
              item._collapsed = true;
            else
              item._collapsed = false;
            dataView.updateItem(item.id, item);
          }
          // Ensure this doesn't screw with the header filters
          slickgrid.setColumnFilter(field, null);
          e.stopImmediatePropagation();
        }
      });
    }
    function doFilter(item, search){
      if(item.parent != null) {
        var parent = dataView.getItemById(item.parent);
        if(typeof parent == 'object' && parent._collapsed) {
          return false;
        }
      }
      return true;
    }
    init();
    // Public API
    $.extend(this, {"doFilter": doFilter});
  }
  // Slick.Filter.textFilter
  $.extend(true, window, {Slick: {Filter: {Text: textFilter, Collapsible: collapsibleFilter}}});
})(jQuery);