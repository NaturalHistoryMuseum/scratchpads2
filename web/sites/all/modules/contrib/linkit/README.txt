

INTRODUCTION
============

Linkit provides an easy interface for internal linking. Linkit links to
nodes, users, managed files and terms by default, using an autocomplete field.
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
 *  entity API
 *  file_entity (2.x for file support)

You also need to install one of these modules:

 *  CKEditor (Standalone) <http://drupal.org/project/ckeditor>
 *  WYSIWYG <http://drupal.org/project/wysiwyg> with TinyMCE or CKEditor


CONFIGURATION
=============

See PROFILES


PROFILES
========

A profile defines Linkit's scope. By defining which roles have access to a
profile we are able to provide sets of linkit capabilities to various roles.
For example one profile for a admin could include search for nodes,
Taxonomy tern and users, while a profile for an editor could include only
nodes.

You can add as many profiles you like, and the profiles are exportable, also
via Features <http://drupal.org/project/features>.

Profiles are fund at <http://example.com/admin/config/content/linkit> or
Configuration -> Content authoring -> Linkit Profiles.


CREATE A LINKIT PROFILE
=======================

1. Go to /admin/modules and click on its Configure link.
2. You’ll be now on page /admin/config/content/linkit.
3. Click on the ADD NEW PROFILE tab.
4. For example lets create a profile for admins:
  - In field set ROLES WITH ACCESS click the administrator checkbox.
  - In field set LINKIT PLUGINS click the content that should be searched by 
    Linkit, e.g. click Node if Linkit should present page names in its
    auto-complete search field.
  - In field set NODE PLUGIN SETTINGS check all node types that should included
    in the search.
  - In field set HTML ATTRIBUTES check all the link attributes you need Linkit
    to include when creating the links.
  - Ignore the AUTOCOMPLETE OPTIONS, its setting will cover most use cases. You
    can always come back and fine-tune these options.
5. Click Save

Now that we have configured the Linkit module we need to tell CKEditor that it
should use Linkit.


USING LINKIT WITH THE CKEDITOR MODULE
=====================================

1. Go to /admin/config/content/ckeditor.
2. You will see a list of CKEditor profiles.
3. Click on the Edit link of the profile that you want to use Linkit with.
4. Open the field set EDITOR APPEARANCE.
5. Find the checkbox grouping Plugins.
6. Select the checkbox Support for Linkit module.
7. Find section Toolbar. This should be right above the Plugins section. Here
   you can configure the CKEditor toolbar via drag-and-drop.
8. Under All buttons find the Linkit link button. This button features a
   horizontal chainlink with a green plus sign next to it. (Hover over it and
   the tooltip will say Linkit).
9. Drag this button to section Used Buttons.
10. Click Save.

You are done. Go to any content item of a type that was enabled in the Linkit
profile and you’ll see now the linkit button in the CKEditor toolbar.


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
