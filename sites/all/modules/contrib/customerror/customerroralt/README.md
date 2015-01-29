INTRODUCTION
------------
This module allows the site admin to create custom error pages for HTTP status
code 403 (access denied) for authenticated users that are different from status
code 403 (access denied) for anonymous users.

The two situations (log in and you will probably be able to see this versus
given the privileges granted to your user account you are denied access) should
really be separate status codes.  And indeed, they are, but because status code
401 (unauthorized) is specified to require a WWW-Authenticate header field
Drupal will not use it.  That would cause the browser to handle authentication,
rather than Drupal being able to provide register, log in, and forgot password
forms, so we're stuck overloading 403.

It builds on the Custom Error module and so inherits its main features:

* Configurable page title and descriptions.
* There are no author and date/time headers as with normal nodes.
* Any HTML formatted text can be be put in the page body.
* The error pages are themeable.
* Users who are not logged in and try to access an area that requires
  login will be redirected to the page they were trying to access after
  they login.

Since the error pages are not real nodes, they do not have a specific
content type, and will not show up in node listings.

This module does not require any new database tables to be installed.


REQUIREMENTS
------------
This module requires the following module:
 * Custom Error (https://www.drupal.org/project/customerror)


INSTALLATION
------------

Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7 for
further information.


CONFIGURATION
-------------

0. Configure Custom Error per its instructions.  Access denied errors, under
   Configuration -> System -> Site information, must be set to use
   customerror/403.

1. Add a custom error message for authenticated users getting access denied:
   - Go to Configuration -> System -> Custom error
   - Enter any title and description you want for the 403 (access denied) pages.
   - You can use any HTML tags to format the text.
   Click on Save configuration.

2. Test your error pages.

   - Log into your site as a non-administrator user.
   - Try to go to admin/modules
   You should see your custom error page title and text.


MAINTAINERS
-----------

Principal author is Benjamin Melançon of Agaric.
  - https://drupal.org/u/mlncn
  - http://agaric.com

The author can be contacted for paid customizations of this module as well as
other Drupal consulting, development, and training.


FURTHER CREDIT
--------------

This module relies entirely on the Custom Error module principally written by
Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com) and
ported to Drupal 7 by Gisle Hannemyr (https://drupal.org/user/409554).
