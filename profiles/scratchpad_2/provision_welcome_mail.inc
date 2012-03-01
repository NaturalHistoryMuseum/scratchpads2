<?php
// We try to set the relevent options using Drush.
drupal_set_message('At the sending of the email for Scratchapds 2 profile');
$data = array();
if(function_exists('drush_get_option')){
  $site_title = drush_get_option('site_title', FALSE);
  drupal_set_message('drush_get_option exists');
  drupal_set_message('Site title is ' . $site_title);
  if($site_title){
    variable_set('site_name', $site_title);
  }
}else{
  drupal_set_message('drush_get_option does not exists');
}
// $Id$
/**
 * @file
 * Define subject and body text for an email to the admin user created
 * for a site. 
 *
 * At the very least you will want to send them the one-time login url, !login_url.
 */
/**
 * Adjust the UID 1. Not ideal doing this here, but we only actually need to do
 * this when being installed via Aegir, and this only gets installed when called
 * by Aegir.
 */
//$account = user_load(1);
//$account->name = 'Scratchpad Team';
//$account->pass = 'no-direct-login';
//$account->mail = 'scratchpad@nhm.ac.uk';
//user_save($account);
/**
 * Here we load account two, as that is the one we want to send the email for.
 */
$account = user_load(2);
$onetime = user_pass_reset_url($account);
// Set the status of account two (it's not set to zero for some reason
//db_query('UPDATE {users} SET status = 1 WHERE uid = 2');
$mail = array();
$mail['subject'] = st('Your new site !site has been created.');
$mail['body'] = st("Dear !username,

!uri

Your new Scratchpad \"!site\" has been created for you

You can now login to !site by clicking on this link or copying it into your browser:

!onetime

This is a one-time login, so it can be used only once.

Once you have logged in, you will need to set your password by visiting !edit_uri

Further help is available on the Scratchpad website [1], or the Issues... tab that is present on all Scratchpads.

The Scratchpad Team

[1] http://scratchpads.eu/help

--
http://scratchpads.eu
scratchpad@nhm.ac.uk", array(
  '!onetime' => $onetime,
  '!username' => $account->name,
  '!edit_uri' => url('user/' . $account->uid . '/edit', array(
    'absolute' => TRUE
  ))
));