<?php

/**
 * Settings page callback
 */
function uwho_site_settings(){
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  return system_settings_form(array(
    'container' => array(
      '#type' => 'fieldset',
      '#title' => '',
      '#description' => t('Enter the following URL and Key into your UWho client on any other site.  These must be copied EXACTLY as you see them here.'),
      '#collapsible' => FALSE,
      'uwho_site_key_fieldset' => array(
        '#type' => 'fieldset',
        '#title' => t('UWho Key'),
        '#collapsible' => FALSE,
        'uwho_site_key' => array(
          '#markup' => '<h2>' . variable_get('uwho_site_key', '') . '</h2>'
        )
      ),
      'uwho_site_url_fieldset' => array(
        '#type' => 'fieldset',
        '#title' => t('UWho URL'),
        '#collapsible' => FALSE,
        'uwho_site_url' => array(
          '#markup' => '<h2>' . url('uwho', array(
            'absolute' => TRUE
          )) . '</h2>'
        )
      )
    ),
    'settings' => array(
      '#title' => t('Settings'),
      '#type' => 'fieldset',
      '#collaspsible' => FALSE,
      'uwho_site_always_switch_user' => array(
        '#type' => 'radios',
        '#title' => t('Switch user when accessing the site via a UWho link'),
        '#default_value' => variable_get('uwho_site_always_switch_user', 1),
        '#options' => array(
          0 => t('No'),
          1 => 'Yes'
        ),
        '#description' => t('If a user is already logged into this site, should UWho change the user account that the user is logged in as?')
      ),
      'uwho_site_roles' => array(
        '#type' => 'checkboxes',
        '#description' => t('Select the roles you would like users created by UWho to have.  All users will of course be "authenticated users"') . (count($roles) ? '' : '<br/><strong>This site currently has no roles defined.</strong>'),
        '#title' => t('Roles'),
        '#default_value' => variable_get('uwho_site_roles', array()),
        '#options' => $roles
      ),
      'uwho_site_redirect_path' => array(
        '#type' => 'textfield',
        '#title' => t('UWho Redirect Path'),
        '#default_value' => variable_get('uwho_site_redirect_path', ''),
        '#description' => t('Enter the Drupal path to where successfully logged in users should be redirected (e.g. "node", "node/add/page", "admin", etc)')
      )
    )
  ));
}