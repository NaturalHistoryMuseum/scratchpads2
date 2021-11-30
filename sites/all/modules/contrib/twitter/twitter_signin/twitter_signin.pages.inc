<?php
/**
 * @file
 * Page callbacks for Twitter Signin module.
 */

/**
 * Settings form for Twitter Signin.
 */
function twitter_signin_admin_settings($form, &$form_state) {
  $form = array();

  $form['twitter_signin_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a button to user login form'),
    '#default_value' => variable_get('twitter_signin_login', TRUE),
    '#description' => t('Only do this if the site has another link to the <a href="!link">redirect page</a>.', array('!link' => url('twitter/redirect'))),
  );

  $img_path = drupal_get_path('module', 'twitter_signin') . '/images';
  $options = array();
  foreach (file_scan_directory($img_path, '/.png/') as $image) {
    $options[$image->filename] = theme('image', array('path' => $image->uri));
  }
  $options['just-text'] = t('Just the text "Sign in with Twitter"');

  $form['twitter_signin_button'] = array(
    '#type' => 'radios',
    '#title' => t('Select sign-in button'),
    '#options' => $options,
    '#default_value' => variable_get('twitter_signin_button', 'Sign-in-with-Twitter-lighter-small.png'),
    '#states' => array(
      'visible' => array(
        'input[name="twitter_signin_login"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['twitter_signin_register'] = array(
    '#title' => t('Automatically register new users'),
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#default_value' => variable_get('twitter_signin_register', 0),
    '#description' => t('Warning, if you enable this, new user accounts will be created without email addresses.'),
  );

  $form['twitter_signin_show_password'] = array(
    '#title' => t('Show password to new users'),
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#default_value' => variable_get('twitter_signin_show_password', 1),
    '#description' => t('When a new user account is created, display their password in a message on the page.'),
    '#states' => array(
      'visible' => array(
        'input[name="twitter_signin_register"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['twitter_signin_maintenance_mode'] = array(
    '#title' => t('Allow signins when site in maintenance mode'),
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#default_value' => variable_get('twitter_signin_maintenance_mode', 0),
    '#description' => t('This only affect administrators who would be able to log into their Drupal account using their Twitter account, non-admin users will be unable to do anything.'),
  );

  return system_settings_form($form);
}
