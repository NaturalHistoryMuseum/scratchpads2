Description
-----------
This module provides basic Facebook connect ability through the Facebook OAuth
API. While out of the box it only provides functionality for logging users into
your site, it can be extended with various actions (such as a contact import) through its APIs.

Requirements
------------
Drupal 7.x

Installation
------------
1. In order for your site to use Facebook Connect you must register an
   application on Facebook. Visit https://www.facebook.com/developers/apps.php
   and create a new application, usually simply the name of your website such as
   "example.com".

2. While setting up your application, set the "Deauthorize Callback" to
   "http://example.com/fboauth/deauthorize". This will allow Facebook OAuth to
   cleanup user information if the application is disconnected from the Facebook
   site.

3. Copy the entire fboauth directory the Drupal sites/all/modules directory.

4. Login as an administrator. Enable the module on the Modules page.

5. Configure the Facebook OAuth module settings under "Configuration" ->
   "People" -> "Facebook OAuth settings". Copy and paste the App ID and
   App Secret from your newly created Facebook application's settings.

   Note that it is highly recommended to request access to the Facebook user's
   e-mail address so that normal Drupal functionality (the password reset for
   example) will continue to work even if the user has logged in with Facebook.
   This option is enabled by default.

   If you have installed the Profile module, you may also map information
   between your Profile fields and Facebook's available fields.

6. You can enable the Facebook connect button either by enabling the included
   Facebook OAuth block under "Structure" -> "Blocks", or you can print out the
   link manually in your theme with the following code:

   <?php print fboauth_action_display('connect'); ?>

7. Click on the Facebook Connect button to bind your Facebook account together
   with your user account. If you are logged out when you connect for the first
   time, a new account will be created for you. If you are logged in when you
   click the connect button, your existing account will be associated with your
   Facebook login.

If you just want to connect your site with Facebook, you do not need to read any
further. The module is now configured and the user can log into your site. If
you'd like to customize the display of your Facebook connect link or query other
information from Facebook you may read the sections on Theming and the APIs.

Theming
-------
The Facebook OAuth module includes default theming connecting to Facebook. You
can override this output (or any other output in Facebook OAuth) by doing the
following:

1. Copy the theme_fboauth_action__connect() function from fboauth.module file.

2. Paste this function into your template.php file in your theme.

3. Change the function's output to match your liking (such as adding a class or
   rel attribute to the link).

4. Clear the Drupal theme cache at admin/settings/performance. Click the "Clear
   all caches" button at the bottom of the page.

Or an alternative if you don't want to use the fboauth_action_display() function
to print out the link, you can simply grab the link properties from the
fboauth_action_link_properties() function and output the link manually:

<?php
$link = fboauth_action_link_properties('connect');
print l(t('Connect'), $link['href'], array('query' => $link['query']));
?>

API Integration
---------------
The Facebook OAuth module provides an API for executing queries against
Facebook's vast store of user data. However in order to use this API it is
important to understand the basic concepts of OAuth. In short, the user (and
only the user) is capable of granting your site access to query information
against Facebook. The user is also only able to do this on Facebook.com, so any
requests to query against Facebook must first redirect the user to Facebook
where they can grant access. The full workflow looks like this:

1. The user clicks on a link (such as the Facebook Connect button) that sends
   the user to Facebook. If the link is requesting permissions that the user has
   not yet granted, the user is prompted to allow access. After the user has
   granted access, or if the user granted access previously, the user is 
   redirected back to your site.

2. When the user is redirected back to your site, Facebook sends along an access
   "code". Your site then takes this access code and does a server-side request
   to Facebook's API servers. Facebook's servers return an access "token" to
   your server. This token is valid for a short amount of time and allows you to
   access the information to which the user granted you access.

3. Your site can now execute queries against the user's Facebook information
   while the token is valid. Because this token only lasts a short amount of
   (about 6 hours usually), it's safest to always request access from Facebook
   before every data import session (by having the user click the link), which
   will renew the existing token or generate a new one.

So all in all, this is a lot of back and forth between your site and Facebook
before you can query a user's information. Fortunately the Facebook OAuth module
handles all of the back and forth and gives you the necessary access token. All
you need to do is register a function with Facebook OAuth and it does all the
work of getting you access. You just write the query and import necessary
information.

Integrating with Facebook OAuth requires writing a module (though it can just be
a few lines). If you help writing a module, the Examples module is a good
introduction to writing modules: http://drupal.org/project/examples.

In your module file, you first need to implement hook_fboauth_actions() such as 
this:

<?php
/**
 * Implements hook_fboauth_actions().
 */
function mymodule_fboauth_actions() {
  // Give each action a unique key, such as "mymodule_photo_import" for a photo
  // import. This function should begin with the name of your module.
  $actions['mymodule_photo_import'] = array(
    // Give you action a human-readable name. This will be used when showing
    // the user a link to start this action.
    'title' => t('Import my Facebook photos'),

    // Specify the name of your callback function that contains the import.
    'callback' => 'mymodule_fboauth_action_photo_import',

    // Specify permissions you need to do this action. See the Facebook API for
    // a list: http://developers.facebook.com/docs/authentication/permissions/
    'permissions' => array(
      'user_photos', // Gets access to a user's photos.
    ),

    // Optionally specify a file that contains your callback function. If you
    // put your callback function in the .module file, this is unnecessary.
    // 'file' => 'mymodule.inc',

    // Optionally define a theme function for printing out your link (not
    // including the "theme_" prefix). If you use this option, you must register
    // this function in hook_theme(). If you don't use this option, the link
    // will be output with the theme_fboauth_action() function or the automatic
    // suggestion theme_fboauth_action__[action_name]().
    // 'theme' => 'mymodule_fboauth_action',
  );
  return $actions;
}
?>

Then write your function specified as the "callback" in the above hook.

<?php
/**
 * Facebook OAuth action callback; Import a user's Facebook photos.
 */
function mymodule_fboauth_action_photo_import($app_id, $access_token) {
  // Query against the Facebook Graph API. See the Facebook API for a list of
  // commands: http://developers.facebook.com/docs/reference/api/
  $result = fboauth_graph_query('me/photos', $access_token);
  foreach ($result['data'] as $photo) {
    // Import into Drupal.
  }

  // Optionally set a completion or error message.
  drupal_set_message(t('Import complete!'));

  // Optionally return a path to which the user will be redirected. If not set
  // the path in the $_REQUEST['destination'] variable will be used. If there
  // is no path at all specified, the user will be redirected to the homepage.
  return 'mymodule/import-complete';
}
?>

Now to get the user to actually execute this action, you need to link to
Facebook so that the user can grant the necessary access. You can do this with
the utility function fboauth_action_display(). Our example action was keyed as
"mymodule_photo_import", so we would print the link like this:

<?php print fboauth_action_display('mymodule_photo_import'); ?>

Now when the user clicks on the output link, they will have the option of
granting access to the requested information. If they approve, your callback
function will be executed.

More information about Facebook OAuth's other hooks are documented in the 
fboauth.api.php file included with this module.

Deauthorization or access revocation from Facebook.
---------------------------------------------------
If a user revokes access to the application from the Facebook side, and the
application has provided a Deauthorize callback URL (see Install step 2 above),
Facebook will notify your site that the user has disconnected their account from
your site.

A hook is provided to allow other modules to respond to this event as well - see
hook_fboauth_user_deauthorize() in fboauth.api.php file.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/fboauth?categories=All
