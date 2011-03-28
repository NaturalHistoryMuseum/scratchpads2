/**
* Filter & Formatter for collapsible taxonomy fields
*/

/**
*************************** COLLAPSIBLE FORMATTER ***************************
*/

var collapsibleFormatter = function(row, cell, value, columnDef, dataContext) {
    var spacer = "<span style='display:inline-block;height:1px;width:" + (15 * dataContext["indent"]) + "px'></span>";
    var idx = dataView.getIdxById(dataContext.id);
	if (data[idx+1] && data[idx+1].indent > data[idx].indent) {
		if (dataContext._collapsed)
			return spacer + " <span class='toggle expand'></span>&nbsp;" + value;
		else
			return spacer + " <span class='toggle collapse'></span>&nbsp;" + value;
	}
	else
		return spacer + " <span class='toggle'></span>&nbsp;" + value;
};

/**
*************************** COLLAPSIBLE FILTER ***************************
*/

 (function($) {

    // Slick.Controls.Pager
    $.extend(true, window, {
        Slick: {
            Filters: {
                collapsibleFilter: collapsibleFilter
            }
        }
    });

    function collapsibleFilter()
    {

        function doFilter(item, field, search) {
        
            
          if (item.parent != null) {
            
           var parent = dataView.getItemById(item.parent);
          
           if(typeof parent == 'object' && parent._collapsed){
             return false;
           }
          
           
          }

    			return true;
            
          }

        // Public API
        
        $.extend(this, {
            "doFilter": doFilter
        });

    }


})(jQuery);