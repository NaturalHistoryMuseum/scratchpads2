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
 * Notifies of a saved twitter account.
 *
 * @param $account
 *   User account object.
 * @param $values
 *   Twitter account values.
 * @see twitter_account_save()
 */
function hook_twitter_account_save($account, $values) {}
