<?php

/**
 * Implements hook_drush_command().
 */
function scratchpads_messages_drush_command() {

  $commands['set-message'] = array(
    'description' => 'Adds a new message to a scratchpad\'s messages list.',
    'aliases' => array('msg'),
    'arguments' => array(
       'message' => 'Message',
     ),
     'options' => array(
       'expires' => 'Expiry date (strtotime format)',
     ),
     'examples' => array(
       'drush msg "There are ongoing server issues"' => 'Add a message.',
       'drush msg "This event expires soon" --expires="2020-06-25 9:00"' => 'Adds a message which expires on June 25th 2020, 9am',
       'drush msg "This event expires in one week" --expires="+1 week"' => 'Adds a message which expires one week from now',
     ),
  );

  $commands['clear-messages'] = array(
    'description' => 'Clears global messages.',
    'aliases' => array('msg-clear'),
     'examples' => array(
       'drush msg-clear',
     ),
  );

  return $commands;
}

/**
 * Create a global message
 * drush_[COMMAND_NAME]().
 */
function drush_set_message($message) {
  if(module_exists('scratchpads_messages')){
    module_load_include('admin.inc', 'scratchpads_messages');

    $expires = drush_get_option('expires', null);

    $values = array(
      'message' => $message,
      'expire' => $expires,
      'created' => 'now'
      'type' => 3
    );

    scratchpads_messages_add_form_submit(array(), array("values" => $values));
  }
}

/**
 * Clear global messages
 * drush_[COMMAND_NAME]().
 */
function drush_clear_messages($message) {
  if(module_exists('scratchpads_messages')){
    // Find all global messages
    $messages = db_select('scratchpads_messages_message', 's')->fields('s')->condition('type', 3)->execute();

    $get_id = function ($row) {
      return $row['mid']
    }

    module_load_include('admin.inc', 'scratchpads_messages');

    $values = array(
      'messages' => array_map($messages, $get_id)
    );

    // Clear messages and their related records
    scratchpads_messages_multiple_delete_confirm_submit(array(), array("values" => $values));
  }
}
