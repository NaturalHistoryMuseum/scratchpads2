<?php
// If we're installing via Drush/Aegir, we'll have a few options that we can
// set here.
if(function_exists('drush_get_option')){
  // Check whether or not we need to continue (based on whether or not the
  // site_title is set).
  if(!drush_get_option('site_title', FALSE)){return;}
  // Set the site title
  if($site_title = drush_get_option('site_title', FALSE)){
    variable_set('site_name', $site_title);
  }
  // Adjust the UID 1. Not ideal doing this here, but we only actually need to 
  // do this when being installed via Aegir, and this only gets installed when 
  // called by Aegir.
  $account = user_load(1);
  $account->name = 'Scratchpad Team';
  $account->mail = 'scratchpad@nhm.ac.uk';
  user_save($account);
  // Ensure this user can not login directly.
  db_update('users')->fields(array(
    'pass' => ''
  ))->condition('uid', 1)->execute();
  // Create an account for the user based on the data they entered when applying
  // for a Scratchpad.
  $account = new stdClass();
  $account->is_new = TRUE;
  $account->name = drush_get_option('fullname', 'Scratchpad Maintainer');
  $account->mail = drush_get_option('client_email', 'scratchpad@nhm.ac.uk');
  $account->status = 1;
  $account->roles = array(
    5 => TRUE
  );
  $account->pass = user_hash_password(uniqid());
  $account->field_user_title[LANGUAGE_NONE][] = array(
    'value' => drush_get_option('client_title', 'Dr')
  );
  // Accept the legal conditions.
  $record = array(
    'legal_id' => 1,
    'version' => 1,
    'revision' => 1,
    'language' => 'en',
    'uid' => 2,
    'accepted' => time()
  );
  drupal_write_record('legal_accepted', $record);
  /*
   * FIXME - Unable to set the country correctly.  This simply means a new user
   * will have to set the country value.
   * 
  $account->field_user_country[LANGUAGE_NONE][] = array(
    'value' => drush_get_option('country', 'GB')
  );
  */
  $name_parts = explode(' ', $account->name);
  if(count($name_parts) > 1){
    $account->field_user_family_name[LANGUAGE_NONE][] = array(
      'value' => array_pop($name_parts)
    );
  }
  $account->field_user_given_names[LANGUAGE_NONE][] = array(
    'value' => implode(' ', $name_parts)
  );
  if($instituition = drush_get_option('institution', 'N/A')){
    $account->field_user_institution[LANGUAGE_NONE][] = array(
      'value' => $institution
    );
  }
  user_save($account);
  $onetime = user_pass_reset_url($account);
  // Finally, we uninstall the update module which is annoying, and not actually
  // required.
  module_disable(array(
    'update'
  ), FALSE);
  $mail = array();
  $mail['subject'] = st('Your new site !site has been created.');
  $mail['body'] = st("Dear !username,

!uri

Your new Scratchpad \"!site\" has been created for you.

You can now login to !site by clicking on this link or copying it into your browser:

!onetime

This is a one-time login, so it can be used only once.

Once you have logged in, you will need to set your password by visiting:

!edit_uri

Further help is available on the Scratchpad help wiki [1].

The Scratchpad Team

[1] http://help.scratchpads.eu/

--
http://scratchpads.eu
scratchpad@nhm.ac.uk", array(
    '!site' => variable_get('site_name', 'Scratchpad'),
    '!onetime' => $onetime,
    '!username' => $account->name,
    '!edit_uri' => url('user/' . $account->uid . '/edit', array(
      'absolute' => TRUE
    ))
  ));
}