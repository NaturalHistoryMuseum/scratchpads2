

INTRODUCTION
============

Linkit provides an easy interface for internal linking. Linkit links to
nodes, users, files and terms by default, using an autocomplete field.
Linkit has two major advantages over traditional linking

 1. The user does not have to copy or remember a URL
 2. If the target node changes it's alias (e.g. if the node's menu item
    title is changed) the link will remain functional.

See http://drupal.org/project/linkit for more information


INSTALLATION
============

 1. Install and configure the dependencies. Check their README files for
    details.
 2. Install and enable the linkit module.
 3 a). Enable the Linkit button in your CKEditor profile settings.
 3 b). Enable the Linkit button in your WYSIWYG editor's settings.


DEPENDENCIES
============

Linkit depends on the module

 *  ctools (For the profile functionality)

You also need to install one of these modules:

 *  CKEditor (Standalone) <http://drupal.org/project/ckeditor>
 *  WYSIWYG <http://drupal.org/project/wysiwyg> with TinyMCE or CKEditor


CONFIGURATION
=============

See PROFILES


PROFILES
========

Linkit has profiles, which means that settings are bundled with a profile
By default, we provied you with a default profile, with default settings.

You can clone that profile and make your own with the settings your satisfied
with.

You can add as many profiles you like, and the profiles are exportable, also
via Features <http://drupal.org/project/features>.

Profiles are fund at <http://example.com/admin/config/content/linkit> or
Configuration -> Content authoring -> Linkit Profiles.

PATHOLOGIC
==========

If you are using Pathologic you need to enable the Pathologic input filter
(Correct URLs with Pathologic) on the text formats you intend to use Linkit on.
You also need to make "/" considered local.


API
===

We have an API example file (linkit.api.php) located in the Linkit module root.
There is also an example module <https://github.com/bratanon/Mymodule>.


MAINTAINERS
===========

 *  anon <http://drupal.org/user/464598>
 *  betamos <http://drupal.org/user/442208>
