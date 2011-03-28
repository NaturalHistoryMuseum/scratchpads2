// Grid & dataView need to be globals so they can be accessed from formatters etc.,
var grid;
var dataView;

(function($) {

    // register namespace
    $.extend(true, window, {
        Slickgrid: Slickgrid
    });

    // Slickgrid class implementation
    function Slickgrid(container, viewName, viewDisplayID, callbackPath) {


        var columnFilters = {};
        var objHttpDataRequest;
        var checkboxSelector;

        var $dialog; // $dialog (at the moment a beautytips instance)
        var activeRow; // The row currently being edited
        var commandQueue = [];
        var locked; // Is this grid locked? We use own locking mechanism, so it prevents editing across the whole grid
        
        // Remove
        var $status; // $status container for result icons & messages
        var $loadingBar; // $loadingBar container for loading bar
        
        // Controls
        var undoControl;

        function init() {

            $status = $('#slickgrid-status');
            $loadingBar = $('#slickgrid-loading-bar');
            
            // Add row checkboxes if multi edit, delete or clone is enabled
            if (options['multi_edit'] || options['delete']) {

                // Init row checkboxes - needs to be done before the grid is initiated as a column needs to be added
                checkboxSelector = new Slick.CheckboxSelectColumn({
                    cssClass: "slick-cell-checkboxsel"
                });

                // Add the selector column
                columns.unshift(checkboxSelector.getColumnDefinition());

            }
            

            // Is undo enabled? If it is, add an editCommandHandler
            if (options['undo']) {
              
              // Undo needs to be initiated prior to building the grid & dataview - it adds an editCommandHandler option (see init())
              undoControl = new Slick.Controls.Undo($("#slickgrid-undo"));
                
            }
            
            
            // Initialise the dataview & slickgrid
            dataView = new Slick.Data.DataView();
            grid = new Slick.Grid(container, dataView, columns, options);

            // Add all the controls
            
            // Add controls defined by the slickgrid library  
            // Page control
            if (options['pager']) {
                var pagerControl = new Slick.Controls.Pager(dataView, grid, $("#slickgrid-pager"));
            }
            // Add controls defined by the slickgrid module  
            // delete control            
            if (options['delete']) {
                var deleteControl = new Slick.Controls.Delete(dataView, grid, $("#slickgrid-delete"));
            }
            


            
            // Are sortable columns enabled?
            // Sortable columns won't work with collapsible taxonomy fields
            if (options['sortable_columns']) {
            
                initSortableColumns();

            }

            // Users can show / hide columns
            if (options['select_columns']) {
      
              var columnpicker = new Slick.Controls.ColumnPicker(columns, grid, options);
              
            }

            // Are there hidden columns?
            if(options['hidden_columns']){
              
              initHiddenColumns();   
              
            }
            
            // Does the grid have filters that need adding?
            if (options['has_filters']) {
                initFilters();
            }

            grid.setSelectionModel(new Slick.RowSelectionModel({
                selectActiveRow: false  // Do not select active row 
                                        // Going to handle this ourselves (making it selected) so user can select & edit multiple items
                                        // Otherwise all rows will be deselected on edit
            }));

            // If row checkboxes are enabled, add row selection to the grid & register the plugin
            if (checkboxSelector) {
                grid.registerPlugin(checkboxSelector);

            }
            
            // Register events for my handling of active rows
            grid.onBeforeEditCell.subscribe(onBeforeEditCell);
            grid.onBeforeCellEditorDestroy.subscribe(onBeforeCellEditorDestroy);

            dataView.onRowCountChanged.subscribe(function(e, args) {
                grid.updateRowCount();
                grid.render();
            });
            
            dataView.onRowsChanged.subscribe(function(e, args) {
                grid.invalidateRows(args.rows);
                grid.render();
            });

            dataView.beginUpdate();

            // Add the data to the dataView
            dataView.setItems(data);

            // If a grouping field has been chosen, group the data
            // NB: needs to come after the data has been added to the dataView
            if (options['grouping_field']) {

                initGroups();   

            }
          
            // Add the collapsible taxonomy field
            if (options['collapsible_taxonomy_field']) {
            
              initCollapsibleTaxonomyField(options['collapsible_taxonomy_field']);
            
            }

            // If has_filter is true, there are header filters being used
            // Apply the filter to the dataView
            if (options['has_filters']) {
                dataView.setFilter(filter);
            }
            
            
            dataView.endUpdate();
            
            initCallbackEvents();

        }
        
        // Add callbacks to grid events so the info can be saved
        function initCallbackEvents(){
          
          grid.onColumnsReordered.subscribe(onColumnsReordered);
          
          // There isn't a grid event when a column is shown / hidden - tag one onto onHeaderContextMenu()
          grid.onHeaderContextMenu.subscribe(onHeaderContextMenu);
          
          grid.onColumnsResized.subscribe(onColumnsResized);
          
          // Add resizable callback event
          $('#slickgrid').resizable({
            handles: 's',
        		stop : function(e, ui) {

        			if (ui.originalSize.height != ui.size.height) {
        				onViewportResized(ui.size.height);
        			}
        			
        		}	
          });
          
        }
        
        // User has reodered the columns - save it to the backend
        function onColumnsReordered(e, ui){

          var orderedColumns = [];
          // This event is firing when columns have been dragged slightly but returned to same position
          // grid.getColumns() has the new order, while the global columns retains the old order
          // set orderActuallyChanged to true when looping through if the order really has changed
          var orderActuallyChanged = false;
          
          $(grid.getColumns()).each(function(i, col) {
            
            orderedColumns.push(col['id']);
            
            if(!orderActuallyChanged && col['id'] != columns[i]['id']){
              orderActuallyChanged = true;
            }
            
          });
          
          if(orderActuallyChanged){
            updateSettings('ordered_columns', orderedColumns);
          }   
          
        }
        
        // User has shown / hidden a column - save it to the backend
        function onColumnsChanged(){
          
          var hiddenColumns = [];
          
          $('input[id^=columnpicker]', '.slick-columnpicker').each(function(i,e) {
              if (!$(this).is(":checked")) {
                  hiddenColumns.push(columns[i]['id']);
              }
          });

          updateSettings('hidden_columns', hiddenColumns);
          
          // Add column filters back into the grid
          if (options['has_filters']) {
              initFilters();
          }          
          
        }
        
        // The context menu (choosing which columns to display) has been opened
        function onHeaderContextMenu(e, ui){
          
          // If multi edit is anebled we want to hide the first column from the user as its the checkbox select one
          if (options['multi_edit']) {
            $('.slick-columnpicker li').eq(0).hide()
          }  
          
          // User has changed the columns
          $('input[id^=columnpicker]', '.slick-columnpicker').change(onColumnsChanged);
          // Auto resize does not fire column resize - so need to call it manually
          $('#autoresize').change(function(){
            onAutoResize($(this).is(':checked') ? 1 : 0);
          });
        }
        
        // Columns have been resized
        function onColumnsResized(e, ui){
          
          // Need to save width of ALL changed columns - if auto resize is on it won't just be the resized column that has changed
          var resizedColumns = {};
          
          $(grid.getColumns()).each(function(i, col) {

              resizedColumns[col['id']] = col.width;
              
          });
          
          updateSettings('column_width', resizedColumns);
  
        }
        
        // Viewport has been resized
        function onViewportResized(height){
          
          updateSettings('viewport_height', height);
          
        }
        
        function onAutoResize(value){
          
          updateSettings('forceFitColumns', value);
          
        }
        
        // User has started editing a cell
        // Need to add row to the selected rows
        function onBeforeEditCell(e, ui){

          // If this grid is locked, prevent editing
          if(locked){
            return false;
          }
          
          setActiveRow(ui.row); 

          // BUG fix: need to turn off & remove tooltips prior to re-editing
          cellNode = grid.getCellNode(ui.row, ui.cell);  

          if(typeof $(cellNode).attr('bt-xTitle') != 'undefined'){
            $(cellNode).btOff();  
            $(cellNode).bt({removeTip: 1});            
          }          

          $(cellNode).removeClass('invalid');
          
        }
        
        // User has stopped editing a cell
        // Deselect the row being actively edited
        function onBeforeCellEditorDestroy(editor){

          unsetActiveRow();     
          
        }
        
        function setActiveRow(row){
          
          $('div[row="'+row+'"]').addClass('active-row');
          
        }
        
        function unsetActiveRow(){
          
          var cell = grid.getActiveCell();
          $('div[row="'+cell.row+'"]').removeClass('active-row');
          
        }
        
        // Get an array of all entity IDs (eg NIDs) to update
        // Pass in the active item, so this won't get added twice but will definitely get added if not selected
        function getEntityIDs(item){
          
          var entityIDs  = [];
          // Item isn't required - delete for example doesn't have any active item
          if(typeof item != 'undefined'){
            entityIDs.push(item.id);
          }
          
          $.each(grid.getSelectedRows(), function(i, row) { 

            // Retrieve the data item of the selected row
            var selected_item = dataView.getItem(row);

            if(entityIDs.indexOf(selected_item.id) === -1){
              // Add the NID to the edited nids object
              entityIDs.push(selected_item.id);
            }
          
          });
          
          return entityIDs;
          
        }
      

        function initFilters() {

            updateFilters();

            // Apply filters to the input kep up event
            $(grid.getHeaderRow()).delegate(":input", "change keyup",
            function(e) {
                columnFilters[$(this).data("columnId")] = $.trim($(this).val());             
                dataView.refresh();
            });
            
            // Register events for the header inputs
            grid.onColumnsReordered.subscribe(function(e, args) {
                updateFilters();
            });

            grid.onColumnsResized.subscribe(function(e, args) {
                updateFilters();
            });
            

        }

        function updateFilters() {

            // add the header inputs
            for (var i = 0; i < columns.length; i++) {

                if (columns[i].id !== "_checkbox_selector") {
                  
                    var header = grid.getHeaderRowColumn(columns[i].id);
                    $(header).empty();
                    
                    if (options['columns'][columns[i].id] && options['columns'][columns[i].id]['filter']) {
                      
                      
                      var c = grid.getColumns()[grid.getColumnIndex(columns[i].id)];
                      
                      if(typeof c != 'undefined'){
                        c.filter = eval('new Slick.Filters.' + options['columns'][columns[i].id]['filter'] + '()');
                          // Does this filter have an input function? 
                         // If it does, add the input html
                         if(typeof c.filter.input === 'function'){
                           var $input = c.filter.input()
                            .data("columnId", c.id)
                            .val(columnFilters[c.id]);
                            Drupal.theme('slickgridFilter', $input, options['columns'][c.id]['filter']).appendTo(header);
                         }                        
                      }
                      

                   
 
                    }

                }
            }

        }

        // Generic filter function, passes filtering to the appropriate filter function
        function filter(item) {          
            for (var columnId in columnFilters) {
                if (columnId !== undefined && columnFilters[columnId] !== "") {
                  
                    var c = grid.getColumns()[grid.getColumnIndex(columnId)];
                      // Pass the filtering to the doFilter function of whatever filter object is being used  
                      if(c.filter.doFilter(item, c.field, columnFilters[columnId]) === false){
                        return false; // only return false at this point so ALL filters get a chance to run
                      } 
                }
            }
            return true;
        }

        // Basic comparison function used in sorting columns
        function comparer(a, b) {
            var x = a[sortcol],
            y = b[sortcol];
            return (x == y ? 0: (x > y ? 1: -1));
        }
        
        function initGroups(){
          
                        // var groupsUI = new Slick.Controls.GroupsUI(dataView, grid, $("#controls"));

                        var groupingFieldLabel = options['columns'][options['grouping_field']]['label'];

                        // Set the grouping field
                        dataView.groupBy(
                        options['grouping_field'],
                        function(g) {
                            return groupingFieldLabel + ":  " + g.value + "  <span class='grouping-field-count'>(" + g.count + " items)</span>";
                        },
                        function(a, b) {
                            return a.value - b.value;
                        }
                        );

                        // Should all groups be collapsed
                        if (options['collapse_groups_by_default']) {
                            // Refresh the dataView so we have access to the groups
                            // Use endUpdate() rather than refresh() to reset suspend
                            dataView.endUpdate();
                            for (var i = 0; i < dataView.getGroups().length; i++) {
                                dataView.collapseGroup(dataView.getGroups()[i].value);
                            }
                        } 

                        // Add event to expand / collapse groups
                        grid.onClick.subscribe(function(e, args) {
                            var item = this.getDataItem(args.row);
                            if (item && item instanceof Slick.Group && $(e.target).hasClass("slick-group-toggle")) {
                                if (item.collapsed) {
                                    this.getData().expandGroup(item.value);
                                }
                                else {
                                    this.getData().collapseGroup(item.value);
                                }

                                e.stopImmediatePropagation();
                                e.preventDefault();
                            }
                        });
          
        }
        
        
        // Add events to manage the collapsible toggle
        function initCollapsibleTaxonomyField(field){
          
          grid.onClick.subscribe(function(e,args) {
            
    			                if ($(e.target).hasClass("toggle")) {
    			                  
    			                    // ensure the filters know this column is selected
    			                    columnFilters[field] = true;   
    			                  
    			                    var item = dataView.getItem(args.row);
    			                    
    			                    if (item) {
    			                        if (!item._collapsed)
    			                            item._collapsed = true;
    			                        else
    			                            item._collapsed = false;

    			                        dataView.updateItem(item.id, item);
    			                    }
    			                    
    			                    // Ensure this doesn't screw with the header filters
    			                    delete columnFilters[field];
    			                    
    			                    e.stopImmediatePropagation();
    			                    
    			                }
    			            });
          
        }
        
        function initSortableColumns(){
          
          grid.onSort.subscribe(function(e, data) {
      
              var sortCol = data.sortCol;
              var sortAsc = data.sortAsc;
              sortdir = sortAsc ? 1: -1;
              sortcol = sortCol.id;
      
              // Set which function to use to sort the column - presently just uses a basic comparer
              dataView.sort(comparer, sortAsc);
      
          });
          
        }
        
        function initHiddenColumns(){
            
             var visibleColumns = [];
             
             $(columns).each(function(i,e) {
           
               if($.inArray(columns[i]['id'], options['settings']['hidden_columns']) === -1){
                 visibleColumns.push(columns[i]);
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
          
          data = {
            'view': viewName,
            'setting': setting,
            'value': value
          }
          
          callback('settings', data);
          
        }
        
        // All callbacks should be routed through this function
        function callback(op, data){

          // Prevent further edits while this callback is running
          lock();

          // Show loading bar is working
          updateLoadingBar(true);
          
          // Check to see if there is an AJAX request already in
          // progress that needs to be stopped.
          if (objHttpDataRequest){

          // Abort the AJAX request.
          objHttpDataRequest.abort();

          }
          
          ajaxOptions = {
            type: 'POST',
            dataType: "json",
            success: callbackSuccess,
            error: callbackError,
            complete: callbackComplete,
            url: callbackPath + '/' + op,
            data: data
          };

          objHttpDataRequest = $.ajax(ajaxOptions);

        }
        
        // Error handling function
        function callbackError(args){
                    
          grid.getEditController().cancelCurrentEdit();
          
          x = args[0];
          e = args[1];

          var errorMessage = []; // Error message for user
          var errorLog; // Error message for log
          
          if(x.status==0){
            
            errorMessage.push({type : 'error', message: 'Could not connect to server. Your website may be unavailable!'});
            
          }else{
          
            errorMessage.push({type : 'error', message: 'Sorry there was an error - please reload this page and try again.'});
          
            if(x.status==404){
            errorLog =  '404: Requested URL not found.';
            }else if(x.status==500){
            errorLog =  '500: Internel Server Error.';
            }else if(e=='parsererror'){
            errorLog =  'Error parsing JSON Request.';
            }else if(e=='timeout'){
            errorLog =  'Request Time out.';
            }else {
            errorLog =  'Unknown Error';
            }

            // TODO - everlasting loop? Check what callback it is.
            // Pass the error to the callback function so we can try and fix any errors.
            callback('log', {error : errorLog});
          
          }
          
          updateLoadingBar(false);
          updateStatus(true, errorMessage);
          
        }
        
        function callbackSuccess(response, status){
          
          deselectAllRows();
          
          updateLoadingBar(false);

          if(response){
            
            // Are there any update nodes
            if(response.updated){
                           
                $.each(response.updated, function(id, node) { 
                
                  // Get the row denoted by the nid
                  row = dataView.getRowById(id);
                
                  // Get the data item for the row
                  var item = dataView.getItem(row);
                
                  // Update the item with the new value (if necessary)
                  if(item[response.field_name] != node.value){

                      // Change the value
                      item[response.field_name] = node.value;

                      // Update the dataView
                      dataView.updateItem(item.id, item);

                  }           
                
                
                });

              // Are we allowing undoing content (there will be a command queue if we are)
              if(options['undo'] && response.op == 'update'){              
                // Add the update items to the undo command queue
                undoControl.queueCommand(response.updated);
                
              }

            }
            
            // Were there any errors?
            if(typeof response.errors != 'undefined' && response.errors.length){  

              response.error = true;

              $.each(response.errors, function(nid, errorMessage) { 
                
                if(errorMessage[response.field_name]){
                  
                  row = dataView.getRowById(nid);

                  cell = grid.getColumnIndex(response.field_id);
                
                  cellNode = grid.getCellNode(row, cell);
                
                  $(cellNode).addClass('invalid');
                  
                   $(cellNode).bt(errorMessage[response.field_name], {
                    positions : 'right',
                    fill : 'rgba(0, 0, 0, .7)',
                    strokeWidth : 0,
                    spikeLength : 10,
                    cssStyles : {
                      color : 'white',
                      'font-size' : '10px'
                    },
                    width: 150,
                    closeWhenOthersOpen : true
                  });
                
                }
                
                                         
              });
              
            }
            
            // Are there items to be deleted?
            if(response.deleted){  
              
              $.each(response.deleted, function(i, id) {
                 dataView.deleteItem(id);
               });
              
            }

            if(response.messages){
            updateStatus(response.error, response.messages);
            }
            
          }

        }
        
        // Callback has completed - unlock the grid for further edits
        function callbackComplete(args){

          unlock();
          
        }
        
        function updateStatus(error, statusMessages){
          
           $status.attr('class', '');
           
           if(error){
             $status.addClass('slickgrid-error').html('Warning');
           }else{
             $status.addClass('slickgrid-no-error').empty();
           }

           $status.bt({
            contentSelector: Drupal.theme('slickgridMessages', statusMessages), 
            positions : 'left',
            fill : 'rgba(0, 0, 0, .7)',
            strokeWidth : 0,
            spikeLength : 10,
            cssStyles : {
              color : 'white',
              'font-size' : '10px'
            },
            width: 250,
            closeWhenOthersOpen : true,
            trigger : 'hover'
          });        
          
        }
        
        function updateLoadingBar(loading){
                    
          if(loading){
            $loadingBar.addClass('loading');
          }else{
            $loadingBar.removeClass('loading');
          }
          
        }
        
        function getViewName(){
          return viewName;
        }
        
        function getViewDisplayID(){
          return viewDisplayID;
        }
        
        function getCallbackPath(){
          return callbackPath;
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

          var options = {
            positions : 'top',
            fill : '#fff',
            strokeWidth : 0,
            spikeLength : 7,
            strokeStyle: '#A5A5A5',
            strokeWidth: 1,
            width: 200,
            trigger : 'none',  // Already clicked so manually activate
            cornerRadius: 0,
            overlap: -1
          };

          if(typeof content == 'object'){
            content.addClass('slickgrid-dialog');
            options.contentSelector = content;
            $selector.bt(options);
          }else{
            $selector.bt(content, options);
          }
          
          $selector.btOn();
          
        }
        
        // Close a dialog
        function closeDialog($selector){
          $selector.btOff();
        }
        
        
        ///////////////////////////////////////////// Public API /////////////////////////////////////////////
        $.extend(this, {
           // Methods
           "callback":               callback,
           "getViewName":            getViewName,
           "getViewDisplayID":       getViewDisplayID,
           "getEntityIDs":           getEntityIDs,
           "openDialog":             openDialog,
           "closeDialog":            closeDialog,
           "getCallbackPath":        getCallbackPath,
           "callbackSuccess":        callbackSuccess 
        });

        init();

    }

})(jQuery);