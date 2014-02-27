// Grid & dataView need to be globals so they can be accessed from formatters
// etc.,
var grid;
var undoControl;
if(!Array.prototype.indexOf) {
  Array.prototype.indexOf = function(obj, start){
    for( var i = (start || 0), j = this.length; i < j; i++) {
      if(this[i] === obj) {
        return i;
      }
    }
    return -1;
  }
}
(function($){
  // register namespace
  $.extend(true, window, {Slickgrid: Slickgrid});
  // Loading indicator div.
  var loadingIndicator = null;
  // Slickgrid class implementation
  function Slickgrid(container, viewName, viewDisplayID){
    var PAGESIZE = 50;
    var columnFilters = {};
    var checkboxSelector;
    var $dialog; // $dialog (at the moment a beautytips instance)
    var activeRow; // The row currently being edited
    var commandHandler;
    var locked;
    var loader;
    // Remove
    var $status; // $status container for result icons & messages & loading ic
    // Controls
    var tabs;
    function init(){
      // Set up an ajax commmand to handle any modal form responses
      Drupal.ajax.prototype.commands.slickgrid = handleModalResponse;
      $status = $('#slickgrid-status');
      // Add row checkboxes if multi edit, delete or clone is enabled
      if(options['row_selection_checkbox']) {
        // Init row checkboxes - needs to be done before the grid is initiated
        // as a column needs to be added
        checkboxSelector = new Slick.CheckboxSelectColumn({cssClass: "slick-cell-checkboxsel"});
        // Add the selector column
        columns.unshift(checkboxSelector.getColumnDefinition());
      }
      // Is undo enabled? If it is, add an editCommandHandler
      if(options['undo']) {
        // Undo needs to be initiated prior to building the grid & dataview - it
        // adds an editCommandHandler option (see init())
        undoControl = new Slick.Controls.Undo($("#slickgrid-undo"));
      }
      // Initialise the remotemodel & slickgrid
      commandHandler = new Slick.Data.CommandHandler();
      loader = new Slick.Data.RemoteModel(viewName, commandHandler);
      $.extend(true, options, {defaultFormatter: function(row, cell, value, columnDef, dataContext){
        if(value == null) {
          return "";
        } else {
          return value;
        }
      }});
      // Make sure this is switched on otherwise slickgrid will not generate it
      options.showHeaderRow = true;
      grid = new Slick.Grid(container, loader.data, columns, options);
      grid.setHeaderRowVisibility(false); // Now hide it - filters will take of this.
      // Load the data when the scroll bar is touched (etc).
      grid.onViewportChanged.subscribe(function(e){
        var vp = grid.getViewport();
        if (vp.bottom >= loader.data.length - 1 && !loader.allRowsLoaded()){
          displayLoadingIndicator();
        }
        ensureViewportData(false);
      });
      // Are sortable columns enabled?
      // Sortable columns won't work with collapsible taxonomy fields
      if(options['sortable_columns']) {
        grid.onSort.subscribe(function(e, args){
          loader.setSort(args.sortCol.field, args.sortAsc ? 1 : -1);
          var vp = grid.getViewport();
          loader.ensureData(vp.top, vp.bottom);
        });
      }
      loader.onDataLoaded.subscribe(function(e, args){
        $(container).trigger('onSlickgridDataLoaded', [args.from, args.to, loader.data]);
        for( var i = args.from; i <= args.to; i++) {
          if (!loader.isRowLocked(i)){
            grid.invalidateRow(i);
          }
        }
        grid.updateRowCount();
        grid.render();
        // FIXME - The loading indicator is causing the horizontal scroll bar
        // to appear on screen.
        hideLoadingIndicator();
      });
      // load the first page
      grid.onViewportChanged.notify();
      displayLoadingIndicator();
      // Add all the controls
      // delete control (requires row selection checkbox)
      if(options['delete'] && options['row_selection_checkbox']) {
        var deleteControl = new Slick.Controls.Delete(grid, $("#slickgrid-delete"));
      }
      // export control
      if(options['export']) {
        var exportControl = new Slick.Controls.Export(grid, $("#slickgrid-export"));
      }
      // export control (requires row selection checkbox)
      if(options['clone'] && options['row_selection_checkbox']) {
        var cloneControl = new Slick.Controls.Clone(grid, $("#slickgrid-clone"));
      }
      // Users can show / hide columns
      if(options['select_columns']) {
        var columnpicker = new Slick.Controls.ColumnPicker(columns, grid, options);
      }
      // Are there hidden columns?
      if(options['hidden_columns']) {
        initHiddenColumns();
      }
      // Add tabs control (Needs to come after columnpicker control & hidden
      // columns is added)
      if(options['tabs']) {
        tabs = new Slick.Controls.Tabs(loader, grid, $("#slickgrid-tabs"), options['default_active_tab']);
      }
      if(options['default_filter']) {
        setColumnFilter(options['default_filter']['field'], options['default_filter']['value']);
      }
      // Does the grid have filters that need adding?
      if(options['filterable']) {
        initFilters(loader);
      }
      grid.setSelectionModel(new Slick.RowSelectionModel({selectActiveRow: false
      // Do not select active row
      // Going to handle this ourselves (making it selected) so user can select
      // & edit multiple items
      // Otherwise all rows will be deselected on edit
      }));
      // Ensure links are not followed when clicking on cell in edit mode
      grid.onClick.subscribe(function(eventData, gridData){
        if(options.editable) {
          eventData.preventDefault();
        }
      });
      // If row checkboxes are enabled, add row selection to the grid & register
      // the plugin
      if(checkboxSelector) {
        grid.registerPlugin(checkboxSelector);
        grid.onSelectedRowsChanged.subscribe(handleSelectedRowsChanged);
      }
      // Register events for my handling of active rows
      grid.onBeforeEditCell.subscribe(handleBeforeEditCell);
      grid.onBeforeCellEditorDestroy.subscribe(handleBeforeCellEditorDestroy);
      // If a grouping field has been chosen, group the data
      if(options['grouping_field']) {
        initGroups();
      }
      // Add the collapsible taxonomy field
      if(options['collapsible_taxonomy_field']) {
        initCollapsibleTaxonomyField(options['collapsible_taxonomy_field']);
      }
      grid.onColumnsReordered.subscribe(handleColumnsReordered);
      // There isn't a grid event when a column is shown / hidden - tag one onto
      // onHeaderContextMenu()
      grid.onHeaderContextMenu.subscribe(handleHeaderContextMenu);
      grid.onColumnsResized.subscribe(handleColumnsResized);
      grid.onValidationError.subscribe(handleValidationError);
      // Add resizable callback event
      $('#slickgrid').resizable({handles: 's', stop: function(e, ui){
        if(ui.originalSize.height != ui.size.height) {
          handleViewportResized(ui.size.height);
        }
      }});
      // We dynamically increase the height of the grid so that it fills the
      // window.
      if(options['height_expand']) {
        // At this point we do not usually have the margin-top for the body, so
        // the "100" has been added to allow for that.
        // (window.height - (top offset + header and footer height + body top \
        // margin + 100)) - current height
        var increase_by = ($(window).height() - ($('#slickgrid').offset().top + ($('#slickgrid').parent().height() - $('#slickgrid').height() + parseInt($('body').css('marginTop')) + 100))) - $('#slickgrid').height();
        if(increase_by > 20) {
          $('#slickgrid').animate().height($('#slickgrid').height() + increase_by);
        }
      }
    }
    function handleValidationError(eventData, error){
      alert(Drupal.t('There has been an error, please reload the page.'))
    }
    function handleSelectedRowsChanged(){
      closeDialog();
    }
    // Display the loading indicator
    function displayLoadingIndicator(){
      if(!loadingIndicator) {
        loadingIndicator = $('<div class="loading-indicator"><div><img src="' + Drupal.settings.slickgrid.loading_image_url + '"/></div></div>').appendTo(document.body);
        loadingIndicator.css("position", "absolute");
        // FIXME - The following "position" command may or may not be
        // required.
        // This feels like a similar issue to what we were having with the
        // toolbar module.
        $('body').css('position', 'relative');
        loadingIndicator.css("top", $(container).offset().top);
        loadingIndicator.css("left", $(container).offset().left);
        loadingIndicator.css("width", $(container).width());
        loadingIndicator.css("height", $(container).height());
        loadingIndicator.css("background-color", "#232323");
        loadingIndicator.css("z-index", 100000);
        loadingIndicator.css("border", "solid 1px #232323");
        loadingIndicator.fadeTo("fast", 0.3);
        loadingIndicator.children().css("position", "relative");
        loadingIndicator.children().css('top', (loadingIndicator.height() / 2) - 24);
        loadingIndicator.children().css('left', (loadingIndicator.width() / 2) - 24);
        // We set the header row as initially visible so that the height of
        // the grid is set correctly.
        grid.resizeCanvas();
      }
      if (loadingIndicator.css('display') == 'none'){
        loadingIndicator.fadeIn();
      }
    }
    // Hide the loading indicator
    function hideLoadingIndicator(){
      if (loadingIndicator && loadingIndicator.css('display') == 'block'){
        loadingIndicator.fadeOut();
      }
    }
    // Ensure we have the viewport data available
    function ensureViewportData(reset){
      var vp = grid.getViewport();
      if (reset){
        vp.bottom = vp.bottom - vp.top;
        vp.top = 0;
        grid.scrollRowToTop(0);
      }
      var page_size = Math.max(Math.floor((vp.bottom - vp.top + 11)/10)*10, 20);
      var start = vp.top <= page_size ? 0 : Math.floor((vp.top - 1 - page_size)/page_size)*page_size;
      var end = Math.floor((vp.bottom+1+2*page_size)/page_size)*page_size;
      if (start > 1){
        //loader.removeRange(0, start - 1);
      }
      if (end < loader.data.length - 1){
        //loader.removeRange(end + 1, loader.data.length - 1)
      }
      loader.ensureData(start, end);
    }
    // User has reodered the columns - save it to the backend
    function handleColumnsReordered(e, ui){
      var orderedColumns = [];
      // This event is firing when columns have been dragged slightly but
      // returned to same position
      // grid.getColumns() has the new order, while the global columns retains
      // the old order
      // set orderActuallyChanged to true when looping through if the order
      // really has changed
      var orderActuallyChanged = false;
      $(grid.getColumns()).each(function(i, col){
        orderedColumns.push(col['id']);
        if(!orderActuallyChanged && col['id'] != columns[i]['id']) {
          orderActuallyChanged = true;
        }
      });
      if(orderActuallyChanged) {
        updateSettings('ordered_columns', orderedColumns);
      }
    }
    // User has shown / hidden a column - save it to the backend
    function handleColumnsChanged(){
      var hiddenColumns = [];
      $('input', '.slick-columnpicker').each(function(i, e){
        if(columns[i]) {
          if($(this).is(":visible")) {
            if($(this).is(":checked")) {
              columns[i]['hidden'] = false;
            } else {
              columns[i]['hidden'] = true;
            }
          }
        }
      });
      $(columns).each(function(i, col){
        if(col.hidden) {
          hiddenColumns.push(col.id);
        }
      });
      updateSettings('hidden_columns', hiddenColumns);
      // Add column filters back into the grid
      if(options['has_filters']) {
        // FIXME - This may need to pass loader.
        initFilters();
      }
    }
    // The context menu (choosing which columns to display) has been opened
    function handleHeaderContextMenu(e, ui){
      // If multi edit is anebled we want to hide the first column from the user
      // as its the checkbox select one
      if(options['multi_edit']) {
        $('.slick-columnpicker li').eq(0).hide()
      }
      // User has changed the columns
      $('input', '.slick-columnpicker').change(handleColumnsChanged);
      // Auto resize does not fire column resize - so need to call it manually
      $('#autoresize').change(function(){
        handleAutoResize($(this).is(':checked') ? 1 : 0);
      });
    }
    // Columns have been resized
    function handleColumnsResized(e, ui){
      // Need to save width of ALL changed columns - if auto resize is on it
      // won't just be the resized column that has changed
      var resizedColumns = {};
      $(grid.getColumns()).each(function(i, col){
        resizedColumns[col['id']] = col.width;
      });
      updateSettings('column_width', resizedColumns);
    }
    // Viewport has been resized
    function handleViewportResized(height){
      updateSettings('viewport_height', height);
    }
    function handleAutoResize(value){
      updateSettings('forceFitColumns', value);
    }
    // User has started editing a cell
    // Need to add row to the selected rows
    function handleBeforeEditCell(e, ui){
      var row = parseInt(ui.row);
      if (locked || loader.isRowLoading(row) || loader.isRowInvalid(row)){
        return false;
      }
      loader.lockRow(row);
      setActiveRow(ui.row);
      cellNode = grid.getCellNode(ui.row, ui.cell);
      $(cellNode).removeClass('invalid');
    }
    // User has stopped editing a cell
    // Deselect the row being actively edited
    function handleBeforeCellEditorDestroy(event, args){
      var row = parseInt(args.grid.getActiveCell().row);
      loader.unlockRow(row);
      unsetActiveRow();
    }
    // Invalidate a row so we know to reload it
    function invalidateRow(row){
      loader.invalidateRange(row, row);
    }
    // Invalidate all selected rows, and return the
    // list of rows that were invalidated.
    function invalidateSelectedRows(){
      var rows_to_invalidate = grid.getSelectedRows().slice();
      var active_cell = grid.getActiveCell();
      if (active_cell !== null && $.inArray(active_cell.row, rows_to_invalidate) == -1){
        rows_to_invalidate.push(active_cell.row);
      }
      for (var i = 0; i < rows_to_invalidate.length; i++){
        loader.invalidateRange(rows_to_invalidate[i], rows_to_invalidate[i]);
      }
      return rows_to_invalidate;
    }
    function handleModalResponse(ajax, modal, status){
      if (grid.getActiveCell() !== null){
        grid.getEditController().commitCurrentEdit();
      }
      if(typeof modal.response.result === 'object') {
        callbackSuccess(modal.response.result);
      }
    }
    function setActiveRow(row){
      $('div[row="' + row + '"]').addClass('active-row');
    }
    function unsetActiveRow(){
      var cell = grid.getActiveCell();
      $('div[row="' + cell.row + '"]').removeClass('active-row');
    }
    // Get an array of all entity IDs (eg NIDs) to update
    // Pass in the active item, so this won't get added twice but will
    // definitely get added if not selected
    function getEntityIDs(item){
      var entityIDs = [];
      // Item isn't required - delete for example doesn't have any active item
      if(typeof item != 'undefined') {
        entityIDs.push(item.id);
      }
      $.each(grid.getSelectedRows(), function(i, row){
        // Retrieve the data item of the selected row
        var selected_item = loader.data[row];
        if(entityIDs.indexOf(selected_item.id) === -1) {
          // Add the id to the edited ids object
          entityIDs.push(selected_item.id);
        }
      });
      return entityIDs;
    }
    function initFilters(loader){
      $('#slickgrid-toggle-search-panel').click(function(){
        var options = grid.getOptions();
        grid.setHeaderRowVisibility(!options.showHeaderRow);
      });
      updateFilters();
      // Apply filters to the input kep up event
      var keyup_timer = false;
      $(grid.getHeaderRow()).delegate(":input", "change keyup", function(e){
        var new_filter_value = $.trim($(this).val());
        var column_id = $(this).data("columnId");
        if (columnFilters[column_id] == new_filter_value){
          return;
        }
        columnFilters[column_id] = new_filter_value;
        if (keyup_timer){
          clearTimeout(keyup_timer);
        }
        if (e.type == 'change'){
          loader.setFilters(columnFilters);
          ensureViewportData(true);
          displayLoadingIndicator();
        } else {
          keyup_timer = setTimeout(function(){
            loader.setFilters(columnFilters);
            ensureViewportData(true);
            displayLoadingIndicator();
          }, 1000);
        }
      });
      // Register events for the header inputs
      grid.onColumnsReordered.subscribe(function(e, args){
        updateFilters();
      });
      grid.onColumnsResized.subscribe(function(e, args){
        updateFilters();
      });
      // Set initial status
      if (!$.isEmptyObject(columnFilters)){
        // Do this once the height has been calculated
        setTimeout(function(){
          grid.setHeaderRowVisibility(true);
        }, 0);
      }
      grid.setOptions({headerRowHeight:$(grid.getHeaderRow()).parent().height()});
      loader.setFilters(columnFilters);
    }
    function updateFilters(){
      // add the header inputs
      for( var i = 0; i < columns.length; i++) {
        if(columns[i].filter) {
          var header = grid.getHeaderRowColumn(columns[i].id);
          $(header).empty();
          var c = grid.getColumns()[grid.getColumnIndex(columns[i].id)];
          if(typeof c == 'object') {
            if(typeof c.filter == 'string') {
              c.filter = eval('new ' + columns[i].filter + '("' + c.id + '")');
            }
            if(typeof c.filter.input === 'function') {
              var $input = c.filter.input().data("columnId", c.id).val(columnFilters[c.id]);
              Drupal.theme('slickgridFilter', $input, options['columns'][c.id]['filter']).appendTo(header);
            }
          }
        }
      }
    }
    // Generic filter function, passes filtering to the appropriate filter
    // function
    function filter(item){
      for( var columnId in columnFilters) {
        if(columnId !== undefined && columnFilters[columnId] !== "") {
          var c = grid.getColumns()[grid.getColumnIndex(columnId)];
          // Pass the filtering to the doFilter function of whatever filter
          // object is being used
          if(c.filter.doFilter(item, columnFilters[columnId]) === false) {
            return false; // only return false at this point so ALL filters get
            // a chance to run
          }
        }
      }
      return true;
    }
    // Basic comparison function used in sorting columns
    function comparer(a, b){
      var x = a[sortcol].toLowerCase(), y = b[sortcol].toLowerCase();
      return(x == y ? 0 : (x > y ? 1 : -1));
    }
    function initGroups(){
      var groupingFieldLabel = options['columns'][options['grouping_field']]['label'];
      // Set the grouping field
      dataView.groupBy(options['grouping_field'], function(g){
        return groupingFieldLabel + ":  " + g.value + "  <span class='grouping-field-count'>(" + g.count + " items)</span>";
      }, function(a, b){
        return a.value - b.value;
      });
      // Should all groups be collapsed
      if(options['collapse_groups_by_default']) {
        // Refresh the dataView so we have access to the groups
        // Use endUpdate() rather than refresh() to reset suspend
        dataView.endUpdate();
        for( var i = 0; i < dataView.getGroups().length; i++) {
          dataView.collapseGroup(dataView.getGroups()[i].value);
        }
      }
      // Add event to expand / collapse groups
      grid.onClick.subscribe(function(e, args){
        var item = this.getDataItem(args.row);
        if(item && item instanceof Slick.Group && $(e.target).hasClass("slick-group-toggle")) {
          if(item.collapsed) {
            this.getData().expandGroup(item.value);
          } else {
            this.getData().collapseGroup(item.value);
          }
          e.stopImmediatePropagation();
          e.preventDefault();
        }
      });
    }
    function initHiddenColumns(){
      var visibleColumns = [];
      $(columns).each(function(i, e){
        if($.inArray(columns[i]['id'], options['hidden_columns']) === -1) {
          visibleColumns.push(columns[i]);
          columns[i]['hidden'] = false;
        } else {
          columns[i]['hidden'] = true;
        }
      });
      grid.setColumns(visibleColumns);
    }
    function deselectAllRows(){
      var ranges = [];
      var selectionModel = grid.getSelectionModel();
      selectionModel.setSelectedRanges(ranges);
    }
    function updateSettings(setting, value){
      commandHandler.addCommand('update-setting', {
        type: 'POST',
        dataType: 'json',
        url: Drupal.settings.slickgrid.get_form_callback_url + 'slickgrid_settings_form',
        data: {
          'view': viewName,
          'setting': setting,
          'display_id': viewDisplayID,
          'value': value
        },
        success: function(response, status){
          commandHandler.addCommand('update-setting', {
            type: 'POST',
            url: $(response[1]['arguments'][0]).children('form').attr('action'),
            data: $(response[1]['arguments'][0]).children('form').serialize()
          });
        }
      });
    }
    // All callbacks should be routed through this function
    function callback(op, data){
      // Show loading bar is working
      updateStatus({loading: true});
      // Queue the command
      commandHandler.addCommand('update-cell', {
        type: 'POST',
        dataType: 'json',
        url: Drupal.settings.slickgrid.slickgrid_callback_url + op,
        data: data,
        success: $.proxy(callbackSuccess, this),
        error: callbackError
      });
    }
    // Error handling function
    function callbackError(jqXHR, status, errorThrown){
      grid.getEditController().cancelCurrentEdit();
      var errorMessage = [];
      if(jqXHR.status == 0) {
        errorMessage.push({type: 'error', message: 'Could not connect to server. Your website may be unavailable!'});
      } else {
        errorMessage.push({type: 'error', message: 'Sorry there was an error - please reload this page and try again.'});
      }
      updateStatus(true, errorMessage);
    }
    function callbackSuccess(response, status){
      // Ensure there are no invalid cells selected from previous update
      $('div.invalid').removeClass('invalid');
      deselectAllRows();
      var status = {loading: false, success: 0, errors: 0};
      if(response) {
        // Are there any update nodes
        if(response.updated) {
          $.each(response.updated, function(id, entity){
            status.success++;
          });
        }
        // Are we allowing undoing content (there will be a command queue if
        // we are)
        if(options['undo'] && response.op == 'update' && status.success > 0) {
          // Add the update items to the undo command queue
          undoControl.queueCommand(response.updated);
        }
        if(response.deleted) {
          // Loop through each deleted.
          $.each(response.deleted, function(i, id){
            status.success++;
          });
        }
      }
      // Were there any errors?
      if(typeof response.errors !== 'undefined') {
        $.each(response.errors, function(id, err){
          // Get the row denoted by the nid
          row = loader.getRowById(id);
          // Get the data item for the row
          cell = grid.getColumnIndex(response.field_id);
          cellNode = grid.getCellNode(row, cell);
          $(cellNode).addClass('invalid');
          $(cellNode).stop(true, true).effect("highlight", {color: "red"}, 300);
          status.errors++;
        });
      }
      updateStatus(status, response.messages);
      // If the callback has returned a new data array (which will happen on
      // node clone & node add) or new columns reload all rows ; otherwise expect rows will
      // have been invalidated already.
      if(typeof response.data === 'object' || typeof response.columns != "undefined") {
        loader.invalidateRange(0, loader.data.length - 1);
      }
      ensureViewportData();
      // If the callback has returned a column array, update the columns
      if(typeof response.columns === 'string') {
        updateColumns(response.columns);
      }
      $(container).trigger('onSlickgridCallback', {status: status, response: response});
    }
    function updateStatus(status, statusMessages){
      $status.empty();
      if(typeof status.loading !== 'undefined' && status.loading) {
        $status.addClass('slickgrid-loading');
      } else {
        $status.removeClass('slickgrid-loading');
        if(status.errors) {
          $('<span class="slickgrid-status-errors">' + status.errors + '</span>').appendTo($status);
        }
        if(status.success) {
          $('<span class="slickgrid-status-success">' + status.success + '</span>').appendTo($status);
        }
        if(statusMessages) {
          $status.click(function(){
            openDialog($status, Drupal.theme('slickgridMessages', statusMessages));
          })
        }
      }
    }
    function getViewName(){
      return viewName;
    }
    function getViewDisplayID(){
      return viewDisplayID;
    }
    // Lock the grid
    function lock(){
      locked = true;
    }
    // Unlock the grid (prevent editing)
    function unlock(){
      locked = false;
    }
    // Open a dialog (currently via beautytips)
    function openDialog($selector, content, options){
      var options = {positions: 'left', fill: '#fff', strokeWidth: 0, spikeLength: 5, strokeStyle: '#A5A5A5', strokeWidth: 1, width: 200, trigger: 'none', // Already
      // clicked so manually activate
      cornerRadius: 0, overlap: 3};
      if(typeof content == 'object') {
        content.addClass('slickgrid-dialog');
        options.contentSelector = content;
        $selector.bt(options);
      } else {
        $selector.bt(content, options);
      }
      $selector.btOn();
    }
    // Close a dialog
    function closeDialog(){
      $('.bt-active').btOff();
    }
    // Reload the whole grid
    function reload(data){
      grid.setSelectedRows([]);
      loader.reloadData(0, loader.data.length - 1);
    }
    function updateColumns(updatedColumns){
      columns = eval('(' + updatedColumns + ')');
      grid.setColumns(columns);
      if(tabs) {
        tabs.rebuild();
      }
    }
    // Return the current active columns, or the entire list if 'all' is defined and true.
    function getColumns(all){
      if (typeof all !== 'undefined' && all){
        // Make sure properties are up to date
        var grid_cols = grid.getColumns();
        for (var i = 0; i < columns.length; i++){
          var existing_col = grid.getColumnIndex(columns[i].id);
          if (typeof existing_col !== 'undefined'){
            $.extend(columns[i], grid_cols[existing_col]);
          }
        }
        return columns;
      } else {
        return grid.getColumns();
      }
    }
    // Batch change the columns.
    function setColumns(cols){
      columns = cols;
      var new_grid_cols = [];
      for (var i = 0; i < columns.length; i++){
        if (!columns[i].hidden){
          new_grid_cols.push(columns[i]);
        }
      }
      grid.setColumns(new_grid_cols);
    }
    function setColumnFilter(field, value, refresh){
      columnFilters[field] = value;
      if (typeof refresh !== 'undefined' && refresh){
        loader.setFilters(columnFilters);
        ensureViewportData(true);
        displayLoadingIndicator();
      }
    }
    function getContainer(){
      return container;
    }
    // Save the state (viewport height, selected tab, etc) of the grid
    function getGridState(){
      var state = {
        height: $(container).height(),
        row: grid.getViewport().top,
      };
      if (typeof tabs !== 'undefined' && tabs){
        state.activeTab = $('#slickgrid-tabs span.active-tab').attr('id');
      }
      return state;
    }
    // Restore the state generated by getGridState
    function setGridState(state){
      if (typeof state.height !== 'undefined') {
        $(container).height(state.height);
      }
      if (typeof tabs !== 'undefined' && tabs && typeof state.activeTab !== 'undefined') {
        $('#' + state.activeTab).click();
      }
      if (typeof state.row !== 'undefined') {
        grid.scrollRowToTop(state.row);
      }
    }
    $.extend(this, {"callback": callback, "getViewName": getViewName, "getViewDisplayID": getViewDisplayID, "getEntityIDs": getEntityIDs, "getContainer": getContainer, "getColumns": getColumns, "setColumns": setColumns, "openDialog": openDialog, "closeDialog": closeDialog, "reload": reload, 'setColumnFilter': setColumnFilter, 'updateFilters': updateFilters, 'updateSettings': updateSettings, 'updateStatus': updateStatus, 'getGridState': getGridState, 'setGridState': setGridState, 'invalidateRow' : invalidateRow, 'invalidateSelectedRows': invalidateSelectedRows, 'deselectAllRows': deselectAllRows});
    init(this);
    $(container).trigger('onSlickgridInit', this);
  }
})(jQuery);
