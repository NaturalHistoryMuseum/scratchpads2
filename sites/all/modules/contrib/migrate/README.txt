
The Migrate module provides a flexible framework for migrating content into Drupal 
from other sources (e.g., when converting a web site from another CMS to Drupal). 
Out-of-the-box, support for creating Drupal nodes, taxonomy terms, comments, and 
users are included. Plugins permit migration of other types of content.

Usage
-----
Documentation is at http://drupal.org/node/415260. To get started, enable the 
migrate_example module and browse to admin/content/migrate to see its dashboard. 
The code for this migration is in migrate_example/beer.inc (advanced examples are
in wine.inc). Mimic that file in order to specify your own migrations. 

The Migrate module itself has support for migration into core objects. Support
for migration involving contrib modules is in the migrate_extras module. 

Known issues
------------
A user migration with systemOfRecord == DESTINATION will drop pictures from user
records due to core bug http://drupal.org/node/935592 - the simpletests report an
error reflecting this. We have not developed a work-around.

Upgrading
---------
Do not attempt to upgrade directly from Migrate 1 to Migrate 2! There is no
automated path to upgrade - your migrations (formerly known as "content sets")
must be reimplemented from scratch. It is recommended that projects using
Migrate 1 stay with Migrate 1, and that Migrate 2 be used for any new migration
projects.

Acknowledgements 
----------------
Much of the Migrate module functionality was sponsored by Cyrve, for its clients GenomeWeb 
(http://www.genomeweb.com), The Economist (http://www.economist.com), and Examiner.com 
(http://www.examiner.com). 

Authors
-------
Mike Ryan - http://drupal.org/user/4420
Moshe Weitzman - http://drupal.org/user/23
