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

Using additional preprocess functionality in your sub-theme
===========================================================
The power of the Omega theme lies in preprocess functionality. However, you
may have the need to add additional preprocess functions in your own sub
theme. There are two ways to accomplish this type of added functionality.

  1.) Adding template_preprocess_hook() functionality to your template.php
  
  Just like in any theme, you may use theme_preprocess hooks in order to 
  add additional functionality during preprocess. These functions can be 
  created normally the way you would in any theme using the following pattern
  
  function themename_preprocess_page($vars)
  function themename_preprocess_node($vars)
  function themename_preprocess_block($vars)
  etc...
  
  You will see many example functions in the template.php provided in this
  starterkit which are currently commented out. You may uncomment those 
  or create your own functions.
  
  2.) Adding appropriately named files in the preprocess directory
  
  Using the same model as the studio theme, the Omega theme is set to help
  keep an organization of massive amounts of preprocess functionality. It is
  very easy for a template.php file with thousands of lines of preprocess code
  to quickly get out of hand. 
  
  The Omega theme gives you an easy way to organize your preprocess functions 
  via a subdirectory called preprocess in your themes root directory. The 
  appropriate folder is now included in the starterkit, and does not need to be
  created manually. 
  
  To organize your preprocess functionality you would follow the following pattern:
  
  themename_preprocess_page($vars) becomes yourtheme/preprocess/preprocess-page.inc
  themename_preprocess_node($vars) becomes yourtheme/preprocess/preprocess-node.inc
  
  You may use this for ANY preprocess function that is available to Drupal via core, 
  and also preprocess hooks that are created by contributed modules. You may easily 
  create a file for ANY preprocess function by replacing all the underscores in a 
  preprocess function name with dashes, and removing the themename portion.

Proper coding inside of preprocess-hook.inc
===========================================

One item of confusion for some is how this actually is implemented, and how to 
properly code inside of the preprocess files inside of this custom directory.

During the core theme_preprocess($vars, $hook) function, which is called before any 
hook specific preprocess code/files, this function determines that there are files
available via the preprocess folder to include, and calls those files at the
appropriate time. 

When you are coding iside of a file like preprocess-page.inc, you will NOT declare
functions, and not need to include a function themename_preprocess_page() wrapper
because this file is INCLUDED during hook_preprocess_page. 

If you have the need to create custom functions, these should be done in template.php
or in a custom include file as normal. You may then call various custom functions in
your preprocess code. 

Additional 960gs/Omega Resources
================================
  * Please see the README.txt file in the root of the starterkit directory 
    for more information regarding the Omega base theme & subtheming

Contributors
============
- himerus (Jake Strawn)
