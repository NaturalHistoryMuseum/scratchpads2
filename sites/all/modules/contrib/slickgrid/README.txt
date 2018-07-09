  ------------------------------------------------------------------------------------
                         ABOUT SLICKGRID
  ------------------------------------------------------------------------------------

This modules is an implementation of Michael Leibman's jquery slickgrid plugin (https://github.com/mleibman/SlickGrid) for Drupal. It defines a slickgrid style for 
views, outputting it as an editable grid.

For every field defined in the view, you can set its column width, whether it's sortable, and formatters (how the cell data is displayed), an editor, a validator and filters (for filtering the column data).

The sortable, formatter, editor & validator options are provided via ctools plugins. See the plugins directory for examples.

By default, the module comes with the following plugins ready to use:

Formatters:

  * Collaspible taxonomy fields (tree structures)

Editors:

  * Inline cell editor (for editing inline in the grid, used for text_textfield, text_textarea, node_title & date_text fields)
  * Popup node form (pops up the portion of the node form for the field, can be used for all fields)
 
Validators:

  * Required field
  
Filter

  * Text filter (case-insensitive text filter - if the column cell contains the text it will be displayed)
  * Collaspible taxonomy filter (to be used with the collaspible taxonomy formatter)

Not all plugins are available for all fields - for example, the inline cell editor can only be used with text / numeric fields. 

The popup node form will work with all fields.  


 The grid can be customised by setting the view style settings. The following options are available:

  * Grouping field - group fields with a common value together
  * Tabs - organise columns into tabs
  * Enable resizing columns
  * Enable reordering columns
  * Column visibility - allow users to show & hide columns
  * Asynchronous editor loading - load the grid on scroll
  * Force fit columns - force column widths to fit the grid.
  * Override paging - use slickgrid's in-built pagination instead of views'.
  * Set grid, header & row height.
  
At the front end, users can show/hide, resize, reorder columns & resize the height of the grid. These settings will be stored per user and used next time the grid is shown. Saving the view (via the usual views UI) will reset these settings.

Editing Options:

  * Check row selection checkbox to allow for 'multi edit', 'delete items' & 'clone items'. If checked, these options will be presented to the user as buttons in the lower right hand side of the screen. 
  * Auto edit - edit cell immediately on entry, otherwise a double clieck will be required.
  
Export options:

  * If the view data export module is installed & the slickgrid view has an attached export view, an export button will appear. 
  * If "Export selected rows" is checked, the user will be given the option to only export selected rows.
  
Add options:

  * Select a node type to add directly from the grid.     

Undo options:

  * Select undo to allow users to roll back changes. Currently this only works with nodes (as they have a vid)
  
Pagination

  * The slickgrid provides it's own pagination which is very fast - but it does require loading the whole view in one go. 
    This does cause memory issues on large datasets - in the future you'll be able to use http://drupal.org/project/views_batch_page to fix this (but I'm still working on the D7 version) 

  ------------------------------------------------------------------------------------
                         INSTALLATION
  ------------------------------------------------------------------------------------
  
  1. Download the slickgrid plugin from https://github.com/mleibman/SlickGrid
     This module is tested with the master branch (2011-03-23)

  2. Unzip & copy the directory into your sites/all/libraries directory
  
  3. Rename it to "slickgrid".
  
  4. Enable the module & you should have a new view style "slickgrid".
  
  
  ------------------------------------------------------------------------------------
                         REQUIREMENTS
  ------------------------------------------------------------------------------------
  
The following modules are required:

  * views
  * ctools
  * libraries
  * beautytips
  * jquery_update (tested with jquery 1.5.2)
  

  ------------------------------------------------------------------------------------
                         RECOMMENDED MODULES
  ------------------------------------------------------------------------------------  

  * Views data export module (http://drupal.org/project/views_data_export)
    Install this module if you want to export data from the grid. Attach an export view & the export button will appear.
    
  * Editor views (http://drupal.org/project/editor_views) 
    If using the grid as an editor, it's best if used in conjunction with editor views 
    Which allows you to only display nodes the user has permissions to edit / delete. I'm working on D7 port now. 
  