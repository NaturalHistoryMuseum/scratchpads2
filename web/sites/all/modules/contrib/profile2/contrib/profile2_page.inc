<?php

/**
 * @file
 * Adds separate pages for viewing and editing profiles.
 */

/**
 * Shows the profile page for the current user.
 *
 * @see user_page()
 */
function profile2_page_own($base_path) {
  global $user;
  if ($user->uid) {
    menu_set_active_item($base_path . '/' . $user->uid);
    return menu_execute_active_handler(NULL, FALSE);
  }
  else {
    return drupal_get_form('user_login');
  }
}

/**
 * Profile view page.
 */
function profile2_page_view($profile) {
  drupal_set_title($profile->label());
  return $profile->view('page');
}

/**
 * The profile edit form.
 */
function profile2_form($form, &$form_state, $profile) {
  if (empty($form_state['profiles'])) {
    $form_state['profiles'][$profile->type] = $profile;
  }
  // Prevent invoking the same hooks twice, so tell profile2_attach_form() to
  // skip invoking the hooks.
  $form_state['profile2_skip_hook'] = TRUE;
  profile2_attach_form($form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  if (empty($profile->is_new) && user_access('administer profiles')) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('profile2_form_submit_delete')
    );
  }
  $form['#submit'][] = 'profile2_form_submit';
  return $form;
}

/**
 * Profile form submit handler.
 */
function profile2_form_submit($form, &$form_state) {
  // The profile is being saved by profile2_form_submit_handler().
  drupal_set_message(t('The changes have been saved.'));
  $form_state['redirect'] = $form_state['profile2']->path();
}

/**
 * Profile form submit handler for the delete button.
 */
function profile2_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = $form_state['profile2']->path() . '/delete';
}

/**
 * Confirm form for deleting a profile.
 */
function profile2_page_delete_confirm_form($form, &$form_state, $profile) {
  $form_state += array('profile2' => $profile);
  $confirm_question = t('Are you sure you want to delete profile %label?', array('%label' => $profile->label));
  return confirm_form($form, $confirm_question, $profile->path());
}

function profile2_page_delete_confirm_form_submit($form, &$form_state) {
  $profile = $form_state['profile2'];
  $profile->delete();
  drupal_set_message(t('Deleted %label.', array('%label' => $profile->label)));
  $form_state['redirect'] = 'user/' . $profile->uid;
}
