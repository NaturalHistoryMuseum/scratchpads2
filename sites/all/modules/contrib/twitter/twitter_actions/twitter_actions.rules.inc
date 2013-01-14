<?php
/**
 * @file
 * Provides Rules integration in order to post tweets.
 */

/**
 * Implements hook_rules_action_info() on behalf of the Twitter module.
 */
function twitter_actions_rules_action_info() {
  return array(
    'rules_core_twitter_actions_set_status_action' => array(
      'label' => t('Post a message to Twitter'),
      'group' => t('Twitter'),
      'parameter' => array(
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t("The content of the tweet."),
        ),
        'sender' => array(
          'type' => 'user',
          'label' => t('Sender'),
          'description' => t("User whose Twitter account will be used."),
        ),
      ),
      'base' => 'twitter_actions_set_status',
      'access callback' => 'rules_twitter_actions_access_callback',
    ),
  );
}

/**
 * Fetches Twitter account info and submits with the message to the Twitter API
 *
 * @param $message
 *   The message to post
 * @param $sender
 *   The Drupal user that has a Twitter account
 */
function twitter_actions_set_status($message, $sender) {
  if ($twitter_uid = db_query("SELECT twitter_uid FROM {twitter_account} WHERE uid = :uid", array(':uid' => $sender->uid))->fetchField()) {
    module_load_include('inc', 'twitter');
    $twitter_account = twitter_account_load($twitter_uid);
    try {
      twitter_set_status($twitter_account, $message);
      drupal_set_message(t('Successfully posted to Twitter'));
    }
    catch (TwitterException $e) {
      drupal_set_message(t('An error occurred when posting to Twitter: @message',
                           array('@message' => $e->getMessage())), 'warning');
    }
  }
  else {
    watchdog('twitter', 'Could not find the Twitter account to be used for posting. Please review the rule settings.',
             array(), WATCHDOG_ERROR);
  }
}

/**
 * Implements hook_rules_condition_info().
 */
function twitter_actions_rules_condition_info() {
  $defaults = array(
    'group' => t('Twitter'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user to be checked for.'),
      ),
    ),
    'named parameter' => TRUE,
    'access callback' => 'rules_twitter_actions_access_callback',
  );
  $items['rules_core_twitter_conditions_user_has_linked'] = $defaults + array(
    'label' => t('User has linked Twitter account'),
    'help' => t('Evaluates to TRUE in case there is a record in the twitter_account for the provided user.'),
    'base' => 'twitter_actions_has_linked',
  );

  $items['rules_core_twitter_conditions_text_is_under_140'] = array(
    'group' => t('Twitter'),
    'named parameter' => TRUE,
    'parameter' => array(
      'text' => array(
        'type' => 'text',
        'label' => t('Text to check'),
      ),
    ),
    'label' => t('Text is under 140 characters'),
    'help' => t('Returns TRUE if the length of the text is 140 or less.'),
    'base' => 'twitter_actions_less_140',
  );

  return $items;
}

/**
 * The callback function for the Rules condition
 * @param $element
 *   $element['user']: The user to be checked for.
 * @return
 *   TRUE if the user has linked his/her Twitter account.
 */
function twitter_actions_has_linked($element) {
  return db_query("SELECT twitter_uid FROM {twitter_account} WHERE uid = :uid", array(':uid' => $element['user']->uid))->fetchField() ? TRUE : FALSE;
}

/**
 * The callback function for the Rules condition
 * @param $element
 *   $element['text']: The text of the message.
 * @return
 *   TRUE if the message length is less than 141 characters.
 */
function twitter_actions_less_140($element) {
  return strlen($element['text']) < 141;
}

/**
 * The callback function to access the condition
 */
function rules_twitter_actions_access_callback($type, $name) {
  return user_access('add twitter accounts');
}
