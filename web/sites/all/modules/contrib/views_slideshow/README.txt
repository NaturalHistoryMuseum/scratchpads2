
Views Slideshow
===============

The Views Slideshow module is a Views Style Plugin that can be used to output
Views in a jQuery slideshow.

This module contains an api module "Views Slideshow" and one implementation of
that api in "Views Slideshow Cycle".

Installation
=============

1) Assure that you are using Views 3.x. http://drupal.org/project/views
2) Upload/install the Libraries API module. http://drupal.org/project/libraries
3) Upload/install the ctools library. http://drupal.org/project/ctools
4)If enabling via drush, the libraries will be automatically downloaded. Otherwise, you will need to manually download the library.
  a) Create a sites/all/libraries directory on your server.
  b) Create a directory within sites/all/libraries named jquery.cycle.
  c) Locate/download the jQuery cycle plugin. http://malsup.com/jquery/cycle
  d) Upload/install the jQuery cycle plugin: place it inside the jquery.cycle
   directory.
5) There are also a few optional libraries to improve/increase options:
  a) jQuery.easing:
    http://gsgd.co.uk/sandbox/jquery/easing/jquery.easing.1.3.js, should be
    saved as libraries/jquery.easing/jquery.easing.1.3.js.
6) Enable the Views slideshow and Views Slideshow: Cycle modules
7) You should now see the new views style option called "Slideshow"

Upgrading from 2.x
===================

There is no supported upgrade path between Views slideshow 2.x and Views
slideshow 3.x. If you need to upgrade, follow the steps listed here, and if you
find deficiencies, please post an issue.

1) Assure that you are using Views 3.x
2) Disable the views slideshow module, views slideshow thumbnail hover module,
   and views slideshow singleframe modules. (Disabling these modules should not
   require uninstalling)
3) Delete/remove the Views slideshow 2.x module folder and all files within.
4) Upload/install the Views slideshow 3.x module folder and all files within.
5) Upload/install the ctools module. http://drupal.org/project/ctools
6) Upload/install the Libraries API module. http://drupal.org/project/libraries
7) Create a sites/all/libraries directory on your server.
8) Create a directory within sites/all/libraries named jquery.cycle
9) Locate/download the jQuery cycle plugin from github: https://raw.githubusercontent.com/malsup/cycle/3.0.3/jquery.cycle.all.js
10) Upload/install the jQuery cycle plugin: place it inside the jquery.cycle
   directory.
11) Enable the Views slideshow and Views Slideshow: Cycle modules
12) Update all your views that were previously using the slideshow style.
    (note: updating from views 2.x to views 3.x may also break other parts of
    your view not related to slideshows, check everything especially number of
    items per page, and pager settings.)


Requirements
============

* Views 3
* Libraries
* Ctools

Views Slideshow Cycle
* jQuery Cycle library


Description
===========

This module will create a View type of Slideshow that will display nodes in a
jQuery slideshow.

Settings are available for fade, timing, mode, and more.

Known Issues
============

* Do not use the block cache for Views Slideshow blocks as this may prevent the
JavaScript from being added to the page. See https://www.drupal.org/node/1460766
and https://www.drupal.org/node/2448157.

Authors/maintainers
===================

Original Author:

Aaron Winborn (winborn at advomatic dot com)
https://drupal.org/user/33420

-maintainers:

NickWilde
https://www.drupal.org/u/nickwilde

redndahead
https://drupal.org/user/160320

psynaptic
https://drupal.org/user/93429


Support
=======

Issues should be posted in the issue queue on drupal.org:

https://drupal.org/project/issues/views_slideshow
