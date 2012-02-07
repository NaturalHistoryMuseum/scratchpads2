********************************************************************
                P A G E    T I T L E    M O D U L E
********************************************************************
Original Author: Robert Douglass
Current Maintainers: Nicholas Thompson and John Wilkins

********************************************************************
DESCRIPTION:

   This module gives you control over the page title. It gives you the chance
   to provide patterns for how the title should be structured, and on node
   pages, gives you the chance to specify the page title rather than defaulting
   to the node title.

********************************************************************
PERMISSIONS:

   This module defines the "set page title" and "administer page titles"
   permissions. The "set page title" permission determines whether a user will
   be able to edit the "Page title" field on node edit forms (if visible.) The
   "administer page titles" permission determines whether a user will be able to
   edit the "Page title" administration pages.

********************************************************************
INSTALLATION:

1. Place the entire page_title directory into your Drupal modules/
   directory or the sites modules directory (eg site/default/modules)


2. Enable this module by navigating to:

     Administration > Modules

   At this point the Drupal install system will attempt to create the database
   table page_title. You should see a message confirming success or
   proclaiming failure. If the database table creation did not succeed,
   you will need to manually add the following table definition to your
   database:

    CREATE TABLE `page_title` (
      `type` varchar(15) NOT NULL default 'node',
      `id` int(10) unsigned NOT NULL default '0',
      `page_title` varchar(255) NOT NULL default '',
      PRIMARY KEY  (`type`,`id`)
    );

3. Optionally configure the two variations of page title by visiting:

    Administration > Configuration > Search and metadata
