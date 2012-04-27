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