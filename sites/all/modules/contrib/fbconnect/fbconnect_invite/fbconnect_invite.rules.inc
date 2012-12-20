<?php

/**
 * @file
 * Rules integration for the fbconnect_invite module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function fbconnect_invite_rules_event_info() {
  return array(
    'fbconnect_invite_accepted' => array(
      'label' => t('Invitee User has accepted the invite'),
      'module' => 'fbconnect_invite',
      'arguments' => array(
        'inviter' => array(
          'type' => 'user',
          'label' => t('inviter'),
        ),
      ),
    ),
  );
}

/**
 * @}
 */
