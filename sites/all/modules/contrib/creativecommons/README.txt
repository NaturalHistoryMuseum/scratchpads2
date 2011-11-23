
-- SUMMARY --

The Creative Commons module allows users to assign a Creative Commons license to
the content of a node, or to specify a site-wide license. It uses to Creatve 
Commons API to retrieve up-to-date license information. Licenses are diplayed 
using a Creative Commons Node License block and the Creative Commons Site 
License block. The module also supports some license metadata fields. License
information is output using ccREL RDFa inside the blocks, and can optionally be
output as RDF/XML in the body of a node.

Creative Commons search is available at /search/creativecommons/, and (if the
Views module is installed and enabled) a Creative Commons view is available at
/creativecommons. Creative Commons license information and metadata are 
available to the Views module.

For a full description of the module, visit the project page:
  http://drupal.org/project/creativecommons

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/creativecommons


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Permissions >>
  creativecommons module:

  - administer creative commons

    Users can customize the module settings in Administer >> Settings >> 
    Creative Commons

  - attach creative commons

    Users will be able to attach license information to the content of a node.

  - use creative commons user defaults
  
    Users will be able to set their own defaults, independent of site defaults
    (but still subject to site license availability settings).

* Set available license types, required/available metadata and display settings
  Administer >> Settings >> Creative Commons. To make it mandatory to specify a
  license, simply make the 'None' type unavailable.

* Set default license type and jurisdiction in Administer >> Settings >> 
  Creative Commons >> site defaults. Here, you can set the default license to be
  used as a site-wide license if you wish, and you can include any relevant
  metadata.
  
* Enable Creative Commons licensing for desired content types in Administer >>
  Settings >> Creative Commons >> content types. For example, you might wish to 
  allow Creative Commons licensing for blog posts, but not forum posts.
 
* In your Drupal user account settings, you can set a jurisdiction or default
  license to override the site defaults.


-- CONTACT --

Current maintainers:
* Blaise Alleyne (balleyne) - http://drupal.org/user/362044
* Kevin Reynen (kreynen) - http://drupal.org/user/48877
* Turadg Aleahmad (turadg) - http://drupal.org/user/463154

Initial development done by Kevin Reynen (kreynen). Rewrite for Drupal 6 
by Blaise Alleyne (balleyne) as part of the Google Summer of Code 2009.

This project has been sponsored by:
* Google - http://www.google.com/
  A project to update and expand the module for Drupal 6 was completed by Blaise
  Alleyne as part of the Google Summer of Code 2009
  
* Alleyne Inc. - http://www.alleyneinc.net/
  Alleyne Inc. sponsors Blaise's development beyond the Google Summer of Code.


