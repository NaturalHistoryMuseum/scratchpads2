  ------------------------------------------------------------------------------------
                         ABOUT SLICKGRID
  ------------------------------------------------------------------------------------

This modules is an implementation of Michael Leibman's jquery slickgrid plugin (https://github.com/mleibman/SlickGrid) for Drupal. It defines a slickgrid style for 
views, outputting it as a grid.

It is extremely customisable - slickgrid styles can be customised to allow:

  * Field grouping
  * Collaspible taxonomy fields (tree structures)
  * Resize, reorder & hide columns
  * Delete nodes via the grid
  * Async editor loading for large datasets.
  * Auto edit (if an cell editor is defined, activate on entry to the cell; otherwise double click to edit) 
  * Force fit columns (to fill the available column space)
  * Multi edit (checkboxes will be displayed in the rows. When you edit a row, all selected rows will be updated with the same value)
  * Undo (undo previous changes - uses node revisions)
  * Viewport height
  * Row height
 
At the front end, users can show/hide, resize, reorder columns & resize the height of the grid. These settings will be stored per user and used next time the grid is shown. Saving the view (via the usual views UI) will reset these settings.
    
For every field defined in the view, you can set its column width, whether it's sortable, and formatters (how the cell data is displayed), an editor, a validator and filters (for filtering the column data).

This module implements a hook_slickgrid_plugin so other modules can define additional formatters, editors etc., See slickgrid_slickgrid_plugins() for examples.

This modules comes with some plugins already defined & ready to use.

Formatters - TODO

Editors:

  * Text cell editor (for editing inline in the grid, used for text_textfield, text_textarea, node_title & date_text fields)
  * Simple textarea (a popup textarea, used for text_textarea and node body fields)
  * Popup node form (pops up the portion of the node form for the field, can be used for all fields)
 
Validators:

  * Required field
  
Filter

  * Text filter (case-insensitive text filter - if the column cell contains the text it will be displayed)

  
  ------------------------------------------------------------------------------------
                         WARNINGS & ISSUES
  ------------------------------------------------------------------------------------  

The slickgrid plugin requires jquery 1.4.3 & jquery UI 1.8.5. These libraries are included as part of Slickgrid, and added to all slickgrid views pages. This will be fixed in the Drupal 7 version of this module. 
 
Lightbox 2 has a problem with jquery 1.4.3 - but it can be worked around by using lightbox2_lite. I'll post an issue in the queue detailing how to send this variable on just slickgrid views pages.


  ------------------------------------------------------------------------------------
                         REQUIREMENTS
  ------------------------------------------------------------------------------------
  
The following modules are required:

  * views 2
  * beautytips
  * ajax_load
  
  
  ------------------------------------------------------------------------------------
                         RECOMMENDED MODULES
  ------------------------------------------------------------------------------------  
  
If using the grid as an editor, it's best if used in conjunction with editor views (http://drupal.org/project/editor_views)

Which allows you to only display nodes the user has permissions to edit / delete.  
  