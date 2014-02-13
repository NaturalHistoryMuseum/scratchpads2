(function($){
  Drupal.CharacterInheritance = function(){
    /**
     * init
     */
    this.init = function(){
      // Subscribe to the context menu
      Drupal.characterContextMenu.subscribe($.proxy(this, 'contextMenu'));
      // Subscribe to the metadata manager
      Drupal.characterMetadataManager.updateCellSubscribe($.proxy(this, 'updateCell'));
    }
    
    /**
     * contextMenu
     * 
     */
    this.contextMenu = function(info){
      if (info.column.field == 'character_entity_field' || info.column.field == 'sel'){
        return [];
      }
      if (!info.row.id.match(/^taxonomy_term:/)){
        return [];
      }
      var metadata = Drupal.characterMetadataManager.getMetadata(info.cell.row, info.column.id);
      if (metadata.flag == 'computed' || metadata.disabled){
        return [];
      }      
      var pass_up = (typeof metadata.pass != 'undefined' && (metadata.pass == 'up' || metadata.pass == 'both'));
      var pass_down = (typeof metadata.pass != 'undefined' && (metadata.pass == 'down' || metadata.pass == 'both'));
      var selected_background = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/tick.png")';
      var elements = [];
      elements.push({
        element: Drupal.t('Values'),
        subtitle: true
      });
      elements.push({
        element: Drupal.t('get from descendants'),
        callback: $.proxy(this, 'contextClickCallback', info, metadata, 'up'),
        css: {
          backgroundImage: pass_up ? selected_background: '',
          backgroundRepeat: 'no-repeat',
          backgroundPosition: '0 center'
        }
      });
      elements.push({
        element: Drupal.t('pass down to descendants'),
        callback: $.proxy(this, 'contextClickCallback', info, metadata, 'down'),
        css: {
          backgroundImage: pass_down ? selected_background: '',
          backgroundRepeat: 'no-repeat',
          backgroundPosition: '0 center'
        }
      });
      return elements;
    }

    /**
     * contextClickCallback
     */
    this.contextClickCallback = function(info, metadata, type){
      // Update the cell
      var new_pass = '';
      if (typeof metadata.pass == 'undefined' || metadata.pass == ''){
        //metadata.pass = type;
        new_pass = type;
      } else if (type == metadata.pass){
        new_pass = '';
      } else if (metadata.pass == 'both'){
        if (type == 'up'){
          new_pass = 'down';
        } else {
          new_pass = 'up';
        }
      } else {
        new_pass = 'both'
      };
      // Apply the value to all selected cells
      var rows = grid.getSelectedRows();
      if ($.inArray(info.cell.row, rows) == -1){
        rows.push(info.cell.row);
      }
      for (var i = 0; i < rows.length; i++){
        var data = grid.getDataItem(rows[i]);
        if (!data.id.match(/^taxonomy_term/)){
          continue;
        }
        var cell_metadata = Drupal.characterMetadataManager.getMetadata(rows[i], info.column.id);
        cell_metadata.pass = new_pass;
        if (new_pass == '' && cell_metadata.flag == 'inherited'){
          cell_metadata.flag = '';
        }
        slickgrid.invalidateRow(rows[i]);
        var sel_node = grid.getCellNode(rows[i], info.cell.cell);
        Drupal.characterMetadataManager.updateCell(cell_metadata, sel_node);
        $(sel_node).css({
          backgroundImage: "url(" + Drupal.settings.basePath + "misc/throbber.gif)",
          backgroundRepeat: "no-repeat",
          backgroundPosition: "0 -20px",
        });
        // And send the data to be saved ; this may be different per row so we have to do them one by one
        slickgrid.callback('update', {
          entity_ids: [data.id],
          column_id: info.column.id,
          pass: cell_metadata.pass,
          flag: cell_metadata.flag,
          plugin: 'CharacterMetadata'
        });
      }
    }
    
    /**
     * updateCell
     */
    this.updateCell = function(metadata, node){
      var pass_up = (typeof metadata.pass != 'undefined' && (metadata.pass == 'up' || metadata.pass == 'both'));
      var pass_down = (typeof metadata.pass != 'undefined' && (metadata.pass == 'down' || metadata.pass == 'both'));
      var image = 'none';
      if (pass_up && pass_down){
        image = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/pass-up-down.png")';
      } else if (pass_up) {
        image = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/pass-up.png")';
      } else if (pass_down) {
        image = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/pass-down.png")';
      }
      $(node).css({
        backgroundImage: image,
        backgroundRepeat: 'no-repeat',
        backgroundPosition: '100% 0'
      });
    }

    this.init();
  }
})(jQuery);