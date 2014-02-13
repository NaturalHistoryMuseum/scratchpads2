(function($){
  var SlickEditor = {
  InlineCell: function(args){
    var $input;
    var defaultValue;
    var scope = this;
    this.init = function(){
      $input = $("<INPUT type=text class='editor-text' />").appendTo(args.container).bind("keydown.nav", function(e){
        if(e.keyCode === $.ui.keyCode.LEFT || e.keyCode === $.ui.keyCode.RIGHT) {
          e.stopImmediatePropagation();
        }
      }).focus().select();
    };
    this.destroy = function(){
      $input.remove();
    };
    this.focus = function(){
      $input.focus();
    };
    this.getValue = function(){
      return $input.val();
    };
    this.setValue = function(val){
      $input.val(val);
    };
    this.loadValue = function(item){
      defaultValue = item[args.column.field] || "";
      $input.val(defaultValue);
      $input[0].defaultValue = defaultValue;
      $input.select();
    };
    this.serializeValue = function(){
      return $input.val();
    };
    this.applyValue = function(item, value){
      var data = {
      // Data to be passed to the backend
      display_id: slickgrid.getViewDisplayID(), view: slickgrid.getViewName(), entity_ids: slickgrid.getEntityIDs(item), field_id: args.column.id, field_name: args.column.fieldName, entity_type: options['entity_type'], revision: options['undo'], value: value, plugin: 'InlineCell'};
      // Perform the update
      slickgrid.callback('update', data);
      // And update the cell
      var column_index = grid.getColumnIndex(args.column.id);
      var invalid_rows = slickgrid.invalidateSelectedRows();
      for (var i = 0; i < invalid_rows.length; i++){
        $(args.grid.getCellNode(rows_to_invalidate[i], column_index)).addClass('slickgrid-cell-loading');
        var row_item = grid.getDataItem(invalid_rows[i]);
        row_item[args.column.id] = value;
        grid.updateCell(invalid_rows[i], column_index);
      }
    }
    this.isValueChanged = function(){
      return (!($input.val() == "" && defaultValue == null)) && ($input.val() != defaultValue);
    };
    this.validate = function(){
      if(args.column.validator) {
        var validationResults = args.column.validator($input.val(), $input);
        if(!validationResults.valid) return validationResults;
      }
      return {valid: true, msg: null};
    };
    this.init();
  },
  /*
   * This opens the field from the entity form in a popup This *should* work for
   * all field & entity types If it doesn't work for a particular field, raise
   * an issue
   */
  ModalForm: function(args){
    var scope = this;
    var defaultValue;
    var state;
    // Attach Drupal.ajax to the slickgrid element
    var element = $('#slickgrid');
    this.init = function(){
      // Open a CTools modal dialog
      Drupal.CTools.Modal.show('ctools-modal-slickgrid-fixed');
      // Replace the default close event
      $('a.close').unbind('click').bind('click', scope.cancel);
      // Set up the ajax functionality
      var base = 'ctools-modal-slickgrid';
      // Ajax settings
      var element_settings = {event: 'modal',
      // Drupal.ajax needs an event to fire it - use modal as it should be
      // harmless
      url: Drupal.settings.slickgrid.slickgrid_callback_url + 'update', submit: {
      // Data to be passed to the backend
      js: true, display_id: slickgrid.getViewDisplayID(), view: slickgrid.getViewName(), entity_ids: slickgrid.getEntityIDs(args.item), field_id: args.column.id, field_name: args.column.fieldName, entity_type: options['entity_type'], revision: options['undo'], plugin: 'ModalForm'}};
      Drupal.ajax[base] = new Drupal.ajax(base, element[0], element_settings);
      // Trigger the ready event so start the callback
      element.trigger('modal');
    };
    this.loadValue = function(item){
      defaultValue = item[args.column.field] || "";
    };
    this.serializeValue = function(){
      return defaultValue;
    };
    this.isValueChanged = function(){
      return(state !== defaultValue);
    };
    this.applyValue = function(item){
      var column_index = grid.getColumnIndex(args.column.id);
      var invalid_rows = slickgrid.invalidateSelectedRows();
      for (var i = 0; i < invalid_rows.length; i++){
        $(args.grid.getCellNode(rows_to_invalidate[i], column_index)).addClass('slickgrid-cell-loading');
        var row_item = grid.getDataItem(invalid_rows[i]);
        row_item[args.column.id] = "";
        grid.updateCell(invalid_rows[i], column_index);
      }
    };
    this.cancel = function(){
      args.cancelChanges();
    }
    this.stop = function(){
    }
    this.destroy = function(){
      element.unbind('modal');
      Drupal.CTools.Modal.dismiss();
    }
    // Validation happens server side
    this.validate = function(){
      return {valid: true, msg: null};
    }
    this.init();
  }
  };
  $.extend(window, SlickEditor);
})(jQuery);
