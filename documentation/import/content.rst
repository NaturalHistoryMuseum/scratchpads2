Import content
==============

*To import data into your Scratchpad they need to be in the correct
format matching the fields, and in some cases the values within these
fields.*

*To help prepare your data in the correct format you can export a
template Excel file that includes all the fields and any pre-defined
field values for the respective content type. Populate this file with
your data and import into the Scratchpad.*

.. figure:: /_static/ImportTaxDescr.jpg

Creating the template file
~~~~~~~~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Content*

2. Click on *Import* next to the content type you want to create the
   template file for, e.g. Taxon description

3. Click on the *Download* link and open the file in Excel. You will see
   the different Scratchpad fields as column headers. Required fields
   are shown in Red

.. figure:: /_static/ImportTaxDescrTemplate.jpg

4. Fill the template file with your data and save

Create the pre-populated template file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Content*

2. Click *Import* next to the content type into which you want to import
   data, e.g. Taxon description.

.. figure:: /_static/AdminTaxonDesc.jpg   

3. From the *Maximum number of rows* drop down menu select the option
   that best matches your file. (e.g. for a excel file with 3500 rows,
   choose 5000)

4. *Browse* for the file and click on the *Import* button

5. View your imported data by clicking on the respective tab for the
   content type in the :term:`Main menu` or by clicking on *Content* in the
   :term:`Admin menu` and then on *View* next to the respective content type

Other Important Information
~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  **Always download an up-to-date template file** - the Excel
   spreadsheets can be used for most content and are dynamically
   generated. This means that if you add fields you will need to use a
   new template.

-  **Be patient with importing data** - the Excel file needs to be
   uploaded, parsed, then saved in your Scratchpad. Upload speeds are
   usually much slower than download speeds, so depending on your
   internet connection this may take some time (especially for large
   files >1MB). Importing medium-sized (3000-6000 term) taxonomies with
   rich data can take 5-15 minutes.
   
-  **Keep the browser window open when running an import** - if you
   close the browser window the import will stop.

-  **If a taxonomy imports in the wrong order, try running the import
   again** - if you have defined parent child relationships and a child
   is imported before its parent, it will be placed at the root of a
   taxonomy. Running an import again will update the taxonomy and the
   hierarchical relationship should now be correct.

-  **Use GUIDs if you have any** - A GUID is a global unique identifier
   for a record/node. GUIDs can be used to compare/synchronize different
   databases. Adding a GUID is not required, you only need it if your
   records/nodes were generated from an established database and you
   want to be able to update your data from this database at a later
   stage. Note that the GUID really has to be globally unique, at least
   across the whole Scratchpad. So it is not enough just add a number.
   Better is a combination like “Species2000-1”.   
