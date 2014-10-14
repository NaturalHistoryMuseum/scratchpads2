

                      TAXONOMY CSV IMPORT/EXPORT
                      ==========================


-- SUMMARY --
  ---------

This module allows to import or export taxonomy from or to a CSV
(comma-separated values) local or distant file or a copy-and-paste text.

When you want to quick import a non-standardized vocabulary, for example an
old thesaurus or a simple list of children, synonyms, related terms,
descriptions or weights of a set of terms, Taxonomy CSV is simpler to use. It
can manage internationalized vocabularies with i18n_taxonomy, a submodule of
i18n module (see http://drupal.org/project/i18n).


Some other modules allow to import and export taxonomies:
* Taxonomy XML (http://drupal.org/project/taxonomy_xml) is perfect for
  standardized taxonomies and vocabularies importation. Despite its name, it can
  import csv files too, but only if they are ISO 2788 formatted.

* Migration modules, see http://groups.drupal.org/node/21338 and
  http://groups.drupal.org/soc-2006-import-export-api.


Taxonomy CSV is a more specialised tool which allows more precise tuning.
It's designed to be used when the website is building. After that, it's
recommended to disable it.


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

See online issue at http://drupal.org/project/issues/taxonomy_csv.


-- FAQ --
  -----

See Advanced help in Help > Taxonomy CSV import/export.

See technical infos in TECHINFO.txt.


-- LICENCE --
  ---------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


-- CONTACT --
  ---------

Current maintainers:
* Daniel Berthereau (Daniel_KM) => http://drupal.org/user/428555
* Dennis Stevense (naquah)      => http://drupal.org/user/26342

First version has been written by Dennis Stevense (naquah).
Major rewrite 6.x-2.0 and subsequents by Daniel Berthereau (Daniel_KM).

First version of the project has been sponsored by
* The Site Mechanic (http://www.thesitemechanics.com)


-- COPYRIGHT --
  ---------

Copyright © 2007-2009 Dennis Stevense (naquah)
Copyright © 2009-2012 Daniel Berthereau <daniel.drupal@berthereau.net>
