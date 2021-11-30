

"It feeds"


FEEDS
=====

An import and aggregation framework for Drupal.
http://drupal.org/project/feeds

Features
========

- Pluggable import configurations consisting of fetchers (get data) parsers
  (read and transform data) and processors (create content on Drupal).
-- HTTP upload (with optional PubSubHubbub support).
-- File upload.
-- CSV, RSS, Atom parsing.
-- Creates nodes or terms.
-- Creates lightweight database records if Data module is installed.
   http://drupal.org/project/data
-- Additional fetchers/parsers or processors can be added by an object oriented
   plugin system.
-- Granular mapping of parsed data to content elements.
- Import configurations can be piggy backed on nodes (thus using nodes to track
  subscriptions to feeds) or they can be used on a standalone form.
- Unlimited number of import configurations.
- Export import configurations to code.
- Optional libraries module support.

Requirements
============

- CTools 7.x-1.x
  http://drupal.org/project/ctools
- Job Scheduler
  http://drupal.org/project/job_scheduler
- Drupal 7.x
  http://drupal.org/project/drupal
- PHP safe mode is not supported, depending on your Feeds Importer configuration
  safe mode may cause no problems though.

Installation
============

- Install Feeds, Feeds Admin UI.
- To get started quick, install one or all of the following Feature modules:
  Feeds News, Feeds Import, Feeds Fast News (more info below).
- Make sure cron is correctly configured http://drupal.org/cron
- Go to import/ to import data.

SimplePie Installation
======================

- To install the SimplePie parser plugin, complete the following steps:
  1. Download SimplePie from http://simplepie.org/downloads. The recommended
     version is: 1.3.
  2. Decompress the downloaded zip file.
  3. Rename the uncompressed folder to 'simplepie'.
     For example rename 'simplepie-simplepie-e9472a1' to 'simplepie'.
  4. Move the folder to sites/all/libraries. The final directory structure
     should be sites/all/libraries/simplepie.
  5. Flush the Drupal cache.
  6. The SimplePie parser should be available now in the list of parsers.

Feature modules
===============

Feeds ships with three feature modules that can be enabled on
admin/build/modules or - if you are using Features - on admin/build/features.
http://drupal.org/project/features

The purpose of these modules is to provide a quick start for using Feeds. You
can either use them out of the box as they come or you can take them as samples
to learn how to build import or aggregation functionality with Feeds.

The feature modules merely contain sets of configurations using Feeds and in
some cases the modules Node, Views or Data. If the default configurations do not
fit your use case you can change them on the respective configuration pages for
Feeds, Node, Views or Data.

Here is a description of the provided feature modules:

- Feeds News -

This feature is a news aggregator. It provides a content type "Feed" that can
be used to subscribe to RSS or Atom feeds. Every item on such a feed is
aggregated as a node of the type "Feed item", also provided by the module.

What's neat about Feeds News is that it comes with a configured View that shows
a list of news items with every feed on the feed node's "View items" tab. It
also comes with an OPML importer filter that can be accessed under /import.

- Feeds Import -

This feature is an example illustrating Feeds' import capabilities. It contains
a node importer and a user importer that can be accessed under /import. Both
accept CSV or TSV files as imports.

PubSubHubbub support
====================

Feeds supports the PubSubHubbub publish/subscribe protocol. Follow these steps
to set it up for your site.
https://github.com/pubsubhubbub/

- Go to admin/build/feeds and edit (override) the importer configuration you
  would like to use for PubSubHubbub.
- Choose the HTTP Fetcher if it is not already selected.
- On the HTTP Fetcher, click on 'settings' and check "Use PubSubHubbub".
- Optionally you can use a designated hub such as http://superfeedr.com/ or your
  own. If a designated hub is specified, every feed on this importer
  configuration will be subscribed to this hub, no matter what the feed itself
  specifies.

Libraries support
=================

If you are using Libraries module, you can place external libraries in the
Libraries module's search path (for instance sites/all/libraries. The only
external library used at the moment is SimplePie.

Libraries found in the libraries search path are preferred over libraries in
feeds/libraries/.

Transliteration support
=======================

