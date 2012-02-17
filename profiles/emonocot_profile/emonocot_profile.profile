<?php

<<<<<<< HEAD
//Empty
=======
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function emoncot_profile_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}
>>>>>>> c1ed120d53399545abf9f9f5765eeae835e8e531
