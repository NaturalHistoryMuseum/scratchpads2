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

The Taxonomy translation module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, provides multiple options to
translate taxonomy vocabularies and terms. For each vocabulary, there are four
types of behaviors to choose from: Language-independent terms, Language-specific
terms, Localized terms, and Mixed-language vocabulary.

* For a full description of the module visit:
  https://www.drupal.org/node/1114016

* To submit bug reports and feature suggestions, or to track changes visit:
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

Language-independent terms - only vocabulary will be translatable.
1. Navigate to Structure > Taxonomy.
2. Select the "edit vocabulary" link.
3. Select the "No multilingual options for terms".

Language-specific terms - vocabulary is only used for content in that language.
The terms will only be available if the term language matches the UI language.
1. Navigate to Structure > Taxonomy and select the "edit vocabulary link".
2. Choose "Fixed Language" and a Language drop-down field will be displayed.
3. Select the language.
4. Select "Fixed Language" and Save.

Localized terms - Terms are common for all languages, but their name and
description may be localized.
1. Navigate to Structure > Taxonomy > vocabulary-to-edit > Edit.
2. Select "Localize" and select Save.
3. Edit a term and there will be a Translate tab. Select this tab.
4. Select Translate, translate the Name and Description, select
   "Save translation", and repeat for all languages.
5. Repeat the process for all terms.
6. Navigate to Structure > Content types > term-to-edit > Manage display.
7. By default, the term reference is set to Link. Change this to "Link
   (localized)" and Save.
The vocabulary will be appropriate for the language.

Mixed-language vocabulary - Use for vocabularies with terms in multiple
languages.
1. Navigate to Structure > Taxonomy > vocabulary-to-edit > Edit.
2. Select the Translate radio button and Save.
3. Edit a vocabulary term and there will be a new Language field. Choose a
   language and then select Save and translate.
4. There are two options, the user can either select "Add translation link" or
   the user can select an existing term in the Select translations form.
5. Create translations for the terms and add terms for specific languages only.
Now if the user edits a node associated with this vocabulary, only the relevant
terms will appear.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
