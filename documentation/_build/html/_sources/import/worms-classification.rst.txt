:orphan:

.. Worms not needed for Uruguay, need to get working: See https://github.com/NaturalHistoryMuseum/scratchpads2/issues/5719


Importing a worms classification
================================

Instructions
~~~~~~~~~~~~

Creating a new classification
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to import data from the WoRMS service you will need first to
have set up a new biological classification that will be used to hold
imported the taxonomic terms. You can find detailed documentation on how
to do so in the :doc:`/classification/biological-classification` page.

Enabling the WoRMS importer
^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. From the administration menu go to *Structure > Tools*
2. Under the *Data provider* section find the *WoRMS - Importer*
3. Click on the slider next to it to enable it. If the importer is
   already enabled go to `Importing the data`_ section.

   .. figure:: /_static/Worms_importer_enabled.png

1. Click *Save*

Importing the data
^^^^^^^^^^^^^^^^^^
1. From the administration menu bar go to Import >
2. From the first drop-down menu select *Taxonomy > WoRMS web service*
3. Select the taxonomy you created in step 1: `Creating a new classification`_
4. Check whether the message *’The WoRMS service appears to be running*
   appears. If not do not proceed and check again later
5. Enter the root term of your classification. All taxa below this term
   will be imported with the highest level taxon included. (e.g. By
   entering the genus *Carcharhinus* the importer will create a
   classification with the Genus *Carcharhinus* as the top level taxon
   and all the infra-generic taxa.)
6. Click *Search WoRMS*
7. If more than one results select the desired highest level taxon

   .. figure:: /_static/Worms_importer_select_taxon.png


1. Click *Import from WoRMS*

Important notes
~~~~~~~~~~~~~~~

-  Disregard the progress bar while importing, as the service does not return a
   total taxa to be imported value we are unable to keep track of the
   progress. The number of imported taxa will show beneath this bar
-  **Currently you can not update an existing classification through the
   WoRMS service**. This means that if you want to import an updated
   version of a classification held by WoRMS you will have to delete and
   rebuild the taxonomy, losing all associations with other content
   types.