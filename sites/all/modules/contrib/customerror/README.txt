CUSTOM ERROR README.txt
=======================

This module allows the site admin to create custom error pages for 404
(not found), and 403 (access denied).

Since the error pages are not real nodes, they do not have a specific
content type, and will not show up in node listings.

Features
--------

* Configurable page title and descriptions.
* Any HTML formatted text can be be put in the page body.
* Handles 404 and 403 errors at present. Drupal only allows those two
  errors to be assigned custom pages. The design of this module is
  flexible though and can accommodate future codes easily.
* The pages are themeable using the phptemplate_customerror() function
  in the template.php. The first argument is the error code (currently
  403 or 404), and the message content.
* The messages can contain PHP, using one of two methods:
  - By using the phptemplate_customerror() function (see above).
  - By using the PHP checkbox in the settings.
* Users who are not logged in and try to access an area that requires
  login will be redirected to the page they were trying to access after
  they login.
* Return the correct HTTP status codes (403 and 404), which will
  prevent robots from indexing the error pages.

It also allows the site admin to setup static redirects for 404s. For
example if you had a page called foo and a page called xyz, then you
moved them to a page called bar, and abc respectively, you can setup a
redirect pair of:

  ^foo$ bar
  ^xyz$ abc

The first pair will transparently redirect users trying to access
example.com/foo to example.com/bar.  The first pair will transparently
redirect users trying to access example.com/xyz to example.com/abc.

You can have multiple pairs of redirects. Each must be on a line by
itself.

Note that the first string is a regexp, and the second string is a
path. You have to use a single space between them.  You cannot use
variables.  For more flexible URL rewriting, including variables, you
may consider using an external URL rewrite engine, such as Apache
mod_rewrite.


Redirecting upon login
----------------------

Here is an example of how to add custom PHP to a 403 to give the user the
option to login then redirect them to what they were after.

<?php
global $user;
if ($user->uid == 0) {
  $output = '<p>';
  $output .= t('If your user account has access to this page, please !message.',
    array('!message' =>
      l('log in', 'user', array('destination' => drupal_get_destination())),
    )
  );
  $output .= '</p>';
  print $output;
}
?>

That way when there's a 403 they get redirected back to the page they were trying to access.
The above should be better refined to fit "best practices", such as doing this in a template.php
rather than code stored in the database.

Thanks to: Andrew Berry (http://drupal.org/user/71291 deviantintegral).


Database
--------

This module does not require any new database tables to be installed.


Installation:
-------------

1. Install the customerror module directory in the directory where you
   keep contributed modules (e.g. sites/all/modules/).

2. Go to the Modules page
   - Enable the customerror module.
   - If you want to messages to contain PHP, enable the core
     PHP filter module.
   - Click on Save configuration.

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
   - You can use any HTML tags to format the text.
   Click on Save configuration.

5. Test your error pages.
   - Copy your present admin page url.
   - Try to go to a non-existent Drupal page on your site.
   You should see your custom error page for 404 (not found) page.

   - Log out from your site.
   - Paste the admin page url and try to go there.
   You should see your custom error page for 403 (access denied) page.


FAQ
---

* I want to prevent robots from indexing my custom error pages by
  setting the robots meta tag in the HTML head to NOINDEX.

  - There is no need to. CustomError returns the correct HTTP status
    codes (403 and 404). This will prevent robots from indexing the
    error pages.

* Some 403 errors (e.g. "http://example.org/includes") are served by
  the Apache web server and not by CustomError. Isn't that a bug?

  - No. CustomError is only designed to provide a custom error page
    when the page is processed by Drupal.  The .htaccess file that
    comes with Drupal will catch some attempts to access forbidden
    directories before Drupal even see the requests.  These access
    attempts will get the default Apache 403 error document, unless
    you use the Apache ErrorDocument directive to override this, e.g:
      ErrorDocument 403 /error/403.html
    For more information about this, see:
    http://httpd.apache.org/docs/current/custom-error.html


Bugs/Features/Patches
---------------------

If you want to report a bug, request a feature, or submit a patch,
please do so in the issue queue at the project page on the Drupal web
site:

   http://drupal.org/project/customerror


Author
------

Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com)

If you use this module, find it useful, and want to send the author a
thank you note, then use the Feedback/Contact page at the URL above.

The author can also be contacted for paid customizations of this and
other modules.
