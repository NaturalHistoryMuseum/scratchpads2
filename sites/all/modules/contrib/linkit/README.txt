

INTRODUCTION
============

Linkit provides an easy interface for internal linking. Linkit links to
nodes, users, files and terms by default, using an autocomplete field.
Linkit has two major advantages over traditional linking

 1. The user does not have to copy or remember a URL
 2. If the target node changes it's alias (e.g. if the node's menu item
    title is changed) the link will remain functional (thanks to Pathologic)

See http://drupal.org/project/linkit for more information


INSTALLATION
============

 1. Install and configure the dependencies. Check their README files for details.
    You need to enable the Pathologic input filter on the text formats you intend to use Linkit on.
 2. Install and enable the linkit module and at least one of linkit_node,
    linkit_views, linkit_taxonomy or linkit_user.
 3. Enable the Linkit button in your WYSIWYG editor's settings.

If you are using standalone CKEditor, see editors/ckeditor/README.txt for additional install information.


DEPENDENCIES
============

Linkit depends on the module

 *  Pathologic (this is where the alias magic happens)

You also need to install one of these modules:

 *  WYSIWYG <http://drupal.org/project/wysiwyg> with TinyMCE, CKEditor or FCKeditor
 *  CKEditor (Standalone) <http://drupal.org/project/ckeditor>


CONFIGURATION
=============

No additional configuration is necessary but you may can find optional Linkit's settings at
Configuration → Linkit settings (/admin/config/content/linkit). You need "administer linkit" permission.


MAINTAINERS
===========

 *  anon <http://drupal.org/user/464598>
 *  betamos <http://drupal.org/user/442208>