If you plan to store files with Feeds - for instance when storing podcasts
or images from syndication feeds - it is recommended to enable the
Transliteration module to avoid issues with non-ASCII characters in file names.
http://drupal.org/project/transliteration

API Overview
============

See "The developer's guide to Feeds":
http://drupal.org/node/622700

Running the Drush integration tests
===================================

In order the run Drush integration tests, Drush itself needs to be installed
with its *dev dependencies*. Furthermore, the phpunit version that comes with
Drush should be used for running the tests (instead of a globally installed
phpunit), as that one has proven to be compatible with the Drush tests.

  1. Git clone of Drush 8.

       git clone --branch 8.x https://github.com/drush-ops/drush.git
       cd drush

  2. Install Drush with dev dependencies using Composer.

       composer install

     And ensure that the following text is displayed:

       "Loading composer repositories with package information
       Installing dependencies (including require-dev) from lock file"

     Especially note that Composer says 'including require-dev'. This means that
     the Drush dev dependencies are installed (including phpunit).

  3. Execute a command like the following:

       UNISH_NO_TIMEOUTS=1 UNISH_DRUPAL_MAJOR_VERSION=7 /path/to/drush/vendor/bin/phpunit --configuration /path/to/drush/tests /path/to/feeds/tests/drush

     Replace '/path/to' with the appropriate path to the directory in question.
     Also be sure to point to the phpunit version that comes with Drush.

     So if Drush is installed in /users/megachriz/drush and the Feeds module is
     located at /users/megachriz/Sites/drupal7/sites/all/modules/feeds:

       UNISH_NO_TIMEOUTS=1 UNISH_DRUPAL_MAJOR_VERSION=7 /users/megachriz/drush/vendor/bin/phpunit --configuration /users/megachriz/drush/tests /users/megachriz/Sites/drupal7/sites/all/modules/feeds/tests/drush

Debugging
=========

Set the Drupal variable 'feeds_debug' to TRUE (i. e. using drush). This will
create a file /tmp/feeds_[my_site_location].log. Use "tail -f" on the command
line to get a live view of debug output.

Note: at the moment, only PubSubHubbub related actions are logged.

Performance
===========

See "The site builder's guide to Feeds":
http://drupal.org/node/622698

Hidden settings
===============

Hidden settings are variables that you can define by adding them to the $conf
array in your settings.php file.

Name:        feeds_debug
Default:     FALSE
Description: Set to TRUE for enabling debug output to
             /DRUPALTMPDIR/feeds_[sitename].log

Name:        feeds_library_dir
Default:     FALSE
Description: The location where Feeds should look for libraries that it uses.
             You can use this variable to override the libraries that are in
             the Feeds libraries folder, for example "http_request.inc".

Name:        feeds_importer_class
Default:     'FeedsImporter'
Description: The class to use for importing feeds.

Name:        feeds_source_class
Default:     'FeedsSource'
Description: The class to use for handling feed sources.

Name:        feeds_process_limit
Default:     50
             The number of nodes feed node processor creates or deletes in one
             page load.

Name:        http_request_timeout
Default:     15
Description: Timeout in seconds to wait for an HTTP get request to finish.
Note:        This setting could be overridden per importer in admin UI :
             admin/structure/feeds/<your_importer>/settings/<your_fetcher> page.

Name:        feeds_never_use_curl
Default:     FALSE
Description: Flag to stop feeds from using its cURL for http requests. See
             http_request_use_curl().

Name:        feeds_http_file_cache_dir
Default:     private://feeds/cache
Description: The location on the file system where results of HTTP requests are
             cached.

Name:        feeds_in_progress_dir
Default:     private://feeds/in_progress
Description: The location on the file system where temporary files are stored
             that are in progress of being imported.

Name:        feeds_sync_cache_feeds_http_interval
Default:     21600
Description: How often the feeds cache directory should be checked for orphaned
             cache files.

Name:        feeds_use_mbstring
Default:     TRUE
Description: The extension mbstring is used to convert encodings during parsing.
             The reason that this can be turned off is to be able to test Feeds
             behavior when the extension is not available.

Glossary
========

See "Feeds glossary":
http://drupal.org/node/622710
