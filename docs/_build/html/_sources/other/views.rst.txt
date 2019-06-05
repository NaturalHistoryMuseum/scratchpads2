Views
=====

.. figure:: /_static/TableView3.jpg


**Aim**: Create a table view for the literature mining content type and
add filters to search for specific data.

**You need**:

-  The Literature mining content type (see `Add a new content type`_)
-  Several Literature mining nodes
-  The Views module enabled. To enable views go to Structure in the
   :term:`Admin menu`, click on Tools, enable ‘Views’ and save.

**Time**: 30 min

Instructions
============

.. figure:: /_static/LiteratureMining4.jpg


|Literature mining page with default HTML list display| With views you
can modify how lists of content are displayed on your site. The default
display format for a new content type is an HTML list. You can change
the format to a table, grid or for example a map to show your data in an
optimal way. For the Literature mining content type a table would be
best, so that is what we are going to do:

-  Click on the LITERATURE MININGS tab in the `Menu menu`_ and then on
   the cog icon to the right of the “Literature minings” title. Click on
   Edit view.

    .. figure:: /_static/TableView1.jpg

    Edit view administration page

    

-  We want to change the current page display, so we are not going to
   add a new display

    .. figure:: /_static/TableViewTitle.jpg

    Edit view: Change title
   

Title
^^^^^

-  The system automatically added an “s” to the view title as well as
   the menu item. Remove the “s” from the title by clicking on the
   Literature minings link in the ‘Title’ section.
-  Delete the “s” from the title.
-  Usually, when you edit any part of the view, you can decide whether
   you want to edit all displays and thereby change the default, or just
   the current page and thereby override the default settings only for
   this case. In our case we want to change the title for all displays,
   so click on the Apply (all displays) button.
-  If you added a new display for the view, then you usually need to
   take care to change the setting just for the current page. A yellow
   message (“All changes are stored temporarily…”) appears at the top of
   the page to make you aware that your changes are not permanent yet.
-  Click on the Save button in the upper right corner to make your
   changes permanent. This will automatically bring you back to the page
   view where you can check the changes. You will notice that the menu
   item in the Main menu is still with an extra “s”, so we need to
   change this, too.
-  Go back to editing the view.

Page settings: Menu
^^^^^^^^^^^^^^^^^^^

-  In the ‘Page settings’ section click on the link next to “Menu”.
-  Remove the “s” from the TITLE. If needed you can add a description or
   change the parent menu item. Apply.

Format
^^^^^^

.. figure:: /_static/ViewFormat.jpg
   :alt: Edit view: Change format

   Edit view: Change format

-  To change the display format, click on “HTML list” in the ‘Format’
   section. Select “Table”. Apply.
-  Next, you can edit the style options. We will come back to this
   later, so cancel for now.

At the bottom of the ‘Edit view’ page you can now see an example of what
our new view is going to look like. As you can see, only the title field
is showing, so we need to add more fields. Before we do this however,
save the view and edit again.

Fields
^^^^^^

-  A new section called ‘Fields’ has appeared below the ‘Format’
   section. Click on the Add link to add more fields.

-  Select “Content: Literature mining category”, “Content: Page”,
   “Content: Text”, “Field: Reference” and “Field: Taxonomic name”
   fields. Note that below each field name is a list of the content
   types in which it appears, so you can find fields by searching for
   the literature mining content type. Apply.

-  Now you can configure the fields you selected one by one. The default
   is usually fine, so you don’t need to change anything. However, you
   could for example make the column header (label) bold by clicking on
   Style settings , selecting ‘Customize label HTML’ and choosing
   “STRONG” from the ‘Label HTML element’ dropdown menu.

.. figure:: /_static/TableView2.jpg

Table view


-  Change the field order by clicking on the arrow down icon next to the
   add link in the ‘Fields’ section. Click on the rearrange link. Drag
   and drop the fields into a better order. Apply.

-  Save the view.

Filter criteria
^^^^^^^^^^^^^^^

Filters restrict a view to a certain set of data. Filters can be hidden
or exposed. Exposed filters can be used by the users to search for
certain data. Two filters are already present for this view. The
“Published (Yes)” filter that ensures that only published content will
be shown in this view and the “Type (= Literature mining)” filter that
ensures that only literature mining content is shown. To help users in
finding certain literature mining content we are now going to add a few
filters that are exposed to the users.

-  Click on the add link in the ‘Filter criteria’ section. Select the
   “Content: Literature mining category
   (field_literature_mining_category)”, “Field: Taxonomic name
   (field_taxonomic_name)” and “Field: Reference (field_reference)”
   fields. Apply.
-  Next you can configure each of the fields in turn. Adapt the labels
   and select “Expose this filter to visitors…” for all of them. For the
   taxonomic name filter also select “Expose operator” and “Allow
   multiple selections”.
-  Save the view and check out the different filters.

Header
^^^^^^

It would be nice to add some introductory text above the table. The area
above the table is called “Header” and the one below is the “Footer”. So
we need to add a header:

-  Edit the view again and click on add for the ‘Header’.
-  Select “Global: Result summary” to get a summary of the number of
   items on the page and select “Global: Text area” for our introductory
   text. Next you can configure both fields. Add some introductory field
   in the text area field.

Table settings
^^^^^^^^^^^^^^

With the right settings, the table rows (items) can be sorted by
clicking on the header of the respective column. 

.. figure:: /_static/ViewStyleOptions.jpg

Edit table view: Style options

-  To make certain fields sortable click on the Settings link next to
   ‘Table’ in the ‘Format’ section.
-  Check “Sortable” for the title, reference and category fields. Apply.

Pager
^^^^^

The default for tables is to use a pager that shows 10 items per page.
To view more items the user has to click to the next page(s). Views
using pagers are quicker to load because only the first page needs to be
uploaded. However, one problem with using pagers is that the sorting
only works within a page not across all items. If we want to be able
sort all items, we need to disable the pager.

-  Click on the Full link next to ‘Use pager’ in the ‘Pager’ section.
-  Select “Display all terms”. Apply. Don’t change anything in the next
   view and apply again.

Sort criteria
^^^^^^^^^^^^^

The default sort criterium is the post date of the items, which the most
recent nodes at the top. You can change the sort criteria or add
criteria in the ‘Sort criteria’ section.

Additional options
~~~~~~~~~~~~~~~~~~

Displays
^^^^^^^^

In addition to changing the existing display, you can add new displays.

.. figure:: /_static/ViewAddDisplay.jpg

Edit view: Add Display


-  Click on the Add button in the ‘Displays’ section near the top of the
   views administration page. Select one of the display options:

   -  Attachment: With this display, you can attach one display of a
      view to another display. This allows you to group different views
      together.
   -  Block: Creates a block display.
   -  Data export: Creates a file.
   -  Feed: Creates a feed.
   -  Page: Creates a page display.
   -  References: Creates a references view that can be added for
      example as a field (e.g. node references view) to custom content
      types.

Take care when changing the settings of your new display that you only
change the current display (override) and not all displays.