CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* Troubleshooting
* Advantages
* Limitations
* Maintainers

INTRODUCTION
------------

This module provides a Views integration to the Apache Solr Search Integration
project. It provides the views plugins and handlers needed to be able to
create a view that fetches its results from Apachesolr index, without hitting
the database.

REQUIREMENTS
------------

Requires Apache Solr Search https://www.drupal.org/project/apachesolr
Requires the use of an Apachesolr search index.

INSTALLATION
------------

Download and install the module as normal.

CONFIGURATION
-------------

First make sure that Apachesolr module is configured correctly.
If in the page "admin/config/search/apachesolr/settings" the Apachesolr
environment is green and working, you can test this by using the site
search page.
Create a view from admin/structure/views/add and choose Apachesolr search. All
of the fields indexed in solr should be available as views fields/filters/sort.
Configure the view as needed and save it.

TROUBLESHOOTING
---------------

This module can diagnose problems with solr entity indexing:
https://www.drupal.org/project/solr_devel
Solr admin adds even more options, and is usually available at:
http://[host]:8983/solr/admin/

ADVANTAGES
----------

It is impossible to have duplicate results, unlike database views.

Queries do not hit the database, and so they offload the database server.

Views are very easy to customize, unlike search pages.

LIMITATIONS
-----------

Exposed filters are only textfields. See [#1807028].

Search facets are supported, but may not work reliably in combination with
views filters.

Search sort block is not supported. See [#443410].

Multiple Apachesolr views on the same page do not work See [#1766254].

Most entity field types are not sent to Solr, and will not be available as
views handlers. Apachesolr views doesn't index the fields itself. This is
done from other modules by implementing hook_apachesolr_field_mappings().

MAINTAINERS
-----------

The 7.x branch is maintained by Miroslav Vladimirov Banov.
https://www.drupal.org/user/1509224

The 6.x branch is unmaintained.
