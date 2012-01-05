##########################################################
##### Omega Theme
##########################################################
Project Page:   http://drupal.org/project/omega
Issue Queue:    http://drupal.org/project/issues/omega
Usage Stats:    http://drupal.org/project/usage/omega
Demo Page:      http://omega.himerus.com
Maintainer(s):  Jake Strawn 
                http://himerus.com
                http://twitter.com/himerus
##########################################################

Omega Theme Information
=======================
The Omega Theme is a powerful and free Drupal theme based on the 960gs. 
It harneses the power and features of many popular themes to provide an 
excellent base theme, and sub-theming system to help you quickly prototype 
and theme your site...

Organizing JS files in your theme
==================================
This js folder is provided for easy organization of your JS files in your
subtheme. Many times for organization, it's best to use many JS files to 
keep them easier to understand and find things rather than using a single
javascript file that can quickly become thousands of lines long.

Including new JS files
======================
To include a new JS file in your theme, you will need to edit the .info
file that defines your theme.

In that .info file you will see the following section:

  ; ------- Declare default javascript includes

  ;scripts[] = js/your_js_file.js

By default, there are several JS files included from the Omega base theme
but to add more of your own, you would use this pattern. (Notice the semi-
colon at the beginning of the stylesheet line) Remove the semi-colon in 
order to have Drupal read this line. You will also want to change the 
your_js_file portion of the file name to whatever you are trying to include.
Beyond that, make sure your new JS file is in place in the js folder of 
your sub-theme, and after saving your .info file, visit /admin/build/themes
in order for Drupal to pick up the appropriate changes.

Additional 960gs/Omega Resources
================================
  * Please see the README.txt file in the root of the starterkit directory 
    for more information regarding the Omega base theme & subtheming

Contributors
============
- himerus (Jake Strawn)
