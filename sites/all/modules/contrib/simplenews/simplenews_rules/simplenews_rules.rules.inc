<?php

/**
 * @file
 * Rules hooks for the Simplenews newsletter module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function simplenews_rules_rules_action_info() {
  return array(
    'simplenews_rules_action_send' => array(
      'label' => t('Send newsletter'),
      'group' => t('Simplenews'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('The newsletter node to be sent.'),
          'description' => t('The newsletter node that should be sent.'),
        )
      )
    ),
    'simplenews_rules_action_subscribe' => array(
      'label' => t('Subscribe an e-mail adress to a newsletter'),
      'group' => t('Simplenews'),
      'named parameter' => TRUE,
      'parameter' => array(
        'mail' => array(
          'type' => 'text',
          'label' => t('E-mail'),
          'description' => t('The e-mail address that should be subscribed.'),
        ),
        'tid' => array(
          'type' => 'integer',
          'label' => t('Simplenews category'),
          'descrption' => t('For which newsletter category the subscription should happen.'),
          'options list' => 'simplenews_category_list',
        ),
        'confirmation' => array(
          'type' => 'integer',
          'label' => t('Confirmation required'),
          'description' => t('Select if a confirmation is required. Default uses the default setting from the chosen newsletter category.'),
          'options list' => 'simplenews_rules_confirmation_list',
          'default value' => SIMPLENEWS_RULES_CONFIRMATION_DEFAULT,
        ),
        'source' => array(
          'type' => 'string',
          'label' => t('Source'),
          'description' => t('A string to identify the source of this subscription'),
          'optional' => TRUE,
        ),
        'source' => array(
          'type' => 'text',
          'label' => t('Source'),
          'description' => t('A string to identify the source of this subscription'),
          'optional' => TRUE,
          'default value' => 'rules',
        ),
        'language' => array(
          'type' => 'token',
          'label' => t('Language'),
          'description' => t('If specified, the language to use for the subscription. Defaults to the default language.'),
          'options list' => 'entity_metadata_language_list',
          'optional' => TRUE,
          'default value' => LANGUAGE_NONE,
        ),
      ),
    ),
    'simplenews_rules_action_unsubscribe' => array(
      'label' => t('Unsubscribe an e-mail adress from a newsletter'),
      'group' => t('Simplenews'),
      'named parameter' => TRUE,
      'parameter' => array(
        'mail' => array(
          'type' => 'text',
          'label' => t('E-mail'),
          'description' => t('The e-mail address that should be unsubscribed.'),
        ),
        'tid' => array(
          'type' => 'integer',
          'label' => t('Simplenews category'),
          'description' => t('For which newsletter category the unsubscription should happen.'),
          'options list' => 'simplenews_category_list',
        ),
        'confirmation' => array(
          'type' => 'integer',
          'label' => t('Confirmation required'),
          'description' => t('Select if a confirmation is required. Default uses the default setting from the chosen newsletter category.'),
          'options list' => 'simplenews_rules_confirmation_list',
          'default value' => SIMPLENEWS_RULES_CONFIRMATION_DEFAULT,
        ),
        'source' => array(
          'type' => 'text',
          'label' => t('Source'),
          'description' => t('A string to identify the source of this subscription'),
          'optional' => TRUE,
          'default value' => 'rules',
        ),
        'language' => array(
          'type' => 'token',
          'label' => t('Language'),
          'description' => t('If specified, the language to use for the subscription. Defaults to the default language.'),
          'options list' => 'entity_metadata_language_list',
          'optional' => TRUE,
          'default value' => LANGUAGE_NONE,
        ),
      ),
    ),
    'simplenews_rules_action_subscription_delete' => array(
      'label' => t('Delete an e-mail address from a newsletter'),
      'group' => t('Simplenews'),
      'named parameter' => TRUE,
      'parameter' => array(
        'mail' => array(
          'type' => 'text',
          'label' => t('E-mail'),
          'description' => t('The e-mail address that should be permanently deleted from the newsletter. Note, use the unsubscribe action to temporarily unsubscribe an email address.'),
        ),
        'tid' => array(
          'type' => 'integer',
          'label' => t('Simplenews category'),
          'description' => t('For which newsletter category the delete should happen.'),
          'options list' => 'simplenews_category_list',
        ),
      ),
    ),
    'simplenews_rules_action_subscriber_delete' => array(
      'label' => t('Delete an e-mail address from all newsletters'),
      'group' => t('Simplenews'),
      'named parameter' => TRUE,
      'parameter' => array(
        'mail' => array(
          'type' => 'text',
          'label' => t('E-mail'),
          'description' => t('The e-mail address that should be permanently deleted from all newsletters. Note, use the unsubscribe action to temporarily unsubscribe an email address.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_event_info().
 */
