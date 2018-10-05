<?php
// If we're installing via Drush/Aegir, we'll have a few options that we can
// set here.
$mail = array();
if(function_exists('drush_get_option')){
  // Get the options at the start. This differs based on the version of Aegir
  // we are using (Aegir 6.x-1.x works with drush_get_option, whilst Aegir
  // 7.x-3.x works with drush_get_context().
  if(($site_title = drush_get_option('site_title', FALSE)) != FALSE){
    $client_name = drush_get_option('fullname', 'Scratchpad Maintainer');
    $client_email = drush_get_option('client_email', 'scratchpads@nhm.ac.uk');
    $client_title = drush_get_option('client_title', 'Dr');
    $client_country = drush_get_option('country', 'United Kingdom');
    $client_institution = drush_get_option('institution', 'Jolly Nice Museum');
  }else{
    $alias = drush_get_context('alias');

    // FIXME: For some reason this data is no longer available in the alias config.
    // Instead we make a backup of it in hosting_scratchpads_hosting_site_context_options
    // (hosting_scratchpads repo) so that it can be used here.
    // This is quite hacky but at least it works.
    if(!isset($alias['site_data'])) {
      $aliasname = '@' . $alias['#name'];
      $alias_extrainfo = drush_sitealias_get_record($aliasname.".site_data");
      $alias['site_data'] = $alias_extrainfo['site_data'];

      // Delete the extra config file as we no longer need it and it will mess up the alias list
      $paths = drush_sitealias_alias_path();
      foreach ($paths as $p) {
        $path = $p . "/" . $alias['#name'] . ".site_data.alias.drushrc.php";
        if(file_exists($path)){
          unlink($path);
        }
      }
    }

    $site_data = $alias['site_data'];
    $site_title = !empty($site_data['site_title']) ? $site_data['site_title'] : FALSE;
    $client_name = !empty($site_data['client_name']) ? $site_data['client_name'] : 'Scratchpads Maintainer';
    $client_email = !empty($site_data['client_email']) ? $site_data['client_email'] : 'scratchpads@nhm.ac.uk';
    $client_title = !empty($site_data['client_title']) ? $site_data['client_title'] : 'Mr';
    $client_country = !empty($site_data['client_country']) ? $site_data['client_country'] : 'GB';
    $client_institution = !empty($site_data['client_institution']) ? $site_data['client_institution'] : 'Jolly Nice Museum';
    $site_taxonomic_scope = !empty($site_data['site_taxonomic_scope']) ? $site_data['site_taxonomic_scope'] : 'Life';
  }
  if($site_title){
    variable_set('site_name', $site_title);
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
    $account->name = $client_name;
    $account->mail = $client_email;
    $account->status = 1;
    $account->roles = array(
      5 => TRUE
    );
    $account->pass = user_hash_password(uniqid());
    $account->field_user_title[LANGUAGE_NONE][] = array(
      'value' => $client_title
    );
    // Can the following be removed? What is it doing there?  Need to test it's
    // actually not doing anything.
    drupal_write_record('legal_accepted', $record);
    // Try to set the country based on what was entered in the drop down.
    $row = db_select('countries_country', 'c')->fields('c', array(
      'cid'
    ))->condition('name', $client_country)->execute()->fetch();
    if($row && count($row)){
      $account->field_user_country[LANGUAGE_NONE][] = countries_load($row->cid);
    }
    $name_parts = explode(' ', $account->name);
    if(count($name_parts) > 1){
      $account->field_user_family_name[LANGUAGE_NONE][] = array(
        'value' => array_pop($name_parts)
      );
    }
    $account->field_user_given_names[LANGUAGE_NONE][] = array(
      'value' => implode(' ', $name_parts)
    );
    if($client_institution){
      $account->field_user_institution[LANGUAGE_NONE][] = array(
        'value' => $client_institution
      );
    }
    user_save($account);
    $onetime = user_pass_reset_url($account);
    // Finally, we uninstall the update module which is annoying, and not actually
    // required.
    module_disable(array(
      'update'
    ), FALSE);
    $mail['subject'] = st('Your new site !site has been created.');
    $mail['body'] = st("Dear !username,

!uri

Your new Scratchpad \"!site\" has been created for you.

You can now login to !site by clicking on this link or copying it into your browser:

!onetime

This is a one-time login, so it can be used only once.

Once you have logged in, you will need to set your password by visiting:

!edit_uri

If you experience any problems help is available on the Scratchpad help wiki [1] and by using the \"Issues...\" tab once logged into your Scratchpad.

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
    return;
  }
}

if (!isset($mail['subject'])) {
  $mail['subject'] = st('Your new site !site has been created.');
  $mail['body'] = st("!username,

Your new site !site has been created.

You can now login to !site by clicking on this link or copying it into your browser:

!login_url

This is a one-time login, so it can be used only once.

Once you have logged in, you will be redirected to !edit_uri so you can change your password.");
}
