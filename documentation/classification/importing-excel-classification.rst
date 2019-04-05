Importing classification from Excel
===================================

*Taxonomic terms can be imported directly into a existing vocabulary
through a pre-populated template excel file. This can be very helpful
especially when large volume of terms has to be imported or when the
imported terms are already in a structured (or semi-structured) format
(e.g. an MS Excel file, an MS Access database).*

Create the vocabulary
~~~~~~~~~~~~~~~~~~~~~

Skip this section if you already have a taxonomy you wish to update

1. From the Admin menu go to *Structure > Taxonomy*
2. Click *+ Add vocabulary*
3. Provide the Name of your new classification
4. Select if the classification is Biological or non-biological. This is a very important step

   .. only:: training

        If you are using the Botanical :ref:`training-material`, you might want to name your vocabulary "Lily2". 
        If using the Zoological training material, "Lice2"


5. Click *Save*

Downloading the template file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` select *Import*

2. Select *Taxonomy: Excel file import*

3. Select a vocabulary.

4. Download the Excel template.

   - Select the first template file if you wish to replace all contents of an existing classification.

   - Select the second template file if you wish to update (correct, add) terms in a existing classification


.. only:: training

    As part of the training course, we have only just created this Vocabulary, so both files will only contain the header information and no taxonomic name data.

    A pre-populated Excel file is included in the :ref:`training-material`. Open the file named TEMPLATE-import_into_*_classification.xls to view the structure.

Editing the template file
~~~~~~~~~~~~~~~~~~~~~~~~~

**Complete the columns as follows:**

**Term Name - Required field**

Provide the full term without the authority. e.g. \ *Thymus* or *Thymus
longicaulis* or *Thymus longicaulis subsp. longicaulis* Do not
italicize.

Always start with the higher taxa of your classification first

**Parent Term Name**

Provide the term name of the immediate hierarchical parent in your
classification. Provide the name as entered in the *Term Name* field

**Term description**

Provide some basic description for the term. This field should not be
confused with the taxon description that can be entered in detail from
the Taxon description content type

**GUID**

Provide a unique and at least 10 character long alphanumeric identifier
for the term. This identifier will be used throughout Scratchpads to
identify this term. If left empty the system will auto-generate one once
the file is uploaded

**Parent GUID**

Provide the GUID of the parent term. This can be left empty if you have
already provided the *Parent Term Name* or if the taxon is the highest
in the hierarchy.

**Associated accepted name (Name)**

Provide the name of the term that is the currently accepted name if the
taxon is not accepted. Should be left empty if the column *Associated
accepted name (TID)* or *Associated accepted name (GUID)* is filled.

**Associated accepted name (TID)**

Provide the TID number of the term that is the currently accepted name
if the taxon is not accepted. Should be left empty if the column
*Associated accepted name (name)* or *Associated accepted name (GUID)*
is filled.

**Associated accepted name (GUID)**

Provide the GUID number of the term that is the currently accepted name if the taxon is not accepted. Should


Importing the template file
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once you have made the desired changes and additions to the template file, you can import it back into your Scratchpad.

Select *Browse*, locate the file, and select *Import*.

.. figure:: /_static/ClassificationImportExcelBrowse.png


When the import is complete, the overlay will report the number of terms created.  

.. figure:: /_static/ClassificationImportExcelResult.png

To view your new classification and the taxon pages, close the overlay and go to the new tab with the classification's name in the *Main menu* - see :doc:`/classification/viewing-classification`.


.. only:: training

    We have now created two classifications for our Scratchpad. To save confusion, you may want to delete one of them  - :doc:`/classification/deleting-classification`.

