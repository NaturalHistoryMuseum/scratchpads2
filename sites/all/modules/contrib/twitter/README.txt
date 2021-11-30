Twitter
-------
The Twitter module allows listing tweets in blocks or pages. Its integration
with Views opens the door to all sorts of formatting (ie. as an automatic
slideshow with views_slideshow). It also provides useful input filters to easily
link Twitter accounts and searches within text.

Twitter's submodules allow posting to twitter, executing actions/rules when
tweeting, login with a Twitter account, or listing the most recent tweet from a
specific account.


Installation
--------------------------------------------------------------------------------
The OAuth module is required:
  https://www.drupal.org/project/oauth

When installing the Twitter module without the above being available, Drupal
will complain about a missing "oauth_common" module. This module is actually
provided by the OAuth project - the module name and project name are not the
same, hence this error.

Once OAuth and Twitter have been enabled, go to admin/config/services/twitter
and follow the instructions in order to provide the Twitter Application keys.

Further installation instructions can be found at:
  https://www.drupal.org/node/1346824


How to use the username, hashtag and embedded tweet input filters
--------------------------------------------------------------------------------
1. Go to admin/settings/filters.
2. Select the text format where the filters are to be added to.
3. At "Enabled filters" check the Twitter filters.

After that, clear cache and try to create a page with the following body:

#drupal @drupal

This will link to a search in Twitter for the #drupal hashtag, and to the
@drupal account profile page.

To display a single tweet in a page, insert a full URL to a tweet in a node's
body field, e.g.:

https://twitter.com/drupal/status/580521032664145920

These filters are avilable when configuring list of tweets in Views.

Note: These filters may be used without OAuth being configured.


How to post to Twitter
--------------------------------------------------------------------------------
 1. Install and configure OAuth, as described above.

 2. Verify permissions at admin/people/permissions:
    - Post a message to Twitter
      Users with this permission will be able to post to Twitter using an
      authenticated account that they have added to the site.
    - Post a message to Twitter using a global account
      Users with this permission will be able to post to Twitter using an
      account that is set up with the "is global" option.

 3. Go to admin/config/services/twitter/post and select from which node types a
    user may post to Twitter, and the default message that will be posted.

 4. Add a Twitter account and try to edit or post content.

Further information can be found at https://www.drupal.org/node/1016584.


How to sign in with Twitter
--------------------------------------------------------------------------------
Existing and new users can sign in with Twitter by enabling the twitter_signin
module. The following scenarios are being supported so far:

* A visitor logs in with their Twitter account and, once authenticated at
  Twitter.com, fills in their email in the Drupal registration form and receives
  an email to log in and their account password.

* An existing user signs in with Twitter and then logs in into their Drupal user
  account. This results in the Twitter account mecoming related to the user
  account so next time Twitter sign-in will work.

* An existing user with an already configured Twitter account can log in
  automatically by clicking on the "Sign in with Twitter" button.


Credits / Contact
--------------------------------------------------------------------------------
Currently maintained by Damien McKenna [1]. Originally written by James Walker
[2] with many contributions by Michael Hellein [3], Juampy Novillo Requena [4],
Chris Burgess [5], Jeff Eaton [6] and others in the community.

Ongoing development is sponsored by Mediacurrent [7].

The best way to contact the authors is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  https://www.drupal.org/project/issues/twitter


References
--------------------------------------------------------------------------------
1: https://www.drupal.org/u/damienmckenna
2: https://www.drupal.org/u/walkah
3: https://www.drupal.org/u/michaek
4: https://www.drupal.org/u/juampynr
5: https://www.drupal.org/u/xurizaemon
6: https://www.drupal.org/u/eaton
7: http://www.mediacurrent.com/
