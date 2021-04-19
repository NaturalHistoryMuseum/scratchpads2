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

The Redirect translation module, part of the Internationalization
(https://www.drupal.org/project/i18n) package, improves search engine
optimization (SEO) for multilingual websites.

It redirects anonymous users (including web crawlers) to the translation of the
page in the requested language, if it exists, using a 301 redirect code.

* For a full description of the module, visit this page:
  https://www.drupal.org/node/1280468.

* To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/i18n.


REQUIREMENTS
------------

This module requires the following module:

* Internationalization - https://www.drupal.org/project/i18n

The Translation redirect module requires the implementation of
hook_i18n_translate_path by another module for the redirect page to be
determined. Currently, the Multilingual content, Path translation, and Taxonomy
translation modules implement this hook.


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

No configuration is necessary.

For node translation, enable the Multilingual content and the Translation
redirect modules. For non-node pages, the redirection hook must be implemented
by the relevant module.
For example, for taxonomy pages, you should enable the Taxonomy translation
module because the module provides the necessary hook code. If you are using the
Path translation module to create translation sets for non-node pages, then it
implements the hook code for determining the redirection page.


MAINTAINERS
-----------

* Jose Reyero - https://www.drupal.org/u/jose-reyero
* Florian Weber (webflo) - https://www.drupal.org/u/webflo
* Peter Philipp - https://www.drupal.org/u/das-peter
* Joseph Olstad - https://www.drupal.org/u/joseph.olstad
* Nathaniel Catchpole - https://www.drupal.org/u/catch
