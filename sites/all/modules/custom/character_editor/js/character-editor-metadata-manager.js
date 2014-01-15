(function($){
  Drupal.CharacterMetadataManager = function(slickgrid){
    /**
     * init
     */
    this.init = function(){
      this.$slick = $(slickgrid.getContainer());
      this.metadata = []
      // Bind to 'onSlickgridDataLoaded'
      this.$slick.bind('onSlickgridDataLoaded', $.proxy(this, 'slickgridDataLoaded'));
      // Subscribe to the context menu
      Drupal.characterContextMenu.subscribe($.proxy(this, 'contextMenu'));
    }
    
    /**
     * slickgridDataLoaded
     * 
     * Callback - read the metadata and remove it from the cell content.
     */
    this.slickgridDataLoaded = function(e, from, to, data){
      for (var i = from; i <= to; i++){
        var row = {};
        var col = 0;
        console.log(data);
        for (var column in data[i]){
          if (column.match(/^character_\d+_\d+$/)){
            var match = data[i][column].match(/^(?:([^:]*):)?([\s\S]*)$/);
            if (match){
              row[column] = match[1];
              data[i][column] = match[2];
            }
          }
        }
        this.metadata[i] = row;
      }
      // Once the data has been displayed we can assign the flags to the cells.
      window.setTimeout($.proxy(function(){
        for (var i = 0; i < this.metadata.length; i++){
          for (var column in this.metadata[i]){
            if (this.metadata[i][column] && this.metadata[i][column].length > 0){
              var node = grid.getCellNode(i, grid.getColumnIndex(column));
              var flag = Drupal.settings.CharacterEditorFlags[this.metadata[i][column]];
              $(node).attr('character-flag', flag.abbr);
            }
          }
        }
      }, this), 0);
    }
    
    /**
     * contextMenu
     * 
     */
    this.contextMenu = function(info){
      if (info.cell.cell == 0){
        return [];
      }
      var elements = [];
      var cell_flag_id = this.metadata[info.cell.row][info.column.id];
      if (cell_flag_id == 'computed' || cell_flag_id == 'inherited'){
        return [];
      }
      for (var flag_id in Drupal.settings.CharacterEditorFlags){
        if (flag_id == 'computed' || flag_id == 'inherited'){
          continue;
        }
        var flag = Drupal.settings.CharacterEditorFlags[flag_id];
        elements.push({
          element: flag.flag + (cell_flag_id == flag_id ? ' v' : ''),
          callback: $.proxy(this, 'contextClickCallback', info, flag)
        });
      }
      return elements;
    }

    /**
     * contextClickCallback
     */
    this.contextClickCallback = function(info, selected_flag){
      // Update the cell
      var node = grid.getCellNode(info.cell.row, info.cell.cell);
      var cell_flag_id = this.metadata[info.cell.row][info.column.id];
      if (cell_flag_id == selected_flag.id){
        $(node).attr('character-flag', '');
        this.metadata[info.cell.row][info.column.id] = '';
      } else {
        $(node).attr('character-flag', selected_flag.abbr);
        this.metadata[info.cell.row][info.column.id] = selected_flag.id;
      }
      // And send the data to be saved.
      console.log(info);
      slickgrid.callback('update', {
        entity_id: info.row.id,
        column_id: info.column.id,
        flag: selected_flag.id,
        plugin: 'CharacterFlag'
      });
    }
    
    this.init();
  }
})(jQuery);