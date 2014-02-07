(function($){
  Drupal.CharacterMetadataManager = function(slickgrid){
    /**
     * init
     */
    this.init = function(){
      this.$slick = $(slickgrid.getContainer());
      this.metadata = []
      this.subscriptions = [$.proxy(this, 'updateCell')];
      // Bind to 'onSlickgridDataLoaded'
      this.$slick.bind('onSlickgridDataLoaded', $.proxy(this, 'slickgridDataLoaded'));
      // Subscribe to the context menu
      Drupal.characterContextMenu.subscribe($.proxy(this, 'contextMenu'));
    }
    
    /**
     * updateCellSubscribe
     */
    this.updateCellSubscribe = function(fn){
      this.subscriptions.push(fn);
    }
    
    /**
     * slickgridDataLoaded
     * 
     * Callback - read the metadata and remove it from the cell content.
     */
    this.slickgridDataLoaded = function(e, from, to, data){
      var char_columns = {};
      var slick_cols = slickgrid.getColumns(true);
      for (var c in slick_cols){
        if (slick_cols[c].id.match(/^character_\d+_\d+$/)){
          char_columns[slick_cols[c].id] = true;
        }
      }
      for (var i = from; i <= to; i++){
        var row = {};
        var col = 0;
        for (var column in data[i]){
          if (char_columns[column]){
            try{
              row[column] = {disabled: false};
              if (data[i][column] === ""){
                continue;
              }
              var decode = $.parseJSON(data[i][column]);
              if (decode !== null){
                if (typeof decode.disabled !== 'undefined' && decode.disabled){
                  data[i][column] = '';
                  row[column].disabled = true;
                } else {
                  if (typeof decode.data !== 'undefined'){
                    data[i][column] = decode.data;
                  }
                  if (typeof decode.value !== 'undefined'){
                    row[column].value = decode.value;
                  }
                  if (typeof decode.metadata !== 'undefined'){
                    for (var attr in decode.metadata){
                      if (decode.metadata[attr] == "0"){
                        row[column][attr] = false;
                      } else if (decode.metadata[attr] == "1"){
                        row[column][attr] = true;
                      } else {
                        row[column][attr] = decode.metadata[attr];
                      }
                    }
                  }
                }
              }
            } catch(e){
              /* NO OP */
            }
          }
        }
        this.metadata[i] = row;
      }
      // Once the data has been displayed we can assign the flags to the cells.
      grid.onViewportChanged.subscribe($.proxy(this, 'updateViewportRows'));
      window.setTimeout($.proxy(this, 'updateViewportRows'));
    }

    /**
     * updateViewportRows
     */
    this.updateViewportRows = function(){
      var vp = grid.getViewport();
      for (var i = vp.top; i <= vp.bottom; i++){
        for (var column in this.metadata[i]){
          if (this.metadata[i][column].disabled){
            var node = grid.getCellNode(i, grid.getColumnIndex(column));
            $(node).addClass('character-editor-disabled-cell');
          } else {
            var node = grid.getCellNode(i, grid.getColumnIndex(column));
            for (var s in this.subscriptions){
              (this.subscriptions[s])(this.metadata[i][column], node)
            }
          }
        }
      }
    }
    
    /**
     * updataCell
     */
    this.updateCell = function(metadata, node){
      if (metadata.flag && metadata.flag.length > 0){
        var flag = Drupal.settings.CharacterEditorFlags[metadata.flag];
        $(node).attr('character-flag', flag.abbr);
      } else {
        $(node).attr('character-flag', '');
      }
    }
    
    /**
     * contextMenu
     * 
     */
    this.contextMenu = function(info){
      if (info.cell.cell == 0){
        return [];
      }
      var cell_flag_id = this.metadata[info.cell.row][info.column.id].flag;
      if (cell_flag_id == 'computed' || cell_flag_id == 'inherited'){
        return [];
      }
      if (this.metadata[info.cell.row][info.column.id].disabled){
        return [];
      }
      var selected_background = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/tick.png")';
      var elements = [];
      elements.push({
        element: Drupal.t('Modifiers'),
        subtitle: true
      });
      for (var flag_id in Drupal.settings.CharacterEditorFlags){
        if (flag_id == 'computed' || flag_id == 'inherited'){
          continue;
        }
        var flag = Drupal.settings.CharacterEditorFlags[flag_id];
        elements.push({
          element: flag.flag,
          callback: $.proxy(this, 'contextClickCallback', info, flag),
          css: {
            backgroundImage: (cell_flag_id == flag_id) ? selected_background: '',
            backgroundRepeat: 'no-repeat',
            backgroundPosition: '0 center'
          }
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
      var cell_flag_id = this.metadata[info.cell.row][info.column.id].flag;
      if (cell_flag_id == selected_flag.id){
        this.metadata[info.cell.row][info.column.id].flag = '';
      } else {
        this.metadata[info.cell.row][info.column.id].flag = selected_flag.id;
      }
      this.updateCell(this.metadata[info.cell.row][info.column.id], node);
      // And send the data to be saved.
      slickgrid.callback('update', {
        entity_id: info.row.id,
        column_id: info.column.id,
        flag: cell_flag_id == selected_flag.id ? '' : selected_flag.id,
        plugin: 'CharacterMetadata'
      });
    }
    
    /**
     * getMetadata
     */
    this.getMetadata = function(row, column_id){
      if (typeof this.metadata[row] !== 'undefined' && typeof this.metadata[row][column_id] !== 'undefined'){
        return this.metadata[row][column_id];
      } else {
        return null;
      }
    }

    this.init();
  }
})(jQuery);