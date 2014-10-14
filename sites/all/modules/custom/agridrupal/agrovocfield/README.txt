The Agrovoc Field module is a module for manually indexing nodes using Agrovoc concepts.
This module creates a new CCK field (called Agrovoc and derived from the Content Taxonomy field) that allows to select Agrovoc terms through an autocomplete textbox connecting to the Agrovoc web services 2.0.
Being a CCK field synchronized with a taxonomy, it allows to exploit both CCK and taxonomy features in Drupal.
The module has very good and very well integrated multi-lingual support: when indexing a node, the module lists Agrovoc terms in the website currently active language, but then stores the selected terms in all the languages enabled in the website. When switching to a different language, the corresponding Agrovoc terms in that language will be displayed and when translating a node that has already been indexed, the translated Agrovoc terms will be displayed and the autocomplete field will only list terms in that language.
The author of this module is already in contact with the author of another Agrovoc Drupal module (http://drupal.org/project/agrovoc) to coordinate on development.
Both modules derive from the same initial developments in CONDESAN and use the same basic functionalities (a common API), but the Agrovoc module only integrates Agrovoc with the Drupal taxonomy, while the Agrovoc Field module integates it also with CCK.
Coordination on the development of these two modules will probably lead to the release of a co-maintained module called "Agrovoc API" and two different interface modules. 
The next version of the module (or the next version of the Agrovoc API module) will include the option to use the Agrovoc SPARQL endpoint instead of the soon obsolete Agrovoc web services 2.0. 
It was developed by CONDESAN in coordination with the AIMS team in FAO and with the IT team in GFAR.


INSTRUCTIONS FOR VERSION 7.x-2.1
 
First time you install your module in your Drupal 7 :
 
-For local searches, you need to store all terms of Agrovoc in your database. First, you need to download a file from

https://bitbucket.org/asanchez75/agrovocfield/downloads/agrovocterm.d7.csv.zip

and put (after uncompress it) inside folder named sites/all/modules/agrovocfield/db/. Next, execute update.php for loading data. This is mandatory if you want to setup local searches

- If you have problems to use update.php you can a sql file to store all AGROVOC terms in your database (table called agrovocfield_data). Use this file

https://bitbucket.org/asanchez75/agrovocfield/downloads/agrovocfield_data.d7.sql.zip


Update the old version of Agrovoc for Drupal 7:

In the last version of the module, we decided to change the multilingual approach of the vocabulary and work with the option: “Localize”.
This because we realized that using the option:“Translate” we get some problems with multi-lingual support, mainly for what concerns Views.
Using “Translate” terms, Drupal save different terms, one for each enabled language, and each node is linked only to the term in the language in which 
the node was indexed.
So If I search by term, for example in Spanish, I can t find nodes indexed with the same term but in a different language,
this could be logic but in Agridrupal , as document repository, we prefer to give the possibility to browse all documents in all languages.
Actually, to make Agrovoc and Localize taxonomy  completely multilingual, and to allow it working well with views, it needs also this module: i18n_localize_taxonomy_fixes.
This module is not on drupal.org yet, but you can download it here: http://agridrupal.org/downloads/AgriDrupal/Agridrupal7.x.7.9/modules.

So the correct  steps to upgrade Agrovoc module to last version are:
1)	Replace the module with the new version
2)	Move Agrovoc taxonomy from “translate” to “Localize”
3)	Run update.php
4)	Empty cache
5)	Install i18n_localize_taxonomy_fixes


