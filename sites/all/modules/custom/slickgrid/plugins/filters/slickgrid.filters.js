/**
 * Slick filters
 */


/**
*************************** BASIC TEXT FILTER ***************************
*/

 (function($) {

    // Slick.Controls.Pager
    $.extend(true, window, {
        Slick: {
            Filters: {
                textFilter: textFilter
            }
        }
    });

    function textFilter()
    {
        function input() {
            // Return an input as a jquery object
            return $("<input type='text'>");
        }

        function doFilter(item, field, search) {
          
            // Perform the actual filtering
            if(item[field].toLowerCase().indexOf(search.toLowerCase()) === -1){
              return false;
            }
            
            return true;
            
        }

        // Public API
        
        $.extend(this, {
            "input": input,
            "doFilter": doFilter
        });

    }


})(jQuery);