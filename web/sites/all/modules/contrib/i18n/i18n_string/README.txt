CONTENTS OF THIS FILE
---------------------
  
* Introduction
* Requirements
* Recommended modules
* Installation
* Configuration
* Maintainers


INTRODUCTION
------------

The String translation module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, provides support for other
modules to translate user-defined strings. This is an API module that must be
enabled only when required by other modules in the i18n package.


* For a full description of the module, visit this page:
  https://www.drupal.org/node/1279668

* To submit bug reports and feature suggestions, or to track changes:
  https://www.drupal.org/project/issues/i18n


REQUIREMENTS
------------

This module requires the following module:

* Internationalization - https://www.drupal.org/project/i18n


RECOMMENDED MODULES
-------------------

* Internationalization Views - https://www.drupal.org/project/i18nviews
* Language Icons - https://www.drupal.org/project/languageicons
* Translation Overview - https://www.drupal.org/project/translation_overview
* Localization Client - https://www.drupal.org/project/l10n_client
* Internationalization contributions -
  https://www.drupal.org/project/i18n_contrib


INSTALLATION
------------

This is a submodule of the Internationalization module. Install the
Internationalization module as you would normally install a contributed Drupal
module. Visit https://www.drupal.org/node/895232 for further information.


CONFIGURATION
-------------

Strings will be translated from the source languages. By default the source
language is the site's default language, so changing the default language could
break these translations.

1. The user can set which language is used as the source language via
   Administration > Configuration > Regional and language > Multilingual
   settings > Strings. By default, only plain strings are enabled, so regular
   blocks are not fully translatable.
2. To allow Filtered HTML, Full HTML or Plain text select the appropriate radio
   box(es) and Save configuration.
3. To select the strings to be translated navigate to Administration >
   Configuration > Regional and language > Translate interface and select on
   Stings vertical tab. From here the user can select which text groups to
   translate and select the Refresh strings tab.


FAQ
---

The String translation module allows you to configure which text formats are
translatable. Formats like PHP Filter and Full HTML are translated before they
are processed, so allowing a translator to edit these can be a security risk.
This is particularly problematic when importing translations in bulk from a CSV
file, since the translator's access to the import formats isn't verified by
Drupal. After updating this setting, be sure to refresh the strings via
Administration > Configuration > Regional and language > Translate interface >
Strings so that strings in forbidden formats are deleted.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
