content_lock

-- SUMMARY --

The purpose of this module is to avoid the situation where two people
are editing a single node at the same time. On busy sites with dynamic
content, edit collisions are a problem and may frustrate editors with
an error stating that the node was already modified and can't be
updated. This module implements a pessimistic locking strategy, which
means that content will be exclusively locked whenever a user starts
editing it. The lock will be automatically released when the user
submits the form or navigates away from the edit page.

Content locks that have been "forgotten" can be automatically released
after a configurable time span using the bundled content_lock_timeout
submodule.

For a full description, visit the project page:
  http://drupal.org/project/content_lock
Bug reports, feature suggestions, and latest developments:
  http://drupal.org/project/issues/content_lock
For integrating modules with content_lock, see:
  http://drupalcontrib.org/api/drupal/contributions--content_lock--content_lock.api.inc

-- INSTALLATION --

1. Install as usual, see http://drupal.org/node/70151 for further information.

2. Configure user permissions at User management >> Permissions:

   check out documents - This enables content locking when a user starts
     editing it.  Note that even *without* this permission, users are still
     able to edit node contents and are *not* protected from concurrent
     editing.

   administer checked out documents - View and release locked contents
     of all users.  This enables the administrative tab on Content
     management >> Content. Note users can manage their own content
     locks on their profile page *without* this permission in their
     profile page. This is intended for administrators or moderators
     only.

3. Configure the module at Site Configuration >> Content lock.

    Use javascript to detect leaving the node form - Automatically unlock nodes
      when users navigate away from a node edit page by clicking miscellaneous
      links. Also prompts user when trying to navigate away from a node edit page.

    Show lock / unlock message - Make content_lock more verbose, informing a
      user when he locks a node and about his inconsideration when he visits
      one node while he has kept another node locked.

    Add cancel button - Adds a link in a node's edit form to cancel the edit,
      letting the user intentionally navigate away from the Edit page without
      being asked for confirmation by a javascript dialog.

    Lockable content types - You may choose to limit content_lock's effects
      to specific node types instead of across the board. Do not select any
      content types to ensure that all content types are protected.

    Lockable text formats - You may choose what type of node input formats
      may be lockable. Do not select any input types if content_lock should
      protect all input types.

4. If you want stale locks to time out, enable the content_lock_timeout
    ("Content Locking (edit lock) timeouts") module. Then return to the
    Content Lock settings page and configure the timeout.

5. View and administer locked nodes Content management >> Content >>
    Locked Documents.

-- CREDITS --
Current maintainers:
Eugen Mayer http://drupal.org/user/108406
Nathan Phillip Brink (ohnobinki) http://drupal.org/user/108406

This module is a fork/continuation of
http://drupal.org/project/checkout which was written and maintained
by:

Stefan M. Kudwien
Joël Guesclin
