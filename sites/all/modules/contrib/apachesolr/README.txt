
This module integrates Drupal with the Apache Solr search platform. Solr search
can be used as a replacement for core content search and boasts both extra
features and better performance. Among the extra features is the ability to have
faceted search on facets ranging from content author to taxonomy to arbitrary
Field API fields.

The module comes with a schema.xml, solrconfig.xml, and protwords.txt file which
must be used in your Solr installation.

This module depends on the search framework in core.  When used in combination
with core search module, Apache Solr is not the default search. Access it via a
new tab on the default search page, called "Search".  You may configure it
to be default at ?q=admin/config/search/settings

Updating from 6.x
-----------------
Make sure that you have first updated to the latest 6.x version on the relevant
branch and that you have run all schema updates.  You will have to install the
new schema.xml and solrconfig.xml files, and restart the Solr server (or core)
and delete your index and reindex all content.

Installation
------------

Prerequisite: Java 5 or higher (a.k.a. 1.5.x).  PHP 5.2.4 or higher.

Install the Apache Solr Drupal module as you would any Drupal module.

Before enabling it, you must also do the following:

Get the PHP library from the external project. The project is
found at:  http://code.google.com/p/solr-php-client/
From the apachesolr module directory, run this command:

svn checkout -r22 http://solr-php-client.googlecode.com/svn/trunk/ SolrPhpClient

Alternately you may download and extract the library from
http://code.google.com/p/solr-php-client/downloads/list

Make sure to select a r22 archive, either of these two:
http://solr-php-client.googlecode.com/files/SolrPhpClient.r22.2009-11-09.zip
http://solr-php-client.googlecode.com/files/SolrPhpClient.r22.2009-11-09.tgz

Note that revision 22 is the currently tested and required revision. 
Make sure that the final directory is named SolrPhpClient under the apachesolr
module directory.  

If you are maintaing your code base in subversion, you may choose instead to 
use svn export or svn externals. For an export (writing a copy to your local
directory without .svn files to track changes) use:

svn export -r22 http://solr-php-client.googlecode.com/svn/trunk/ SolrPhpClient

Instead of checking out, externals can be used too. Externals can be seen as 
(remote) symlinks in svn. This requires your own project in your own svn ]
repository, off course. In the apachesolr module directory, issue the command:

svn propedit svn:externals .

Your editor will open. Add a line

SolrPhpClient -r22 http://solr-php-client.googlecode.com/svn/trunk/

On exports and checkouts, svn will grab the externals, but it will keep the 
references on the remote server.

Those without svn, etc may also choose to try the bundled Acquia Search
download, which includes all the items which are not in Drupal.org CVS due to 
CVS use policy. See the download link here: 
http://acquia.com/documentation/acquia-search/activation

Download the latest Solr 1.4.x release (e.g. 1.4.1) from:
http://www.apache.org/dyn/closer.cgi/lucene/solr/

Unpack the tarball somewhere not visible to the web (not in your apache docroot
and not inside of your drupal directory).

The Solr download comes with an example application that you can use for
testing, development, and even for smaller production sites. This
application is found at apache-solr-1.4.1/example.

Move apache-solr-1.4.1/example/solr/conf/schema.xml and rename it to
something like schema.bak. Then move the schema.xml that comes with the
ApacheSolr Drupal module to take its place.

Similarly, move apache-solr-1.4.1/example/solr/conf/solrconfig.xml and rename
it like solrconfig.bak. Then move the solrconfig.xml that comes with the
ApacheSolr Drupal module to take its place.

Finally, move apache-solr-1.4.1/example/solr/conf/protwords.txt and rename
it like protwords.bak. Then move the protwords.txt that comes with the
ApacheSolr Drupal module to take its place.

Now start the solr application by opening a shell, changing directory to
apache-solr-1.4.1/example, and executing the command java -jar start.jar

Test that your solr server is now available by visiting
http://localhost:8983/solr/admin/

Now, you should enable the "Apache Solr framework" and "Apache Solr search" 
modules. Check that you can connect to Solr at ?q=admin/setting/apachesolr
Now run cron on your Drupal site until your content is indexed. You
can monitor the index at ?q=admin/settings/apachesolr/index

The solrconfig.xml that comes with this modules defines auto-commit, so
it may take a few minutes between running cron and when the new content
is visible in search.

Enable blocks for facets first at Administer > Site configuration > Apache Solr > Enabled filters,
then position them as you like at Administer > Site building > Blocks.   

Configuration variables
--------------

The module provides some (hidden) variables that can be used to tweak its
behavior:

 - apachesolr_luke_limit: the limit (in terms of number of documents in the
   index) above which the module will not retrieve the number of terms per field
   when performing LUKE queries (for performance reasons).

 - apachesolr_tags_to_index: the list of HTML tags that the module will index
   (see apachesolr_add_tags_to_document()).

- apachesolr_exclude_nodeapi_types: an array of node types each of which is
   an array of one or more module names, such as 'comment'.  Any type listed
   will have any listed modules' hook_node_update_index() implementation skipped
   when indexing. This can be useful for excluding comments or taxonomy links.

 - apachesolr_ping_timeout: the timeout (in seconds) after which the module will
   consider the Apache Solr server unavailable.

 - apachesolr_optimize_interval: the interval (in seconds) between automatic
   optimizations of the Apache Solr index. Set to 0 to disable.

 - apachesolr_cache_delay: the interval (in seconds) after an update after which
   the module will requery the Apache Solr for the index structure. Set it to
   your autocommit delay plus a few seconds.

 - apachesolr_service_class: the Apache_Solr_Service class used for communicating
   with the Apache Solr server.

 - apachesolr_query_class: the default query class to use.
 
 - apachesolr_index_comments_with_node: TRUE | FALSE. Whether to index comments
   along with each node.

 - apachesolr_cron_mass_limit: update or delete at most this many documents in
   each Solr request, such as when making {apachesolr_search_node} consistent
   with {node}.

Troubleshooting
--------------
Problem:
Links to nodes appear in the search results with a different host name or
subdomain than is preferred.  e.g. sometimes at http://example.com
and sometimes at http://www.example.com

Solution:
Set $base_url in settings.php to insure that an identical absolute url is
generated at all times when nodes are indexed.  Alternately, set up a re-direct
in .htaccess to prevent site visitors from accessing the site via more than one
site address.



Themers
----------------

See inline docs in apachesolr_theme and apachesolr_search_theme functions 
within apachesolr.module and apachesolr_search.module.

