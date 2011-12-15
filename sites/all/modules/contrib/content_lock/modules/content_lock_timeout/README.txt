-- SUMMARY --

This module extends content_lock to automatically break stale
locks. It has a configurable stale lock timeout and will break locks
through cron. It can also check for a stale lock and break it when
another user tries to edit a node.

-- REQUIREMENTS --

Working cron, if you wish to make use of automated check-ins.  If your
hoster doesn't allow you to access cron take a look at Poormanscron
(http://drupal.org/project/poormanscron) or enable the ``Break stale
locks on edit'' option to break stale locks as needed.

-- INSTALLTION --

1. This module is shipped with the content_lock module. After
   installing that module, just enable this one from drupal's
   administration pages.

2. Configure content_lock at Content management >> Post settings. This
   module respects the following general content_lock module settings:

   Show lock / unlock message - When set, users are informed when they
     break locks. They will also be informed how long their lock will
     be considered valid when they lock nodes.

3. Configure the lock timeouts at Content management >> Post settings
   in the ``Lock Timeouts'' fieldset.

   Lock timeout - This sets the number of minutes that a lock may be
     held before it is considered stale. Stale locks will only be
     broken when cron runs.  After a stale lock is cleaned by cron or
     by the below option, other users may lock the node associated
     with the lock.

   Break stale locks on edit - This option allows enables stale lock
     cleanup for a locked node when another user tries to edit that
     node. This is useful when you need short timeouts. Since cron is
     not meant to be run every few seconds, the cron based stale lock
     cleanup might take an hour to clean up a stale lock even if the
     stale lock timeout is set low, such as 20 minutes.
