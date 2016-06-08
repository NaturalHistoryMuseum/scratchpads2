INTRODUCTION
------------

The Apache Solr Field Collection module allows content stored within a field
collection (http://drupal.org/project/field_collection) to be indexed for
search in Apache Solr (http://drupal.org/project/apachesolr).

The content stored within each field collection will be indexed as part of the
entity that the field collection is attached to.

If the Facet API module (http://drupal.org/project/facetapi) is enabled, the
fields attached to the field collection will also be available to add as search
facets.

IMPORTANT NOTES
---------------

Using this module when the Facet API module is enabled currently requires
applying the patch at http://drupal.org/node/1679392 to the Apache Solr module.

CREDITS
-------

This project was sponsored by Advomatic (http://advomatic.com).
