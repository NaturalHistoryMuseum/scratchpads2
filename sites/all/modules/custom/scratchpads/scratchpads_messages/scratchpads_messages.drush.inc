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

  return $commands;
}

/**
 * Drush command logic.
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
      'type' => 1
    );

    scratchpads_messages_add_form_submit(array(), array("values" => $values));
  }
}
