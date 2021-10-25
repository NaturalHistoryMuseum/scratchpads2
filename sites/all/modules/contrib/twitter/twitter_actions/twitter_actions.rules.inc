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
          'type' => 'text',
          'label' => t('Sender'),
          'description' => t('Twitter account which will be used. ' .
            'By selecting [current user] the rule will check if the user ' .
            'has authenticated a Twitter account to use.'),
          'options list' => '_twitter_rules_account_options',
          'restriction' => 'input',
          'default mode' => 'input',
        ),
      ),
      'base' => 'twitter_actions_set_status',
      'access callback' => 'rules_twitter_actions_access_callback',
    ),
  );
}

/**
 * Returns the list of Twitter accounts to be used for posting.
 *
 * @return
 *   an array of Twitter accounts.
 * @see twitter_actions_account_options()
 */
function _twitter_rules_account_options(RulesPlugin $element, $param_name) {
  return twitter_actions_account_options();
}

/**
 * Fetches Twitter account info and submits with the message to the Twitter API.
 *
 * @param $message
 *   The message to post
 * @param $sender
 *   The Drupal user that has a Twitter account
 */
function twitter_actions_set_status($message, $sender) {
  $twitter_uid = _twitter_actions_get_twitter_id($sender);
  if (!isset($twitter_uid)) {
    // No Twitter authenticated account found.
    return;
  }
  // Send tweet.
  twitter_actions_post($twitter_uid, $message);
}

/**
 * Implements hook_rules_condition_info().
 */
function twitter_actions_rules_condition_info() {
  $items = array();

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
