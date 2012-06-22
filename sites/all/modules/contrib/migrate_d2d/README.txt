Drupal-to-Drupal migration
==========================

This is a framework based on the Migrate API to ease building migrations
from one Drupal site to another. It is only supported at this time on Drupal 7
(i.e., Drupal 7 is the only destination). Only Drupal 5 and 6 are semi-fully-supported
as sources at this time, although some Drupal 7 support is sketched out - it
should be fairly straight-forward to flesh out the Drupal 7 support. Besides
addressing contemporary needs to migrate to Drupal 7, it is intended to help
serve as a proof-of-concept for incorporating the migration approach into core
as an upgrade path (http://drupal.org/node/1052692).

migrate_d2d
===========

The core framework provided here is used by providing your own module, which
will register instances of the migrate_d2d classes (or derivations of them).
See migrate_d2d_example for one approach, where instances are registered when
the Drupal caches are cleared (note that registration updates previously-
registered classes with any argument changes).

migrate_d2d_auto
================

Not yet implemented, the idea is to implement a fully-automated D?=>D7
migration module, which would move metadata (i.e., define the same vocabularies,
content types, and fields in D7 as were in the source site) and automatically
map the content itself using the same field names etc. as the source.
