CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Recommended modules
* Installation
* Configuration
* Troubleshooting
* Maintainers


INTRODUCTION
------------

The Block languages module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, allows the user to configure
for which languages each block is visible.

* For a full description of the module,
  visit https://www.drupal.org/node/1279698.

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

The settings for visibility per language are provided under Visibility
Settings via the Languages tab when configuring a block.

The Languages tab also provides a setting for whether the block is translatable.
For custom blocks, the block title and the block content will be translatable.
For blocks defined by modules, only the block title will be translatable. If
"Make this block translatable" is selected, a Translate tab will appear for that
block. This tab provides a UI for adding translations of the block in each
available language.


TROUBLESHOOTING
---------------

Conflicts with Context

The Block languages module conflicts with the Context module, which alters how
blocks are rendered. This issue can be tracked in the Internationalization
issue queue: http://drupal.org/node/1343044

String Errors

The user must allow your used string format to be translated on
admin/config/regional/i18n/strings or you are going to have an error message
like "The string blocks:block:1:body for textgroup blocks is not allowed for
translation because of its text format."


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
