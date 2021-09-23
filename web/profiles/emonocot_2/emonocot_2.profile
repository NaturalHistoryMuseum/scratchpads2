<?php

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function emonocot_2_form_install_configure_form_alter(&$form, $form_state){
  // Pre-populate the site name with the server name.
  $form['update_notifications']['update_status_module']['#default_value'] = array();
}