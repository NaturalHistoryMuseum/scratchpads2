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
    'aliases' => array('msg-clear', 'msg-clr'),
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
function drush_scratchpads_messages_set_message(...$message_parts) {
  // You should quote the command arg but in case someone forgets...
  $message = implode(' ', $message_parts);

  if(module_exists('scratchpads_messages')){
    module_load_include('admin.inc', 'scratchpads_messages');

    $expires = drush_get_option('expires', null);

    $values = array(
      'message' => $message,
      'expire' => $expires,
      'created' => 'now',
      'type' => 3
    );

    $mid = scratchpads_messages_add_message($values);

    if($mid) {
      drush_log("Created message with id $mid", "ok");
    }else {
      drush_log("Could not create message\n", "error");
    }

    return (bool)$mid;
  }
}

/**
 * Clear global messages
 * drush_[COMMAND_NAME]().
 */
function drush_scratchpads_messages_clear_messages() {
  if(module_exists('scratchpads_messages')){
    // Find all global messages
    $messages = db_select('scratchpads_messages_message', 's')->fields('s')->condition('type', 3)->execute()->fetchAll();

    if (count($messages) == 0) {
      drush_log("Nothing to delete", "ok");
      return;
    }

    module_load_include('admin.inc', 'scratchpads_messages');

    // Function to get the records' ids when we map
    $get_id = function ($row) {
      return $row->mid;
    };

    // Clear messages and their related records
    $result = scratchpads_messages_delete_messages(array_map($get_id, $messages));

    drush_log("Deleted {$result['messages']} global messages, {$result['viewed']} viewed, {$result['roles']} roles", "ok");
  }
}
