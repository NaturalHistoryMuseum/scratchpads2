$Id: README.txt,v 1.7.2.4 2009/10/01 18:55:12 kbahey Exp $

Copyright 2005 http://2bits.com

Description
-----------
This module allows the site admin to create custom error pages for
404 (not found), and 403 (access denied).

Since the error pages are not real nodes, they do not belong a category
term, and hence will not show up in node listings.

It also allows the site admin to setup redirects for 404s. For example
if you had a page called foo and a page called xyz, then you moved them
to a page called bar, and abc respectively, you can setup a redirect pair
of:

  ^foo$ bar
  ^xyz$ abc

Users trying to access example.com/foo will be transparently redirected
to example.com/bar.

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

1. Copy the customerror.module to the Drupal modules/ directory.

2. Go to Administer -> Build -> Modules
   - Enable the customerror module, click on Save

3. Configure Error reporting
   - Go to Administer -> Site configuration -> Error reporting
   - For 403 (access denied), enter the value:
       customerror/403
   - For 404 (not found), enter the value:
       customerror/404

4. Configure the module:
   - Go to Administer -> Site configuration -> Custom error
   - Enter any title and description you want for the 404 (not found)
     and 403 (access denied) pages.
   - You can use any HTML tags to format the text.
   - Ensure the Enable checkbox is checked. That sets or unsets the Error
     Reporting settings for you.

5. Test your error pages.
   - Copy your present admin page url.
   - Try to go to a non-existent Drupal page on your site.
   You should see your custom error page for 404 (not found) page.

   - Log out from your site.
   - Paste the admin page url and try to go there.
   You should see your custom error page for 403 (access denied) page.

Bugs/Features/Patches
---------------------
If you want to report bugs, feature requests, or submit a patch, please do so
at the project page on the Drupal web site.
http://drupal.org/project/customerror

Author
------

Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com)

If you use this module, find it useful, and want to send the author
a thank you note, then use the Feedback/Contact page at the URL above.

The author can also be contacted for paid customizations of this
and other modules.
