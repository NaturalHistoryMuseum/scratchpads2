/* $Id: README.txt,v 2.5.2.5.2.5 2011/01/06 18:08:27 danielkm Exp $ */


                      TAXONOMY CSV IMPORT/EXPORT
                      ==========================


-- SUMMARY --
  ---------

This module allows to import or export taxonomy from or to a CSV
(comma-separated values) local or distant file or a copy-and-paste text.

When you want to import a vocabulary, a taxonomy, a structure or a simple list
of terms in your Drupal site, you have three main choices: migration modules,
Taxonomy XML and Taxonomy CSV.

For migration modules, see http://groups.drupal.org/node/21338 and
http://groups.drupal.org/soc-2006-import-export-api.

Taxonomy XML (http://drupal.org/project/taxonomy_xml) is perfect for
standardized taxonomies and vocabularies importation. Despite its name, it can
import csv files too, but only if they are ISO 2788 formatted.

When you want to quick import a non-standardized vocabulary, for example an
old thesaurus or a simple list of children, synonyms, related terms,
descriptions or weights of a set of terms, Taxonomy CSV is simpler to use.

Other modules are available such as Taxonomy batch operations
(http://drupal.org/project/taxonomy_batch_operations). It has only Drupal 4.7
and Drupal 5 releases. Drupal 6 release exists too, but it's an unofficial one.

For export, you can use Taxonomy XML too or one of backup modules. Taxonomy CSV
is a more specialised tool which allows more precise tuning.

For a full description of the module, visit the project page:
  http://drupal.org/project/taxonomy_csv

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/taxonomy_csv

See Advanced help in Help > Taxonomy CSV import/export for updated help.


-- WARNING --
  ---------

Use at your risk. Even if many informations are displayed, taxonomy_csv module
does not tell you what it's gonna do before it's doing it, so make sure you have
a backup so you can roll back if necessary.

It's recommended to use the "autocreate" or the "duplicate vocabulary" choices
to check your csv file before import in a true vocabulary.


-- TROUBLESHOOTING --
  -----------------

See online issues:
  http://drupal.org/project/issues/taxonomy_csv


-- FAQ --
  -----

See Advanced help in Help > Taxonomy CSV import/export.

See technical infos in TECHINFO.txt.


-- CONTACT --
  ---------

Current maintainers:
* Daniel Berthereau (Daniel_KM) => http://drupal.org/user/428555
* Dennis Stevense (naquah)      => http://drupal.org/user/26342

First version has been written by Dennis Stevense (naquah).
Major rewrite 6.x-2.0 and subsequents by Daniel Berthereau (Daniel_KM).

First version of the project has been sponsored by
* The Site Mechanic (http://www.thesitemechanics.com)