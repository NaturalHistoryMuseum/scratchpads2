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

The Menu translation module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, allows users to select a
translation mode for each menu.

* For a full description of the module, visit this page:
  https://www.drupal.org/node/1113982.

* To submit bug reports and feature suggestions, or to track changes:
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

To link menu item menus to nodes, it is useful to have the following modules:

* Entity translation i18n menu module, a submodule of Entity translation -
  https://www.drupal.org/project/entity_translation
* Menu translation node module - https://www.drupal.org/project/i18n_menu_node


INSTALLATION
------------

This is a submodule of the Internationalization module. Install the
Internationalization module as you would normally install a contributed Drupal
module. Visit https://www.drupal.org/node/895232 for further information.


CONFIGURATION
-------------

Language-specific menus
1. To create or edit a menu, navigate to Structure > Menus > (menu to edit) >
   Edit.
2. In the Translation mode section, choose Fixed Language and a Language field
   will appear.
3. Select a language, select Save, and add or update the menu items as needed.
The menu block will only appear when viewing content in the same language.

There are three modes available: Translate and Localize, Fixed Language, and No
Multilingual Options.

Translate and Localize:
The user can create one menu for all languages, and translate or localize each
menu item. There are two ways that menu items will be translated.
1. The user can set a language when creating a custom menu item so that the menu
   item will only show up for that language. Menu items that link to nodes in a
   particular language will be treated this way.
2. The user can localize other custom menu items without a language
   (for example, menu items linking to Views pages). Use the Translate tab to
   translate the menu item title and description. Translators can also use the
   "Translate interface" pages to translate these menu items.

Fixed Language:
If the user chooses Fixed Language, they'll have to set up a separate menu in
each language. This could become tedious if have a lot of languages enabled on
the site, but is useful if the content or menu structure is different for each
language.

No Multilingual Options:
Only the menu will be translatable.

TROUBLESHOOTING
---------------

A menu item linked to a node will be displayed only when the node language
matches the page language. This is due to how the menu system works and the
"Language selection" feature in i18n. Therefore, to get translated menus items
that link to nodes, you first need translated content. For more information
visit https://www.drupal.org/docs/7/multilingual/translating-content.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
