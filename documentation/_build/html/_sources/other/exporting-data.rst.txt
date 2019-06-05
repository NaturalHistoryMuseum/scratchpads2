Exporting data
==============

Introduction
------------

Scratchpads support multiple ways of exporting your data. You can export
data in specific content types as an .xls file or generate a standard
compliant Darwin Core Archive file that includes all of your Scratchpads
data.

Instructions
------------

Exporting as .xls
~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Import*
2. From the *Select Import* drop-down menu select accordingly

   -  Select *Excel file Import* under *Files* to export metadata of
      uploaded media files
   -  Select *Excel file Import* under *Nodes* to export data present of
      a specific content type
   -  Select *Excel file Import* under *Taxonomy* to export one of the
      existing classifications in your Scratchpad

3. From the second drop-down menu select the media type or content type
   you wish to export as .xls file
4. Click on *Excel template file* to populate and download the file

The .xls file will include all your data in rows under columns that
represent the fields in the corresponding media type, content type or
taxonomy

Exporting as DwC-A
~~~~~~~~~~~~~~~~~~

Darwin Core archive files can be used to automatically feed your data to
other web-services (e.g. EOL, GBIF). You can learn more about DwC-A
files `here`_

Enabling the DwC-A module
^^^^^^^^^^^^^^^^^^^^^^^^^

1. From the :term:`Admin menu` got to *Structure > Tools*

2. Browse through the page and locate *DarwinCore Archive (DwC-A)
   export* under *Export*

3. Click to enable the module or leave as is if already *ON*

4. Click *Save*. Allow some time before proceeding to the next step

Downloading the file
^^^^^^^^^^^^^^^^^^^^

1. Open a new browser tab and enter the address of your scratchpad site
   adding
   */dhttps://github.com/NaturalHistoryMuseum/scratchpads-help-files/raw/master/files/wca.zip*.
   (E.g.
   xxxx.myspecies.info/dhttps://github.com/NaturalHistoryMuseum/scratchpads-help-files/raw/master/files/wca.zip)
2. You will be prompted to download a .zip file. Accept and save in your
   local or network disk

Please note that DwC-A files are not human readable, they are built to
allow the sharing and re-use of biodiversity related information between
machines over the web. You can use this archive file to contribute your
scratchpad data to other Biodiversity information services like EOL or
GBIF.

Exporting references
~~~~~~~~~~~~~~~~~~~~

Scratchpads can export references in several formats (incl. BibTex RTF
XML RIS)

1. Go to the literature page of your Scratchpad

2. Select the references you want to export through the available
   facets. If no filtering is used all references will be exported

3. Click on the export format file you want to populate and download
   next to *Export selection as*

   .. figure:: /_static/Literature_export.png


4. You will be prompted to download a file, save the file in your local or network disk