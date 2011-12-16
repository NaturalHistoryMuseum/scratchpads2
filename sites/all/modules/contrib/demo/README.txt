/* $Id: README.txt,v 1.7 2009/11/09 18:21:28 sun Exp $ */

-- SUMMARY --

Demo Site is a simple module that allows you to take a snapshot of a Drupal
demonstration site. It turns a Drupal installation into a sandbox that you can
use for testing modules or publicly demonstrating a module / extension / theme
(you name it). In short: With cron enabled, the Drupal site will be reset to
the dumped state in a definable interval. Of course you can reset the site
manually, too.

For a full description visit the project page:
  http://drupal.org/project/demo
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/demo


-- REQUIREMENTS --

* Cron.


-- INSTALLATION --

* Copy the Demo module to your modules directory and enable it on the Modules
  page (admin/structure/modules).

* Optionally configure who is allowed to administer Demo module, create dumps
  and reset the site on the Permissions page (admin/config/people/permissions).


-- CONFIGURATION --

* Configure the path where dumps will be stored at the Dump settings
  (admin/structure/demo).

To configure automatic reset:

* Go to Manage snapshots (admin/structure/demo/manage) and select a snapshot
  for cron.

* Enable atomatic reset from Dump settings (admin/structure/demo). Make sure you
  have cron configured to run at least once within the entered time interval.


-- USAGE --

* Go to Create snapshot (admin/structure/demo/dump) and create your first
  snapshot.

* After a while, reset your site (admin/structure/demo/reset).


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - dev@unleashedmind.com
* Stefan M. Kudwien (smk-ka) - dev@unleashedmind.com

This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

