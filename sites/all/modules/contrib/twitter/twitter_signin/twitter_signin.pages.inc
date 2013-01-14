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

  $img_path = drupal_get_path('module', 'twitter_signin') . '/images';
  $results = file_scan_directory($img_path, '/.png/');

  $options = array();
  foreach ($results as $image) {
    $options[$image->filename] = theme('image', array('path' => $image->uri));
  }

  $form['twitter_signin_button'] = array(
    '#type' =>  'radios',
    '#title' => t('Select sign-in button'),
    '#options' => $options,
    '#default_value' => variable_get('twitter_signin_button', 'Sign-in-with-Twitter-lighter-small.png'),
  );

  $form['twitter_signin_register'] = array(
    '#title' => t('Automatically register new users'),
    '#type' => 'radios',
    '#options' => array(t('No'), t('Yes')),
    '#default_value' => variable_get('twitter_signin_register', 0),
    '#description' => t('Warning, if you enable this, new user accounts will be created without email addresses.'),
  );

  return system_settings_form($form);
}
