
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
4) Create a sites/all/libraries directory on your server.
5) Create a directory within sites/all/libraries named jquery.cycle.
6) Locate/download the jQuery cycle plugin. http://malsup.com/jquery/cycle
7) Upload/install the jQuery cycle plugin: place it inside the jquery.cycle
   directory.
8) Enable the Views slideshow and Views Slideshow: Cycle modules
9) You should now see the new views style option called "Slideshow"

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
9) Locate/download the jQuery cycle plugin from the internet.
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


Authors/maintainers
===================

Original Author:

Aaron Winborn (winborn at advomatic dot com)
http://drupal.org/user/33420

Co-maintainers:

redndahead
http://drupal.org/user/160320

psynaptic
http://drupal.org/user/93429


Support
=======

Issues should be posted in the issue queue on drupal.org:

http://drupal.org/project/issues/views_slideshow
