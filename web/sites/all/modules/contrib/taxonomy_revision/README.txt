Taxonomy revision module
========================

DESCRIPTION
------------

This module enables revisions for the taxonomy terms.

The users will have an option to create new revision for a taxonomy term as for
nodes. This means when a taxonomy term is edited in the default form, a new
checkbox is displayed for revision creation. If clicked a textarea will appear
to fill a revision log message.

Also on the taxonomy term edit page a new menu tab appears after the module
installation which will list all of the taxonomy term's revisions and provides
2 operations for each taxonomy term:
 * revert: which will revert the revision.
 * delete: which will delete the revision.

Also the module is adding some field handlers for taxonomy term revisions to
support the taxonomy term revisions in views.

REQUIREMENTS
------------
Drupal 7.x

INSTALLATION
------------
1.  Place Taxonomy revision module into your modules directory.
    This is normally the "sites/all/modules" directory.

2.  Go to admin/build/modules. Enable the module.

Read more about installing modules at http://drupal.org/node/70151

UPGRADING
---------
Any updates should be automatic.
