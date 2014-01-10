(function($){
  Drupal.CharacterContextMenu = function(slickgrid){
    /**
     * init
     */
    this.init = function(){
      // Setup
      this.plugins = [];
      this.setupContextHandler();
      this.addFlagPlugin();
    }
    
    /**
     * setupContextHandler
     * 
     * Add the context menu handler
     */
    this.setupContextHandler = function(){
      grid.onContextMenu.subscribe($.proxy(function(e){
        // Hide menu if already up
        $('#character-context-menu').remove();
        // Work out which cell we're working with
        var columns = slickgrid.getColumns();
        var info = {};
        info.cell = grid.getCellFromEvent(e);
        info.column = columns[info.cell.cell];
        info.row = grid.getDataItem(info.cell.row);
        info.value = info.row[info.column.id];
        // Call the plugins
        var elements = [];
        for (var i = 0; i < this.plugins.length; i++){
          var out = this.plugins[i](info);
          if ($.isArray(out)){
            elements.concat(out);
          } else if (typeof out !== 'undefined' && out){
            elements.push(out);
          }
        }
        // Display the menu
        if (elements.length > 0){
          e.preventDefault();
          var $menu = $('<div id="character-context-menu"></div>').css({
            position: 'absolute',
            zIndex: '100',
            left: e.pageX,
            top: e.pageY,
          });
          for (var i = 0; i < elements.length; i++){
            if (typeof elements[i].element == 'undefined'){
              continue;
            }
            var $elem = $('<div></div>').addClass('character-context-menu-row');
            if (typeof elements[i].element == 'string'){
              $elem.html(elements[i].element)
            } else {
              $elem.append($(elements[i].element));
            }
            if (typeof elements[i].callback !== 'undefined'){
              var that = this;
              (function(callback){
                $elem.click($.proxy(function(){
                  callback();
                  that.close();
                }, this));
              })(elements[i].callback);
            }
            $menu.append($elem);
          }
          $menu.appendTo('body').show();
          // Overlay for click-out
          $('<div id="character-context-menu-out"></div>').css({
            position: 'absolute',
            top: '0',
            left: '0',
            width: $(document).width().toString() + "px",
            height: $(document).height().toString() + "px",
            zIndex: '99'
          }).appendTo('body').one('click', $.proxy(function(e){
            this.close();
          }, this));
        }
      }, this));
    }
    
    /**
     * close
     */
    this.close = function(){
      $('#character-context-menu').remove();
      $('#character-context-menu-out').remove();
    }
    
    /**
     * addFlagPlugin
     * 
     * Add the flag plugin. This is done in it's own function so we can
     * eventually factor the functionality out easily.
     */
    this.addFlagPlugin = function(){
      /*
      this.subscribe(function(info){
        return {
          element: 'hello',
          callback: function(menu){
            console.log("Clicked!");
          }
        };
      });
      */
    }
    
    /**
     * subscribe
     * 
     * Add a plugin to this context menu. The passed element should be a function
     * which gets invoked when the context menu pops up. The function is called with
     * an object defining:
     * 
     * cell:   an object defining row and column of the current element ;
     * column: The slickgrid column definition of the current column ;
     * row:    The slickgrid data row of the current row ;
     * value:  The value of the current element
     * 
     * The function may return false, or an object (or array of such objects) each
     * defining:
     * element: either a string, a DOM element or a jQuery element ;
     * callback: a function to call when the element is clicked
     */
    this.subscribe = function(fn){
      this.plugins.push(fn);
    }
    
    this.init();
  }
})(jQuery);