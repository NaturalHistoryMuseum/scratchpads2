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
      if (typeof metadata.pass == 'undefined' || metadata.pass == ''){
        metadata.pass = type;
      } else if (type == metadata.pass){
        metadata.pass = '';
      } else if (metadata.pass == 'both'){
        if (type == 'up'){
          metadata.pass = 'down';
        } else {
          metadata.pass ='up';
        }
      } else {
        metadata.pass = 'both'
      };
      this.updateCell(metadata, grid.getCellNode(info.cell.row, info.cell.cell));
      // And send the data to be saved.
      slickgrid.callback('update', {
        entity_ids: slickgrid.getEntityIDs(info.row),
        column_id: info.column.id,
        pass: metadata.pass,
        plugin: 'CharacterMetadata'
      });
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