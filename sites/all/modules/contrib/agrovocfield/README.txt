The Agrovoc Field module is a module for manually indexing nodes using Agrovoc concepts.
This module creates a new CCK field (called Agrovoc and derived from the Content Taxonomy field) that allows to select Agrovoc terms through an autocomplete textbox connecting to the Agrovoc web services 2.0.
Being a CCK field synchronized with a taxonomy, it allows to exploit both CCK and taxonomy features in Drupal.
The module has very good and very well integrated multi-lingual support: when indexing a node, the module lists Agrovoc terms in the website currently active language, but then stores the selected terms in all the languages enabled in the website. When switching to a different language, the corresponding Agrovoc terms in that language will be displayed and when translating a node that has already been indexed, the translated Agrovoc terms will be displayed and the autocomplete field will only list terms in that language.
The author of this module is already in contact with the author of another Agrovoc Drupal module (http://drupal.org/project/agrovoc) to coordinate on development.
Both modules derive from the same initial developments in CONDESAN and use the same basic functionalities (a common API), but the Agrovoc module only integrates Agrovoc with the Drupal taxonomy, while the Agrovoc Field module integates it also with CCK.
Coordination on the development of these two modules will probably lead to the release of a co-maintained module called "Agrovoc API" and two different interface modules. 
The next version of the module (or the next version of the Agrovoc API module) will include the option to use the Agrovoc SPARQL endpoint instead of the soon obsolete Agrovoc web services 2.0. 
It was developed by CONDESAN in coordination with the AIMS team in FAO and with the IT team in GFAR.


INSTRUCTIONS FOR VERSION 6.x-3.x

Code refactoring done.

-New funcionalities were tested for PHP 5.3.x
-New approach based on OOP for remote or local retrieving of Agrovoc terms
-Importants functions were shifted and renamed from agrovocfield_autocomplete.module to agrovocfield.module to allow writing new widgets more easily
-2 widgets were added: agrovocfield_text and agrovocfield_automatic
agrovocfield_text widget is only a textfield but allow reuse clearly parts of its code for other widgets
-agrovocfield_automatic widget allows automatic indexing for body field OR filefield (provided by filefield module) OR upload file(provided by upload core module)
-For local searches, you need to store all AGROVOC terms in your database. First, you need to download a file from

https://bitbucket.org/asanchez75/agrovocfield/downloads/agrovocterm.d6.csv.zip 

and put (after uncompress it) inside folder named sites/all/modules/agrovocfield/db/. Next, execute update.php for loading data. This is mandatory if you want to setup local searches

- If you have problems to use update.php you can a sql file to store all AGROVOC terms in your database (table called agrovocterm). Use this file

https://bitbucket.org/asanchez75/agrovocfield/downloads/agrovocterm.d6.sql.zip

INSTRUCTIONS FOR VERSION 7.x-1.x

-For local searches, you need to store all terms of Agrovoc in your database. First, you need to download a file from

https://bitbucket.org/asanchez75/agrovocfield/downloads/agrovocterm.d7.csv.zip

and put (after uncompress it) inside folder named sites/all/modules/agrovocfield/db/. Next, execute update.php for loading data. This is mandatory if you want to setup local searches

- If you have problems to use update.php you can a sql file to store all AGROVOC terms in your database (table called agrovocfield_data). Use this file

https://bitbucket.org/asanchez75/agrovocfield/downloads/agrovocfield_data.d7.sql.zip

