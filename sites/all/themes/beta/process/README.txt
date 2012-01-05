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

Using additional process functionality in your sub-theme
===========================================================
The power of the Omega theme lies in process functionality. However, you
may have the need to add additional process functions in your own sub
theme. There are two ways to accomplish this type of added functionality.

  1.) Adding template_process_hook() functionality to your template.php
  
  Just like in any theme, you may use theme_process hooks in order to 
  add additional functionality during process. These functions can be 
  created normally the way you would in any theme using the following pattern
  
  function themename_process_page($vars)
  function themename_process_node($vars)
  function themename_process_block($vars)
  etc...
  
  You will see many example functions in the template.php provided in this
  starterkit which are currently commented out. You may uncomment those 
  or create your own functions.
  
  2.) Adding appropriately named files in the process directory
  
  Using the same model as the studio theme, the Omega theme is set to help
  keep an organization of massive amounts of process functionality. It is
  very easy for a template.php file with thousands of lines of process code
  to quickly get out of hand. 
  
  The Omega theme gives you an easy way to organize your process functions 
  via a subdirectory called process in your themes root directory. The 
  appropriate folder is now included in the starterkit, and does not need to be
  created manually. 
  
  To organize your process functionality you would follow the following pattern:
  
  themename_process_page($vars) becomes yourtheme/process/process-page.inc
  themename_process_node($vars) becomes yourtheme/process/process-node.inc
  
  You may use this for ANY process function that is available to Drupal via core, 
  and also process hooks that are created by contributed modules. You may easily 
  create a file for ANY process function by replacing all the underscores in a 
  process function name with dashes, and removing the themename portion.

Proper coding inside of process-hook.inc
===========================================

One item of confusion for some is how this actually is implemented, and how to 
properly code inside of the process files inside of this custom directory.

During the core theme_process($vars, $hook) function, which is called before any 
hook specific process code/files, this function determines that there are files
available via the process folder to include, and calls those files at the
appropriate time. 

When you are coding iside of a file like process-page.inc, you will NOT declare
functions, and not need to include a function themename_process_page() wrapper
because this file is INCLUDED during hook_process_page. 

If you have the need to create custom functions, these should be done in template.php
or in a custom include file as normal. You may then call various custom functions in
your process code. 

Additional 960gs/Omega Resources
================================
  * Please see the README.txt file in the root of the starterkit directory 
    for more information regarding the Omega base theme & subtheming

Contributors
============
- himerus (Jake Strawn)
