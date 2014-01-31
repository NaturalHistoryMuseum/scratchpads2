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
      if (info.cell.cell == 0){
        return [];
      }
      if (!info.row.id.match(/^taxonomy_term:/)){
        return [];
      }
      var metadata = Drupal.characterMetadataManager.getMetadata(info.cell.row, info.column.id);
      var aggregate = (typeof metadata.aggregate != 'undefined' && metadata.aggregate);
      var send_down = (typeof metadata.sendDown != 'undefined' && metadata.sendDown);
      if (metadata.flag == 'computed' || metadata.disabled){
        return [];
      }
      var selected_background = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/tick.png")';
      var elements = [];
      elements.push({
        element: Drupal.t('Values'),
        subtitle: true
      });
      elements.push({
        element: Drupal.t('get from descendants'),
        callback: $.proxy(this, 'contextClickCallback', info, metadata, 'aggregate', aggregate),
        css: {
          backgroundImage: aggregate ? selected_background: '',
          backgroundRepeat: 'no-repeat',
          backgroundPosition: '0 center'
        }
      });
      elements.push({
        element: Drupal.t('pass down to descendants'),
        callback: $.proxy(this, 'contextClickCallback', info, metadata, 'sendDown', send_down),
        css: {
          backgroundImage: send_down ? selected_background: '',
          backgroundRepeat: 'no-repeat',
          backgroundPosition: '0 center'
        }
      });
      return elements;
    }

    /**
     * contextClickCallback
     */
    this.contextClickCallback = function(info, metadata, type, current){
      // Update the cell
      this.updateCell(metadata, grid.getCellNode(info.cell.row, info.cell.cell));
      metadata[type] = !current;
      // And send the data to be saved.
      slickgrid.callback('update', {
        entity_id: info.row.id,
        column_id: info.column.id,
        aggregate: (typeof metadata.aggregate != 'undefined' && metadata.aggregate) ? 1 : 0,
        send_down: (typeof metadata.sendDown != 'undefined' && metadata.sendDown) ? 1 : 0,
        plugin: 'CharacterMetadata'
      });
    }
    
    /**
     * updateCell
     */
    this.updateCell = function(metadata, node){
      var aggregate = (typeof metadata.aggregate != 'undefined' && metadata.aggregate);
      var send_down = (typeof metadata.sendDown != 'undefined' && metadata.sendDown);
      var image = 'none';
      if (aggregate && send_down){
        image = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/pass-up-down.png")';
      } else if (aggregate) {
        image = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/pass-up.png")';
      } else if (send_down) {
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