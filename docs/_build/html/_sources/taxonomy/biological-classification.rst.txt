Adding a biological classification
==================================

*Introduction*

Before adding data other than static pages to a Scratchpad, we need to add the taxonomic names. This is done via the :term:`Taxonomy` module, which contains controlled vocabularies. 

A vocabulary is a list of terms. These do not have to be biological classifications. Country lists, keywords etc. can all be treated as “taxonomies” in the context of your site. Taxonomies are the backbone of a Scratchpad and link together the content. They are used to generate taxon pages. Before you add a set of taxonomic names to your Scratchpad you need to add the vocabulary for it.

1. Click on *Structure* in the :term:`Admin menu` and click on *Taxonomy* near the bottom

   .. figure:: /_static/AdminStructure.jpg

    “Structure” admin page, listing the menu options: Blocks; Contact form; Content types; Menus; Taxonomy; and Tools


2. Click on the *Add vocabulary* link

   .. figure:: /_static/AdminTaxonomy.jpg

    Taxonomy page showing “Add vocabulary” button near the page title

3. Minimally enter the vocabulary *Name*.

   .. only:: training

        If you are using the Botanical :ref:`training-material`, you might want to name your vocabulary "Lily". 
        If using the Zoological training material, "Lice"

4. Add a vocabulary *Description*. 

5. Select which kind of biological classification you are creating (“Animal” in our example)

   .. figure:: /_static/AddVocabulary.jpg

    Add vocabulary form showing name, description and biological classification fields

6. Click *Save*


Adding terms
------------

Having created a vocabulary you now need to add the list of terms (names). 

This can be done by either importing a classification through the Scratchpads Classification Service (see :doc:`/taxonomy/importing-classification`), or by creating and importing your own
classification from Excel (see :doc:`/taxonomy/importing-excel-classification`). Alternatively
you can add terms one by one by clicking on *Add terms* when you are on the *Taxonomy* admin page or by using the taxonomic editor (see :doc:`/taxonomy/editing-classification`).


.. only:: training

    On this training course, we will first look at importing a classification :doc:`/taxonomy/importing-classification`.


Troubleshooting
---------------

Missing tab after creating classification
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If after creating a classification and adding terms to it a tab does not appear for it in the main tab bar try clearing the caches: 

1. Go to *Admin > config > development > clearcache*

3. Click *Clear all caches* - this will take a number of seconds to complete (likely more than 30 seconds)

