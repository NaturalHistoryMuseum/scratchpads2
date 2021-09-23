Drupal site map module:
----------------------
Author - Fredrik Jonsson fredrik at combonet dot se
Requires - Drupal 6
License - GPL (see LICENSE)


Overview:
--------
This module provides a site map that gives visitors an overview of
your site. It can also display the RSS feeds for all blogs and
terms. Drupal generates the RSS feeds automatically but few seems
to be aware that they exist.

The site map can display the following items:

* A message to be displayed above the site map
* The front page.
* The latest blogs.
* Any books that optionally will be displayed fully expanded.
* Any menus that will be displayed fully expanded.
* Any vocabulary with all the terms expanded.
  Optionally with node counts and RSS feeds.
* Display comment RSS links if the Comment RSS module is installed.
* A syndication block, the "more" link goes to the site map.


Installation:
------------
1. Place this module directory in your modules folder (this will
   usually be "sites/all/modules/").
2. Go to "Administer" -> "Site building" -> "Modules" and enable the module.
3. Check the "Administer" -> "User management" -> "Permissions" page to
   enable use of this module to different roles.
4. Make sure the menu item is enabled in
   "Administer" -> "Site building" -> "Menus" -> "Navigation".
   You may move it to another menu if you like.
5. Have a look at the different settings in
   Administer -> Site configuration -> Site map
6. Visit http://example.com/sitemap.

If you have many books/menus/vocabularies the Checkall module is recommended.
It will implement "Check all / Uncheck all" for checkboxes on the
site map settings page. http://drupal.org/project/checkall

Site map term path (and Pathauto):
-------------------------------
There is a "depth" setting on the Site map settings page where you can adjust
how site map constructs the term links.

For making Site map build the same path that Pathauto per default generates
alias for you should set this to "-1" I belive.


Last updated:
------------
