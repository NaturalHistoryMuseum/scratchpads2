This module is intended for site administrators and developers. The module
provides a great way to lock down or freeze a production server so that
maintenance or large deployments can occur without taking the site offline.
In a typical example, Read Only Mode is activated on the production server,
the database is copied to a development server where work is done, and then the
database is pushed back to the production server and Read Only Mode disabled.

Usage and configuration
================================================================================
The module can be configured in the Site Maintenance settings page alongside
the built in maintenance mode settings. Simply choose to enable the mode and
your site is now locked down to content additions. Read Only Mode and
Maintenance mode are separate and you do not need to enable Maintenance Mode to
enable Read Only Mode.

Further configuration is possible to provide custom warnings and errors to users
and to whitelist certain forms that your site deems 'safe' during Read Only
Mode. For example, a form may submit content through email or by accessing a
callback on a web service and no data is stored in the Drupal database. It may
make sense to allow site users to access these forms even when the site is in
Read Only Mode.

In addition to a whitelist, the site also provides a permission that overrides
access restrictions and can be given to site administrators.

Drush Support
================================================================================
Since Read Only Mode is defined by a single variable, it is easy to use Drush
to enable and disable the mode.

Check the status:         drush vget site_readonly
Turn on Read Only Mode:   drush vset site_readonly 1
Turn off Read Only Mode:  drush vset site_readonly 0