function simplenews_rules_rules_event_info() {
  return array(
    'simplenews_rules_event_subscribe' => array(
      'label'  => t('A user has been subscribed'),
      'group' => t('Simplenews'),
      'variables'   => array(
        'mail' => array(
          'type' => 'text',
          'label' => t('E-Mail'),
          'description' => t('The e-mail address that has been subscribed.'),
        ),
        'tid' => array(
          'type' => 'integer',
          'label' => t('Simplenews category'),
          'descrption' => t('The newsletter category of the subscription.'),
          'options list' => 'simplenews_category_list',
        ),
      ),
    ),
    'simplenews_rules_event_unsubscribe' => array(
      'label'  => t('A user has been unsubscribed'),
      'group' => t('Simplenews'),
      'variables'   => array(
        'mail' => array(
          'type' => 'text',
          'label' => t('E-mail'),
          'description' => t('The e-mail address that has been subscribed.'),
        ),
        'tid' => array(
          'type' => 'integer',
          'label' => t('Simplenews category'),
          'descrption' => t('The newsletter category of the subscription.'),
          'options list' => 'simplenews_category_list',
        ),
      ),
    ),
  );
}

/**
 * Action implementation, send a newsletter node.
 */
function simplenews_rules_action_send($node) {
  $newsletter = simplenews_newsletter_load($node->nid);
  if ($newsletter && ($newsletter->status != SIMPLENEWS_STATUS_SEND_PENDING || $newsletter->status != SIMPLENEWS_STATUS_SEND_PENDING)) {
    module_load_include('inc', 'simplenews', 'includes/simplenews.mail');
    simplenews_add_node_to_spool($node);
  }
}

/**
 * Action Implementation: Subscribe an e-mail adress to a Simplenews newsletter.
 */
function simplenews_rules_action_subscribe($args, $settings) {
  if ($args['language'] == LANGUAGE_NONE) {
    $args['language'] = NULL;
  }

  $confirmation = simplenews_rules_map_confirmation($args);

  // Pass the call forward.
  simplenews_subscribe_user($args['mail'], $args['tid'], $confirmation, $args['source'], $args['language']);
}

/**
 * Action Implementation: Unsubscribe an e-mail adress to a Simplenews newsletter.
 */
function simplenews_rules_action_unsubscribe($args, $settings) {
  if ($args['language'] == LANGUAGE_NONE) {
    $args['language'] = NULL;
  }

  $confirmation = simplenews_rules_map_confirmation($args);

  // Pass the call forward.
  simplenews_unsubscribe_user($args['mail'], $args['tid'], $confirmation, $args['source'], $args['language']);
}

/**
 * Action Implementation: Delete an email address from a specific Simplenews newsletter
 */
function simplenews_rules_action_subscription_delete($args, $settings) {
  if (!empty($args['mail']) && !empty($args['tid'])){
    simplenews_subscription_delete(array('mail' => $args['mail'], 'tid' => $args['tid']));
  }
}

/**
 * Action Implementation: Delete an email address from all Simplenews newsletters
 */
function simplenews_rules_action_subscriber_delete($args, $settings) {
  $subscriber = simplenews_subscriber_load_by_mail($args['mail']);
  if ($subscriber) {
    simplenews_subscriber_delete($subscriber);
  }
}

/**
 * Map args to the confrmation argument for subscribing/unsubscribing.
 */
function simplenews_rules_map_confirmation($args) {
  switch ($args['confirmation']) {
    case SIMPLENEWS_RULES_CONFIRMATION_YES:
      $confirmation = TRUE;
      break;
    case SIMPLENEWS_RULES_CONFIRMATION_NO:
      $confirmation = FALSE;
      break;
    case SIMPLENEWS_RULES_CONFIRMATION_DEFAULT:
      $account = simplenews_load_user_by_mail($args['mail']);
      $confirmation = simplenews_require_double_opt_in($args['tid'], $account);
      break;
  }
  return $confirmation;
}

/**
 * @}
 */
