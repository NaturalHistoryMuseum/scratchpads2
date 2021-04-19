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

The Multilingual content module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, provides extended multilingual
options for nodes. These options help accommodate a variety of translation
workflows by controlling how the language for nodes is set.

Note that the Multilingual content module lives in the i18n_node directory
in the Internationalization package. Don't confuse this module with the core
Content translation module.

* For a full description of the module visit:
  https://www.drupal.org/node/1279644.

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

For each content type, the following Extended language options are available
under the Multilingual Settings tab.
1. "Set current language as default for new content" can be useful for content
   that is community-generated.
2. "Require language" prevents users from creating 'Language neutral' nodes.
3. "Lock language" prevents users from changing the language of a node after
   it's created.

Site-wide Settings for Node Translation
1. There are also site-wide settings provided to help streamline how
   multilingual content is created. Navigate to Config > Regional and language >
   Multilingual settings > Node Options.
2. "Switch interface for translating" switches the language of the user
   interface to the chosen language when a user translates a node. This is
   useful if users speak the language in which the translation is written. It
   means that after the node translation is saved, the language of the UI will
   match the language of the node.
3. "Hide content translation links" will prevent the language switcher links
   from appearing in nodes and teasers. This is useful if a language switcher
   block is enabled on the site.
4. The user can also select the default language for new nodes if the
   corresponding content type doesn't have language support. By default, it is
   set to be in the default language of the site, but this can be changed to be
   language neutral. This is a useful option when thinking about forward
   compatibility for adding multilingual support to these content types in the
   future.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
