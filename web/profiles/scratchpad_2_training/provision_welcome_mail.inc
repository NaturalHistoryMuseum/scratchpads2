<?php
$account = user_load(1);
$account->name = 'Scratchpad Team';
$account->mail = 'scratchpad@nhm.ac.uk';
user_save($account);
// Ensure this user can not login directly.
db_update('users')->fields(array(
  'pass' => ''
))->condition('uid', 1)->execute();
$mail = array();
// Uninstall the scratchpads_statistics module that is not needed on this site.
module_disable(array(
  'scratchpads_statistics'
));
$msg = st('Training site !url ready', array(
  '!url' => url('', array(
    'absolute' => TRUE
  ))
));
$mail = array(
  'subject' => $msg,
  'body' => "$msg

Simon
--
http://scratchpads.org/"
);
