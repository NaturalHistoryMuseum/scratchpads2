<?php

/**
 * @file
 * Describe hooks provided by Twitter module.
 */

/**
 * Loads Twitter accounts for a user.
 *
 * @param $account
 *   stdClass object containing a user account.
 * @return
 *   array of stdClass objects with the associated Twitter accounts.
 * @see twitter_twitter_accounts()
 */
function hook_twitter_accounts($account) {}

/**
 * Notifies of a saved tweet.
 *
 * @param $status
 *   stdClass containing information about the status message.
 * @see https://dev.twitter.com/docs/platform-objects/tweets for details about the contents of $status.
 */
function hook_twitter_status_save($status) {}

/**
 * Notify of a saved twitter account.
 *
 * This hook is invoked by twitter_account_save() whenever a new Twitter
 * account is added or an existing one updated (on new statuses being imported
 * or import settings being changed).
 *
 * @param array $values
 *   An associative array containing, for new accounts or for accounts being
 *   updated when new statuses are imported:
 *   - screen_name: (string) Twitter "handle", e.g. 'juampytest'.
 *   - name: (string) display name, e.g. 'Juampy Test Account'.
 *   - location: (string) location, if any.
 *   - description: (string) description, if any.
 *   - followers_count: (int) the number of followers.
 *   - friends_count: (int) the number of accounts given account is following.
 *   - statuses_count: (int) the number of statuses.
 *   - favourites_count: (int) the number of favorited statuses (note British
 *     spelling here).
 *   - url: (string) URL, if any.
 *   - protected: (int 0|1) whether the account is protected.
 *   - profile_image_url: (string) profile image URL; may point to one of
 *     default images.
 *   - profile_background_color: (string) hex color code, e.g. 'BEDEAD'.
 *   - profile_text_color: (string) hex color code, e.g. 'BEDEAD'.
 *   - profile_link_color: (string) hex color code, e.g. 'BEDEAD'.
 *   - profile_sidebar_fill_color: (string) hex color code, e.g. 'BEDEAD'.
 *   - profile_sidebar_border_color: (string) hex color code, e.g. 'BEDEAD'.
 *   - profile_background_image_url: (string) profile background image URL; may
 *     point to one of default backgrounds.
 *   - profile_background_tile: (int 0|1) whether the background image is
 *     tiled. How exactly does this setting work?
 *   - verified: (int 0|1) whether the account is verified.
 *   - created_at: (string) the time account was created, e.g.
 *     'Thu Oct 13 10:21:00 +0000 2011'.
 *   - created_time: (int) UNIX timestamp of account creation time, e.g.
 *     1318501260.
 *   - utc_offset: (int) timezone offset in seconds, e.g. 7200.
 *   - twitter_uid: (int) Twitter user ID of the account, e.g. 390017783.
 *   In addition to that, $values for authenticated accounts contains also:
 *   - oauth_token: (string) OAuth token.
 *   - oauth_token_secret: (string) OAuth secret.
 *   For new accounts being added, $values contains also:
 *   - uid: (string) user ID of the user adding the account, e.g. '1'. NOTE:
 *     this is not necessarily connected to the account being saved and may
 *     well be administrator's user ID. The presence of this key, however, lets
 *     you tell apart new accounts from accounts being updated.
 *   For accounts whose import settings are being updated on
 *   admin/config/services/twitter page, $values contains ONLY the following
 *   keys:
 *   - screen_name: (string) Twitter "handle", e.g. 'juampytest'.
 *   - import: (int 0|1) whether statuses of this account are being imported.
 *   - mention: (int 0|1) (only for authenticated accounts) whether mentions of
 *     this account are being imported.
 *   - twitter_uid: (string) Twitter user ID of the account, e.g. '390017783'.
 *   Note that pressing 'Save changes' button saves all accounts on the page,
 *   regardless of whether the settings have been changed or not.
 *
 * @ingroup hooks
 *
 * @see twitter_account_save()
 */
function hook_twitter_account_save($values) {
  if (isset($values['uid'])) {
    watchdog('twitter', 'A new Twitter account %handle has been added.', array('%handle' => $values['screen_name']));
  }
  else {
    watchdog('twitter', 'Twitter account %handle has been updated.', array('%handle' => $values['screen_name']));
  }
}

/**
 * Alter the twitter user settings page.
 *
 * @param array $output
 *   A render array containing the user settings data.
 */
function hook_twitter_user_settings_alter(&$output) {}

/**
 * Notifies that the module is about to update a user timeline.
 *
 * @param $account
 *   User account object.
 * @param array $params
 *   Any arguments that are going to be passed to the Twitter API. May already
 *   include the 'since' argument.
 *
 * @see twitter_fetch_user_timeline()
 */
function hook_twitter_prefetch_timeline($account, $params) {
  watchdog('mymodule', 'About to fetch the tweets for %screenname.', array('%screenname' => $account->screen_name));
}

/**
 * Allow the system to modify tweets that are about to be saved.
 *
 * @param array $statuses
 *   The statuses to be saved.
 * @param object $account
 *   User account object.
 *
 * @see twitter_fetch_user_timeline()
 */
function hook_twitter_statuses_alter(&$statues, $account) {
  watchdog('mymodule', 'About to insert %count tweets for %screenname.', array('%count' => count($statuses), '%screenname' => $account->screen_name));
}

/**
 * Allow the system to react after tweets are saved.
 *
 * @param array $statuses
 *   The statuses that were saved.
 * @param object $account
 *   User account object.
 *
 * @see twitter_fetch_user_timeline()
 */
function hook_twitter_insert_statuses($statues, $account) {
  watchdog('mymodule', '%count tweets were imported for %screenname.', array('%count' => count($statuses), '%screenname' => $account->screen_name));
}
