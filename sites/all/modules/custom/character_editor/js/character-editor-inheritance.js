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
      var metadata = Drupal.characterMetadataManager.getMetadata(info.cell.row, info.column.id);
      var send_up = (typeof metadata.sendUp != 'undefined' && metadata.sendUp);
      var send_down = (typeof metadata.sendDown != 'undefined' && metadata.sendDown);
      if (metadata.flag == 'computed' || metadata.disabled){
        return [];
      }
      var selected_background = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/tick.png")';
      var elements = [];
      elements.push({
        element: Drupal.t('Inheritance'),
        subtitle: true
      });
      elements.push({
        element: Drupal.t('send values up'),
        callback: $.proxy(this, 'contextClickCallback', info, metadata, 'sendUp', send_up),
        css: {
          backgroundImage: send_up ? selected_background: '',
          backgroundRepeat: 'no-repeat',
          backgroundPosition: '0 center'
        }
      });
      elements.push({
        element: Drupal.t('send values down'),
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
    this.contextClickCallback = function(info, metadata, direction, current){
      // Update the cell
      this.updateCell(metadata, grid.getCellNode(info.cell.row, info.cell.cell));
      metadata[direction] = !current;
      // And send the data to be saved.
      slickgrid.callback('update', {
        entity_id: info.row.id,
        column_id: info.column.id,
        send_up: (typeof metadata.sendUp != 'undefined' && metadata.sendUp) ? 1 : 0,
        send_down: (typeof metadata.sendDown != 'undefined' && metadata.sendDown) ? 1 : 0,
        plugin: 'CharacterMetadata'
      });
    }
    
    /**
     * updateCell
     */
    this.updateCell = function(metadata, node){
      var send_up = (typeof metadata.sendUp != 'undefined' && metadata.sendUp);
      var send_down = (typeof metadata.sendDown != 'undefined' && metadata.sendDown);
      var image = 'none';
      if (send_up && send_down){
        image = 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/pass-up-down.png")';
      } else if (send_up) {
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