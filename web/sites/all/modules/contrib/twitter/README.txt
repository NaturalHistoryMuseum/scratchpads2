Introduction
============
Twitter module allows listing tweets in blocks or pages. Its integration with Views opens the
door to all sorts of formatting (ie. as an automatic slideshow with views_slideshow). It also
provides useful input filters to easily link Twitter accounts and searches within text.

Twitter's submodules allow posting to twitter, executing actions/rules when tweeting or login
with a Twitter account.

Installation
============
OAuth module is required for all requests to the Twitter REST API 1.1. When you download the OAuth module, get the latest stable release available at http://drupal.org/project/oauth

Once OAuth and Twitter have been enabled, go to admin/config/services/twitter and follow instructions in order
to provide your Twitter Application keys.

You can find further installation instructions at http://drupal.org/node/1346824

How to use the username and hashtag input filters
=================================================
1. Go to admin/config/content/formats.
2. Select the text format where you want to use the filters.
3. At "Enabled filters" check the Twitter converters.

After that, clear cache and try to create a page with the following body:

#drupal @drupal

The above links to a search in Twitter over the #drupal tag and a to the @drupal profile.
These filters are avilable when configuring a tweets Views.

How to post to Twitter
======================
1. Read the OAuth section to install and configure OAuth.
2. Once OAuth has been configured, go to admin/config/services/twitter/post and select from which
   node types a user may post to Twitter and the default message.
3. Verify permissions at admin/people/permissions.
4. Add a Twitter account and try to edit or post content.

Further information can be found at http://drupal.org/node/1016584.

How to sign in with Twitter
===========================
Existing and new users can sign in with Twitter by enabling the twitter_signin module. The following scenarios are being contemplated so far:

* A visitor logs in with his Twitter account and, once authenticated at Twitter.com, he fills in
  his email in the Drupal registration form and receives an email to log in and set his account
  password.
* An existing user signs in with Twitter and then logs in into his Drupal user account. This results
  in the Twitter account getting related to the user account so next time Twitter sign in will work.
* An existing user with an already configured Twitter account can log in automatically by clicking
  on the Sign in with Twitter button.
