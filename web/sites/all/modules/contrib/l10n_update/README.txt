
Localization Update
-------------------
  Automatically download and update your translations by fetching them from
  http://localize.drupal.org or any other Localization server.

  The l10n update module helps to keep the translation of your drupal core and
  contributed modules up to date with the central Drupal translation repository
  at http://localize.drupal.org. Alternatively locally stored translation files
  can be used as translation source too.

  By choice updates are performed automatically or manually. Locally altered
  translations can either be respected or ignored.

  The l10n update module is developed for:
   * Distributions which include their own translations in .po files.
   * Site admins who want to update the translation with each new module revision.
   * Site builders who want an easy tool to download translations for a site.
   * Multi-sites that share one translation source.

  Project page:  http://drupal.org/project/l10n_update
  Support queue: http://drupal.org/project/issues/l10n_update

Installation
------------
  Download, unpack the module the usual way.
  Enable this module and the Locale module (core).

  You need at least one language (besides the default English).
  On Administration > Configuration > Regional and language > Languages:
    Click "Add language".
    Select a language from the select list "Language name".
    Then click the "Add language" button.

  Drupal is now importing interface translations. This can take a few minutes.
  When it's finished, you'll get a confirmation with a summary of the
  translations that have been imported.

  If required, enable the new language as default language.
  Administration > Configuration > Regional and language > Languages:
    Select your new default language.

Update interface translations
-----------------------------
  You want to import translations regularly using cron. You can enable this
  on Administration > Configuration > Regional and language > Languages:
    Choose the "Translation updates" tab.
    Change "Check for updates" to "Daily" or "Weekly" instead of the default "Never".
  From now on cron will check for updated translations, and import them is required.

  The status of the translations is reported on the "Status report" page at 
  Administration > Reports.

  To check the translation status and execute updates manually, go to
    Administration > Configuration > Regional and language > Translate inteface
    Choose the "Update" tab.
  You see a list of all modules and their translation status.
  On the bottom of the page, you can manually update using "Update translations".

Use Drush
---------
  You can also use drush to update your translations:
    drush l10n-update           # Update translations.
    drush l10n-update-refresh   # Refresh available information.
    drush l10n-update-status    # Show translation status of available project


Summary of administrative pages
-------------------------------
  Translations status overview can be found at
    Administration > Configuration > Regional and language > Languages > Translation updates

  Update configuration settings can be found at
    Administration > Configuration > Regional and language > Translate interface > Update

Translating Drupal core, modules and themes
-------------------------------------------
  When Drupal core or contributed modules or themes get installed, Drupal core
  checks if .po translation files are present and updates the translations with
  the strings found in these files. After this, the localization update module
  checks the localization server for more recent translations, and updates
  the site translations if a more recent version was found.
  Note that the translations contained in the project packages may become
  obsolete in future releases.

  Changes to translations made locally using the site's build in translation
  interface (Administer > Site building > Translate interface > Search) and
  changes made using the localization client module are marked. Using the
  'Update mode' setting 'Edited translations are kept...', locally edited
  strings will not be overwritten by translation updates.
  NOTE: Only manual changes made AFTER installing Localization Update module
  are preserved. To preserve manual changes made prior to installation of
  Localization Update module, use the option 'All existing translations are kept...'.

po files, multi site and distributions
--------------------------------------
  Multi sites and other installations that share the file system can share
  downloaded translation files. The Localization Update module can save these
  translations to disk. Other installations can use these saved translations
  as their translation source.

  All installations that share the same translation files must be configured
  with the same 'Store downloaded files' file path e.g. 'sites/all/translations'.
  Set the 'Update source' of one installation to 'Local files and remote server'
  or 'Remote server only', all other installations are set to
  'Local files only' or 'Local files and remote server'.

  Translation files are saved with the following file name syntax:

    <module name>-<release>.<language code>.po

  For example:
    masquerade-6.x-1.5.de.po
    tac_lite-7.x-1.0-beta1.nl.po

  Po files included in distributions should match this syntax too.

Alternative sources of translation
----------------------------------

  Each project i.e. modules, themes, etc. can define alternative translation
  servers to retrieve the translation updates from.
  Include the following definition in the projects .info file:

    l10n server = example.com
    l10n url = http://example.com/files/translations/l10n_server.xml

  The download path pattern is normally defined in the above defined xml file.
  You may override this path by adding a third definition in the .info file:

    l10n path = http://example.com/files/translations/%core/%project/%project-%release.%language.po

API
---
  Using hook_l10n_servers the l10n update module can be extended to use other
  translation repositories. Which is usefull for organisations who maintain
  their own translation.

  Using hook_l10n_update_projects_alter modules can alter or specify the
  translation repositories on a per module basis.

  See l10n_update.api.php for more information.

Maintainers
-----------
  Jose Reyero
  Gábor Hojtsy
  Erik Stielstra
