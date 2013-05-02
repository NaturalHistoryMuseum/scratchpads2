
Lexicon
http://www.drupal.org/project/lexicon
=====================================


DESCRIPTION
-----------
A lexicon is a stock of terms used in a particular profession, subject or style; a vocabulary. 
The Lexicon module generates one or more Lexicon pages based on terms in taxonomies and optionally 
marks and links them in the content.

The Lexicon module is a fork of the Glossary module (http://www.drupal.org/project/glossary) that 
was changed to suit the needs of a project for the Dutch government and later ported to Drupal 7.
The main reason for the changes is compliancy with the web guidelines of the Dutch government
(http://www.webguidelines.nl).


DIFFERENCES BETWEEN LEXICON AND GLOSSARY MODULE
-----------------------------------------------
The main differences with the Glossary module are that the Lexicon module:

* Produces output that is valid W3C XHTML 1.0 Strict.
* Enables showing lists of terms without marking the terms in content (this is still an option).
  You can choose which vocabularies act as Lexicons and you don’t bind them to input filters.
* Has configurable paths and titles for each Lexicon.
* Optionally inserts a “go to top” link for each section on the Lexicon page.
* Optionally “scrolls” to internal links on the Lexicon page.
* Lexicon is also available for Drupal 7
* From the 7.x-1.4 version and up the Lexicon module uses template files instead of theme functions 
  for output rendering


DRUPAL 6 VERSUS DRUPAL 7 VERSION
--------------------------------
Shortly after Drupal 7.0 was released the Drupal 7 version of the Lexicon module was developed.
From that moment new features only go into the Drupal 7 version unless there is lot of demand for a 
feature to be backported to the Drupal 6 version.

Related terms and synonyms for terms have been stripped out of the Taxonomy module in Drupal 7 and 
the functionality should now be created using Fields. The Drupal 7 version of the Lexicon module lets 
the administrator configure which fields of a vocabulary are used as fields for related terms, 
synonyms and/or term images.

Two-way relations of terms is not implemented (yet).

Development of this module is sponsored by LimoenGroen (http://www.limoengroen.nl).


REQUIREMENTS
------------
Drupal 7.x
Taxonomy
Fields


INSTALLING
----------
1. To install the module copy the 'lexicon' folder to your sites/all/modules directory.

2. Go to admin/build/modules. Enable the Lexicon module.
Read more about installing modules at http://drupal.org/node/70151


CONFIGURING AND USING
---------------------
1. Create a taxonomy vocabulary for each Lexicon.
2. Go to admin/settings/lexicon.
3. Select the created taxonomy vocabularies you want to use as Lexicons.
4. Set the desired behavior of the Lexicon module and save the configuration.
5. Configure the alphabar and save the configuration.
6. Optionally set the paths and titles of the Lexicons and save the configuration.
7. Optionally configure fields for synonyms, related terms and term images.
8. Optionally put the suggested menu-items in the right menu and activate them.
9. Enable the Lexicon filter on the appropriate text formats.

Have fun!
