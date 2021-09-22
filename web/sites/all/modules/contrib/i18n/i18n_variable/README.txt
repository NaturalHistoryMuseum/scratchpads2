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

The Variable translation module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, allows the user to translate text
and settings that are stored in Drupal as variables. These variables include
text such as 'site name' and 'site slogan', as well as settings like 'Default
front page' and 'Default 404 page'.

* For a full description of the module, visit:
  https://www.drupal.org/node/1113374.

* To submit bug reports and feature suggestions, or to track changes visit:
  https://www.drupal.org/project/issues/i18n.


REQUIREMENTS
------------

This module requires the following modules:

* Internationalization  - https://www.drupal.org/project/i18n
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

To enable multilingual variables

1. Enable the Variable translation module included with the Internationalization
   package.
2. Go to Administration > Configuration > Regional and language > Multilingual
   settings.
3. Select on the Variables tab.
4. Select the variables that will be multilingual.
5. Save configuration.

Once the user has the correct settings, they'll be marked with "This is a
multilingual variable" when the user navigates to the corresponding
administration pages. The user must switch the site language while in the
administration pages to set the variables for each language. A language switcher
link will appear at the top of each administrative page that has multilingual
variables.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
