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

The Multilingual select module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, allows the user to define whether
content is filtered by language on pages provided by Drupal core.

* For a full description of the module visit:
  https://www.drupal.org/node/1279512.

* To submit bug reports and feature suggestions, or to track changes visit
  https://www.drupal.org/project/issues/i18n.


REQUIREMENTS
------------

This module requires the following module:

* Internationalization - https://www.drupal.org/project/i18n
* Variable - https://www.drupal.org/project/variable


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

The module allows the user to configure whether or not to filter pages by the
current language. If the Taxonomy translation submodule is also enabled, an
option will be available for how taxonomy term pages are filtered. It also
allows the exclusion of the module’s language selection handling for some
elements and certain paths.

1. Enable the Multilingual select module included with Internationalization.
2. Navigate to Configuration > Regional and language > Multilingual settings >
   Selection.
3. Select nodes and taxonomy can be filtered by language in the Content to
   Filter by Language field set.
4. The "Content Selection Mode" allows content with specific tags to be skipped
   by entering a list of tags.
5. The "Enable for Specific Pages" field set allows specific pages to be
   included by path.
6. After making choices, Save configuration.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
