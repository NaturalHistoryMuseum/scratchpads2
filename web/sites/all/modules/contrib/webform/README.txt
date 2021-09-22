Description
-----------
This module adds a webform content type to your Drupal site.
A webform can be a questionnaire, contact or request form. These can be used
by visitor to make contact or to enable a more complex survey than polls
provide. Submissions from a webform are saved in a database table and
can optionally be mailed to e-mail addresses upon submission.

Requirements
------------
Drupal 7.x

Installation
------------
1. Copy the entire webform directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. (Optional) Edit the settings under "Administer" -> "Configuration" ->
   "Content authoring" -> "Webform settings"

4. Create a webform node at node/add/webform.

Upgrading from previous versions
--------------------------------
Note that you must be running the latest 3.x version of Webform (for either
Drupal 6 or Drupal 7) before upgrading to Webform 4.x.

If you have contributed modules, custom modules, or theming on your Webforms,
please read over the documentation for upgrading your code for Webform 4.x at
https://drupal.org/node/1609324.

1. MAKE A DATABASE BACKUP. Upgrading to Webform 4.x makes a signficant number of
   database changes. If you encounter an error and need to downgrade, you must
   restore the previous database. You can make a database backup with your
   hosting provider, using the Backup and Migrate module, or from the command
   line.

2. Copy the entire webform directory the Drupal modules directory, replacing the
   old copy of Webform. DO NOT KEEP THE OLD COPY in the same directory or
   anywhere Drupal could possibily find it. Delete it from the server.

3. Login as an administrative user or change the $update_free_access in
   update.php to TRUE.

4. Run update.php (at http://www.example.com/update.php).

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/webform

