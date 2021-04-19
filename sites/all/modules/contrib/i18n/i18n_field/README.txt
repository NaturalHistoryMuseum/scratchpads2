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

The Field translation module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, allows for translation of text
associated with a field's settings including the label, help text, default
value, and list options.

* For a full description of the module, visit this page
  https://www.drupal.org/node/1279346.

* To submit bug reports and feature suggestions, or to track changes visit
  https://www.drupal.org/project/issues/i18n.


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

1. Enable the Field translation module included with Internationalization.
2. Go to Administration > Configuration > Regional and language > Translate
   interface.
3. Select the Filter Translatable Strings field set and limit search to fields.
4. Edit the desired field and Save.

For the translation to be displayed, the user needs to use some of the Field
Formatters provided by this module whose name usually ends up in 'translated'.
For most core fields it is Default translated.

Note: The Field Translation module does not provide content translation for
fields. This functionality is provided by the Entity Translation (ET) module.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
