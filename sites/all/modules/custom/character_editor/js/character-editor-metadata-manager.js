(function($){
  Drupal.CharacterMetadataManager = function(slickgrid){
    /**
     * init
     */
    this.init = function(){
      this.$slick = $(slickgrid.getContainer());
      this.metadata = []
      this.updatedCells = {
          from: -1,
          to: -1
      };
      this.selectedRows = [];
      this.subscriptions = [$.proxy(this, 'updateCellFlag')];
      // Bind to 'onSlickgridDataLoaded'
      this.$slick.bind('onSlickgridDataLoaded', $.proxy(this, 'slickgridDataLoaded'));
      // Subscribe to the context menu
      Drupal.characterContextMenu.subscribe($.proxy(this, 'contextMenu'));
      // React when rows are selected to restore metadata
      grid.onSelectedRowsChanged.subscribe($.proxy(this, 'selectedRowsChanged'));
      grid.onViewportChanged.subscribe($.proxy(this, 'viewportChanged'));
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
      window.setTimeout($.proxy(this, 'updateViewportRows'));
    }

    /**
     * viewportChanged
     */
    this.viewportChanged = function(){
      this.updateViewportRows();
    }

    /**
     * updateViewportRows
     */
    this.updateViewportRows = function(range){
      if (typeof(range) == 'undefined'){
        var vp = grid.getViewport();
        range = {
          from: vp.top,
          to: vp.bottom
        };
        var new_range = range;
        if (this.updatedCells.from <= range.from && this.updatedCells.to >= range.to){
          return;
        }
        if (this.updatedCells.from <= range.from && this.updatedCells.to > range.from){
          new_range = {
            from: this.updatedCells.to,
            to: range.to
          };
        } else if (this.updatedCells.from <= range.to && this.updatedCells.to >= range.to){
          new_range = {
            from: range.from,
            to: this.updatedCells.from
          };
        } else {
          // We should udpate the whole range. When this happens though it's possible
          // slickgrid hasn't actually displayed the cells yet - so we must delay this
          // update.
          if (typeof this.timer != 'undefined'){
            clearTimeout(this.timer);
          }
          this.timer = window.setTimeout($.proxy(this, 'updateViewportRows', range), 50);
          return;
        }
        this.updatedCells = range;
        range = new_range;
      } else {
        this.updatedCells = range;
      }
      for (var i = range.from; i <= range.to; i++){
        for (var column in this.metadata[i]){
          if (this.metadata[i][column].disabled){
            var node = grid.getCellNode(i, grid.getColumnIndex(column));
            $(node).addClass('character-editor-disabled-cell');
          } else {
            var node = grid.getCellNode(i, grid.getColumnIndex(column));
            this.updateCell(this.metadata[i][column], node);
          }
        }
      }
    }
    
    /**
     * updateCell
     */
    this.updateCell = function(metadata, node){
      for (var s in this.subscriptions){
        (this.subscriptions[s])(metadata, node);
      }
    }

    /**
     * updateCellFlag
     */
    this.updateCellFlag = function(metadata, node){
      if (metadata.flag && metadata.flag.length > 0){
        var flag = Drupal.settings.CharacterEditorFlags[metadata.flag];
        $(node).attr('character-flag', flag.abbr);
      } else {
        $(node).attr('character-flag', '');
      }
    }
    
    /**
     * selectedRowsChanged
     */
    this.selectedRowsChanged = function(event, data){
      var rows_to_update = this.selectedRows.concat(data.rows);
      var range = {
        from: Math.min.apply(Math, rows_to_update),
        to: Math.max.apply(Math, rows_to_update)
      };
      this.updateViewportRows(range);
      this.selectedRows = data.rows.slice();
    }

    /**
     * contextMenu
     * 
     */
    this.contextMenu = function(info){
      if (info.column.field == 'character_entity_field' || info.column.field == 'sel'){
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
        element: Drupal.t('Annotations'),
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
      // Get the value
      var cell_flag_id = this.metadata[info.cell.row][info.column.id].flag;
      var update_value = '';
      if (cell_flag_id != selected_flag.id){
        update_value = selected_flag.id;
      }
      // Apply the value to all selected cells
      var rows = grid.getSelectedRows();
      rows.push(info.cell.row);
      for (var i = 0; i < rows.length; i++){
        this.metadata[rows[i]][info.column.id].flag = update_value;
        slickgrid.invalidateRow(rows[i]);
        var sel_node = grid.getCellNode(rows[i], info.cell.cell);
        this.updateCell(this.metadata[rows[i]][info.column.id], sel_node);
        $(sel_node).css({
          backgroundImage: "url(" + Drupal.settings.basePath + "misc/throbber.gif)",
          backgroundRepeat: "no-repeat",
          backgroundPosition: "0 -20px",
        });
      }
      // And send the data to be saved.
      slickgrid.callback('update', {
        entity_ids: slickgrid.getEntityIDs(info.row),
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