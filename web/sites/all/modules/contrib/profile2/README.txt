
--------------------------------------------------------------------------------
                              Profile2
--------------------------------------------------------------------------------

Maintainers: 
 * Wolfgang Ziegler (fago), nuppla@zites.net
 * Joachim Noreiko (joachim), joachim.n+drupal@gmail.com


This modules is designed to be the successor of the core profile module. In
contrast to the core module this module provides a new, fieldable 'profile'
entity - leverage the power of fields!


Installation
-------------

 * Copy the whole profile2 directory to your modules directory and
   activate the module.


Usage
-----
   
 * Go to /admin/structure/profiles for managing profile types.
 * By default users may view their profile at /user and edit them at
   'user/X/edit'.



--------------------------------------------------------------------------------
                              Profile pages
--------------------------------------------------------------------------------
Maintainers: 
 * Wolfgang Ziegler (fago), nuppla@zites.net
 
This module provides an alternative way for your users to edit their profiles.
Instead of integrating with the user account page, it generates a separate page
allowing your users to view and edit their profile.


Installation
-------------

 * Once profile2 is installed, just active the profile pages module.


Usage
-----
 * The module may be enabled per profile-type by checking the checkbox
   "Provide a separate page for editing profiles." in the profile type's
   settings.
 * Users with sufficient permissions (check user permissions) receive a menu
   item in their user menu, just beside the "My account" menu item.
