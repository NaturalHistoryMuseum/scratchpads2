FEEDS SQL
=========

Adds SQL options to the FEEDS import and aggregation framework for Drupal.
http://drupal.org/project/feeds_sql

Features
========

- Adds SQL options to the Feeds module UI, to enable importing data
  from external databases. 
- Imports nodes, taxonomy, and users.


Requirements
============

- Feeds
  http://drupal.org/project/feeds
- Drupal 7.x
  http://drupal.org/project/drupal

Installation and Quick start
============================

- Install Feeds SQL and enable it
- Add custom database to settings.php
- In Admin interface -> Goto Feeds IU (admin/structure/feeds)
- Click tab "New importer"
- Fill out as needed, click "create"
- Configuration:
  - Basic -> Setting:
             Choose appropriate in "attach to content type"
             ("standalone form" might be the best option)
             Choose appropriate for cron settings
  - Fetcher -> Change: Choose "SQL Fetcher", click save.
    Fetcher -> Settings: Choose your source database(s) (the one(s) you added to
                         settings.php earlier), click save.
  - Parser -> Change: Choose "SQL Parser", click save.
    Parser -> Settings: Use this area to pre-fetch your intended query. This
              will allow you to test the query itself and to populate the
              mapping array. You will see the results of the query after saving.

  Now configure the Processor settings, e.g. for "node", choose the appropriate 
  content type, and map the fields appropriately at Processor -> Mapping.

  Next, you can go to "http://example.com/import", the new importer should be available.
  Enter your final query, choose your database, and click "import".
