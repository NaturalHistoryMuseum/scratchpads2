-- SUMMARY --

This module roughly acts like the OpenId Drupal 6.x core module :
- It lets you use your Facebook ID and password to sign-in to your drupal site.
- Users can see which of their Facebook friends had an account on your drupal site.
- Users can publish a feed on their Facebook feed announcing that they have created an account or commented an article on your website.
- Users can invite their Facebook friends to create an account on the Drupal website.
- In case the user already holds an account on the website: The user can choose to link his drupal account to his Facebook identity.

-- FACEBOOK CONNECT TAB --

This module adds a new Facebook Connect tab in the user's profile page. This tab allows to change:
- the "Use my Facebook picture as user picture" setting.
This field can be seen only if user_pictures is enabled.
The profile picture will be displayed directly from Facebook via FBML.

- the "Let my Facebook friends see me" setting
If this parameter is enabled, the user's will be visible by her Facebook friends.

Also this tab allows to unlink your Facebook account.

-- REQUIREMENTS --
PHP 5.2 or higher versions.
Drupal 7.x.
Facebook PHP 3.x Library or higher: http://github.com/facebook/php-sdk/ (OAuth 2.0 Support Now)
Facebook API key: http://www.facebook.com/developers/


-- INSTALLATION --
  1. Upload the 'fbconnect' folder into your module directory and activate
     the modules.

  2. Upload facebook-php-sdk library (http://github.com/facebook/php-sdk/) into the libraries
     folder so that it looks like 'sites/all/libraries/facebook-php-sdk/src/facebook.php'.

 -- OR --
  Use drush_make script instead of steps 1 and 2:

     projects[] = fbconnect
     libraries[facebook-php-sdk][download][type] = "get"
     libraries[facebook-php-sdk][download][url] = "http://github.com/facebook/php-sdk/tarball/v3.1.1"
     libraries[facebook-php-sdk][destination] = "libraries"

  3. Create a new Facebook application: http://www.facebook.com/developers/createapp.php

  4. Enter the Site URL on Web Site settings tab (e.g. copy value from admin/config/people/fbconnect field “Connect url”).

  5. Configure the module through admin interface (admin/config/people/fbconnect) using the information provided by Facebook
    (Application ID, Application Secret).

-- About "Connecting" with Facebook --

Facebook expects that once you are logged into Facebook and have registered
with the appid for site X that you would automatically logged into site X
if you visit it in the same browser.

Select "Auto login/logout" to reproduce this behavior.

It gets confusing though as a user and someone who tries to provide choices
to users about what the "Connect" button means.

It can mean any of the following:

1) Login to Facebook, register in Facebook with "Site X", and create a user for "Site X"

2) Register on Facebook with "Site X" and create a user on "Site X"

3) Login to Facebook and login to "Site X"

4) Login to "Site X"

Facebook only gives you information about a visitor if they are BOTH logged
into Facbook and have registered with your appid. So it is impossible to
differentiate from cases 1 - 3.

As a user you might not want to automatically login to a site when you are
logged into Facebook.

This module has a "manual" setting for "Login / Logout" mode to give users
that option.

If you select "manual" there are two options for the name of the button.

- "Not Logged into Facebook" satisfies cases 1 - 3. The default button says
  "Connect".

- "Logged into Facebook and Registered but not Logged into This Site" satisfies
  case 4. The default is "Login" and also provides the user name which is
  made available by Facebook.

-- FAQ --

Q: How to skip linking local account to FB account?
A: Enable "Disable linking accounts during registration" checkbox on admin/config/people/fbconnect/apperance page

Q: Why the connect button does not appear?
Q: Facebook Connect dialog says: "Invalid Argument: Given URL is not allowed by the Application configuration".
A: Check 5th step of installation instructions. Check for error messages on page admin/reports/status.

Q: Fbconnect redirects to the Facebook homepage in popup
Q: I'm getting error "Application Unavailable - The application you are trying to access is unavailable or restricted"
A: Try to disable all other facebook* modules. If you're using fb_social you have to enable fbconnect_fb_social.

Q: Cannot login to drupal site after sucessully logged in to Facebook.
A: Check your Application Secret on admin/config/people/fbconnect page.

Q: Fbconnect Causes W3C XHTML Validation To Fail
A: http://drupal.org/node/365584#comment-3539890

Still did not solve problem ? Disable other facebook* related modules, check admin/reports/status page, php logs,
post issue on http://drupal.org/project/issues/fbconnect
