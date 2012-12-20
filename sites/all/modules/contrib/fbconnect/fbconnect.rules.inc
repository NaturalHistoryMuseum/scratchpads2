<?php

/**
 * @file
 * Rules integration for the fbconnect module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function fbconnect_rules_event_info() {
  return array(
    'fbconnect_user_registered' => array(
      'label' => t('User has connected his Facebook Account'),
      'module' => 'fbconnect',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('granted facebook auth user'),
        ),
      ),
    ),
    'fbconnect_user_unregistered' => array(
      'label' => t('User has disconnected his Facebook Account'),
      'module' => 'fbconnect',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('revoked facebook auth user'),
        ),
      ),
    ),
    'fbconnect_user_login' => array(
      'label' => t('User has logged in via Facebook'),
      'module' => 'fbconnect',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('logged in via facebook user'),
        ),
      ),
    ),
  );
}

/**
 * @}
 */
