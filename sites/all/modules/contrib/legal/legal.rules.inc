<?php

/**
 * Implements hook_rules_event_info().
 */
function legal_rules_event_info() {
  return array(
    'legal_accepted' => array(
      'label'     => t('After user accepts terms and conditions'),
      'group'     => t('User'),
      'variables' => array(
        'account'    => array(
          'type'  => 'user',
          'label' => t('updated user'),
        ),
        'conditions' => array(
          'type'  => 'text',
          'label' => t('Terms and Conditions text'),
        ),
        'accepted'   => array(
          'type'  => 'text',
          'label' => t('Timestamp of acceptance'),
        ),
      ),
    ),
  );
}

