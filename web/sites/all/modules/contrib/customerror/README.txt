CUSTOMERROR README.txt
======================


CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Configuration
  - Redirecting upon login
  - Custom redirects for 404 errors
* Submodule
* Integration with other modules
* FAQ
* Maintainers


INTRODUCTION
------------

This module allows the site admin to create custom error pages for
HTTP status codes 403 (access denied) and 404 (not found), without the
need to create nodes for each of them.

Main features:

* Configurable page title and descriptions.
* There are no author and date/time headers as with normal nodes.
* Any HTML formatted text can be be put in the page body.
* The error pages are themeable.
* Users who are not logged in and try to access an area that requires
  login will be redirected to the page they were trying to access after
  they login.
* Allows custom redirects for 404s.

Since the error pages are not real nodes, they do not have a specific
content type, and will not show up in node listings.

At present, the module can be set up to handle 403 and 404
errors. Drupal only allows those two errors to be assigned custom
pages. However, the design of the module is flexible and can
accommodate future error codes easily.

This module does not require any new database tables to be installed.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/customerror
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/customerror
 * For more documentation, please see:
   https://www.drupal.org/node/2064843


INSTALLATION
------------

1. Install the CustomError module directory in the directory where you
   keep contributed modules (e.g. sites/all/modules/).

2. Go to the Modules page
   - Enable the CustomError module.
   Click on Save configuration.

3. Configure Error reporting
   - Go to Configuration -> System -> Site information
   - For 403 (access denied), enter the value:
       customerror/403
   - For 404 (not found), enter the value:
       customerror/404
   Click on Save configuration.

4. Configure the module:
   - Go to Configuration -> System -> Custom error
   - Enter any title and description you want for the 404 (not found)
     and 403 (access denied) pages.
   - You may also set theme to be used on the error pages. The first
     option (System default) lets the system set the theme. Each of
     the remaining options lets you set an explicit theme to be used
     on error pages (but it will not override the administration
     theme, if set).
   - You can use any HTML tags to format the text.
   Click on Save configuration.

5. Test your error pages.
   - Copy your present admin page url.
   - Try to go to a non-existent Drupal page on your site.
   You should see your custom error page for 404 (not found) page.

   - Log out from your site.
   - Paste the admin page url and try to go there.
   You should see your custom error page for 403 (access denied) page.


CONFIGURATION
-------------

Custom redirects for 404 errors
-------------------------------

It is possible to set up custom redirects for status code 404 (not
found).

For example if you had a page called foo and a page called xyz, then
you moved them to a page called bar, and abc respectively, you can
setup a redirect pair of:

  ^foo$ bar
  ^xyz$ abc

The first pair will transparently redirect users trying to access
example.com/foo to example.com/bar.  The second pair will
transparently redirect users trying to access example.com/xyz to
example.com/abc.

You can have multiple pairs of redirects. Each must be on a line by
itself.

Note that the first argument is a regexp, and the second argument is a
path. You have to use one space between them, and enter each pattern
on a line by itself. You cannot use variables.

For more flexible URL redirection or rewriting, including variables,
you may consider the Drupal Redirect module, or using an external URL
rewrite engine, such as Apache mod_rewrite.  If you use some other
means of redirection or rewriting, you should refrain from using the
redirect feature of CustomError.



Using custom PHP on an error page
----------------------------------

If you want error pages to contain PHP, enable the core PHP filter
module.  This allows you to include PHP code (enclosed in <?php ?>
tags) for the error page message.  Note that this can be dangerous in
some situations. Make sure that you are aware of the implications.

Here is an example of how to add custom PHP to a 403 error page to
check if the user is logged in.  If the user is not logged in, a
message saying 'access denied: insufficient permissions' is shown,
otherwise the user is given the option to log in:

<?php
if (user_is_logged_in()) {
   $output = '<p>' . t('access denied: insufficient permissions') . '</p>';
} 
else {
  $output = t('If your user account has access to this page, please !message.',
    array(
      '!message' => l(t('log in'), 'user'),
    )
  );
  $output .= '</p>';
}
print $output;
?>

Note that enabling the PHP filter module is depreciated (it will no
longer be part of core for Drupal 8).  For a safer method to show
different error pages for access denied pages for anonymous and logged
in users, enable the submodule that is part of the project:
CustomError alternate for authenticated.

If your handling of access denied errors allows the user to log in
after been shown the message, CustomError keeps track of what page the
user is trying to access. After succesfully logging in, the user will
be redirected to the page he or she originally requested.


SUBMODULE
---------

Packaged with the project is the submodule: CustomError alternate for
authenticated.

Enabling this sub-module will add fields that allow the administrator
to add a title and description for 403 (access denied) for
authenticated users that are different from status code 403 (access
denied) for anonymous users.

See the submodule's own README.md for more documentation.


INTEGRATION WITH OTHER MODULES
------------------------------

The function customerror_page() can be called by other modules to have
CustomError or CustomError alternate for authenticated handle the
error.

* LoginToboggan[1]:
  If this module is enabled, in can enhance CustomError's handling of
  access-denied messages, but you have to be careful to set them up to
  work together correctly.

  These two modules both attempt to take over handling of system 403
  ("access denied") messages, and can conflict. CustomError does it by
  asking you to go to "admin/config/system/site-information" and
  manually setting the "Default 403 (access denied) page" to
  "customerror/403", whereas LoginToboggan sets that same field to
  "toboggan/denied" automatically (overwriting any other value that
  was there), when you enable its "Present login form on access denied
  (403)".

  If you are using CustomError with LoginToboggan, you should allow
  LoginToboggan to perform this take-over (in other words, don't set
  the "Default 403 (access denied) page" to "customerror/403"). This
  way, if someone attempts to access a page that they don't have
  access to, LoginToboggan will first give them a chance to log in if
  they haven't yet. If they still don't have access to the page,
  CustomError then takes over from LoginToboggan (by overriding one of
  its theme functions), displaying its customisable messages for
  access-denied errors.


FAQ
---

Q: I want to prevent robots from indexing my custom error pages by
   setting the robots meta tag in the HTML head to NOINDEX.
A: There is no need to. CustomError returns the correct HTTP status
   codes (403 and 404). This will prevent robots from indexing the
   error pages.
	
Q: I want to customize the custom error template output.
A: In your site's theme, duplicate your page.tpl.php to be
   page--customerror.tpl.php and then make your modifications there.

Q: I want to have a different template for my 404 and 403 pages.
A: Duplicate your page.tpl.php page to be
   page--customerror--404.tpl.php and
   page--customerror--403.tpl.php. You do not need a
   page--customerror.tpl.php for this to work.

Q: Some 403 errors (e.g. "http://example.org/includes") are served by
   the Apache web server and not by CustomError. Isn't that a bug?
A: No. CustomError is only designed to provide a custom error page
   when the page is processed by Drupal.  The .htaccess file that
   comes with Drupal will catch some attempts to access forbidden
   directories before Drupal even see the requests.  These access
   attempts will get the default Apache 403 error document, unless you
   use the Apache ErrorDocument directive to override this, e.g:
   ErrorDocument 403 /error/403.html For more information about this,
   see: http://httpd.apache.org/docs/current/custom-error.html


MAINTAINERS
-----------

Principal author is Khalid Baheyeldin
(http://baheyeldin.com/khalid and http://2bits.com).

Port to Drupal 7 port has been overseen by Gisle Hannemyr
(https://www.drupal.org/u/gisle).

The authors can be contacted for paid customizations of this module
as well as Drupal consulting, installation, development, and
customizations.


[1]: https://www.drupal.org/project/logintoboggan
