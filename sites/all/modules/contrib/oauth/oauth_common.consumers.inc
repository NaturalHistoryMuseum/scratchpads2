<?php
/**
 * @file
 */

/**
 * Menu system callback for listing a user's consumers.
 *
 * @param object $account
 */
function oauth_common_page_user_consumers($account) {
  module_load_include('inc', 'oauth_common');

  $ci = oauth_common_user_consumers($account->uid);
  $header = array(
    array('data' => t('Name'),       'class' =>  array('oauth-common-consumer-name')),
    array('data' => t('Key'),        'class' =>  array('oauth-common-consumer-key')),
    array('data' => t('Created'),    'class' =>  array('oauth-common-consumer-created')),
    array('data' => t('Operations'), 'class' =>  array('oauth-common-consumer-operations')),
  );
  $rows = array();

  foreach ($ci as $consumer) {
    $data = array(
      'name' => array(
        'data'  => $consumer->name,
        'class' =>  array('oauth-common-consumer-name'),
      ),
      'key' => array(
        'data'  => substr($consumer->key, 0, 6) . '...',
        'class' =>  array('oauth-common-consumer-key'),
      ),
      'created' => array(
        'data'  => format_date($consumer->created),
        'class' =>  array('oauth-common-consumer-created'),
      ),
    );
    $operations = array();

    if (oauth_common_can_edit_consumer($consumer)) {
      $operations[] = array(
        'title' => t('Edit'),
        'href'  => sprintf('user/%d/oauth/consumer/%s', $account->uid, $consumer->csid),
      );
      $operations[] = array(
        'title' => t('Delete'),
        'href'  => sprintf('user/%d/oauth/consumer/%s', $account->uid, $consumer->csid) . '/delete',
      );
    }

    $rows[] = array(
      'data' => $data + array(
        'operations' => array(
          'data'  => theme('links', array('links' => $operations)),
          'class' =>  array('oauth-common-consumer-operations'),
        ),
      ),
      'class' =>  array('oauth-common-consumer'),
    );
  }

  $table = theme('table', array(
    'header' => $header,
    'rows'   => $rows,
    'attributes' => array('id' => 'oauth-common-list-consumers')
  ));

  return $table;
}

/**
 * Menu system callback for the add consumer page.
 */
function oauth_common_add_consumer($account) {
  $consumer = new DrupalOAuthConsumer(user_password(32), user_password(32), array(
    'callback_url' => '',
    'uid' => $account->uid,
    'provider_consumer' => TRUE,
  ));
  return drupal_get_form('oauth_common_form_consumer', $consumer);
}

/**
 * Menu system callback for the edit consumer page.
 */
function oauth_common_edit_consumer($consumer) {
  return drupal_get_form('oauth_common_form_consumer', $consumer);
}

/**
 * The consumer form that is shared by the add and edit page.
 */
function oauth_common_form_consumer($form, &$form_state, $consumer) {
  $form = array();

  $form['consumer_object'] = array(
    '#type' => 'value',
    '#value' => $consumer,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer name'),
    '#required' => TRUE,
    '#default_value' => $consumer->name,
  );

  $form['callback_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Callback url'),
    '#required' => FALSE,
    '#default_value' => $consumer->callback_url,
    '#description' => t('You must include a schema for this to work correctly, ie. http:// or iphoneappname://'),
  );

  if ($consumer->in_database) {
    $contexts = oauth_common_context_list();
    $form['context'] = array(
      '#type' => 'item',
      '#title' => t('Application context'),
      '#markup' => isset($contexts[$consumer->context]) ? $contexts[$consumer->context] : $consumer->context,
    );
  }
  else {
    $allowed_contexts = array();
    foreach (oauth_common_context_list() as $context => $title) {
      if (user_access(sprintf('oauth register consumers in %s', $context))) {
        $allowed_contexts[$context] = $title;
      }
    }

    $form['context'] = array(
      '#type' => 'select',
      '#title' => t('Application context'),
      '#options' => $allowed_contexts,
      '#default_value' => $consumer->context,
    );
  }

  if ($consumer->in_database) {
    $form['key'] = array(
      '#type' => 'item',
      '#title' => t('Key'),
      '#markup' => $consumer->key,
    );

    $form['secret'] = array(
      '#type' => 'item',
      '#prefix' => '<div id="consumer-secret-wrapper">',
      '#title' => t('Secret'),
      '#markup' => substr($consumer->secret, 0, 6) . '...',
    );

    $form['show_secret'] = array(
      '#type' => 'button',
      '#value' => t('Show secret'),
      '#ajax' => array(
        'callback' => 'oauth_common_form_consumer_secret_ajax_callback',
        'wrapper' => 'consumer-secret-wrapper',
      ),
      '#suffix' => '</div>',
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * AJAX callback for showing secrets
 *
 * @return void
 */
function oauth_common_form_consumer_secret_ajax_callback($form, $form_state) {
  $form['secret']['#markup'] = $form_state['values']['consumer_object']->secret;
  $form['secret']['#prefix'] = '';
  return $form['secret'];
}

/**
 * Validate handler for oauth_common_form_consumer
 */
function oauth_common_form_consumer_validate($form, &$form_state) {
  $callback = $form_state['values']['callback_url'];
  if (!parse_url($callback) || strstr($callback, '://') === FALSE) {
    form_set_error('callback_url', t('You must set a valid URL as a callback - http/https://some-path or custom://some-path'));
  }
}

/**
 * Submit handler for oauth_common_form_consumer.
 */
function oauth_common_form_consumer_submit($form, &$form_state) {
  $values = $form_state['values'];
  $consumer = $values['consumer_object'];

  // Translate empty callback url to oob (out of band).
  if (empty($values['callback_url'])) {
    $values['callback_url'] = 'oob';
  }

  if(substr($values['callback_url'], strlen($values['callback_url']) - 2) != '//') {
   // Remove trailing slash
   $values['callback_url'] = rtrim($values['callback_url'], '/');
  }

  // Transfer editable attributes to the consumer.
  $names = array('name', 'callback_url', 'context');
  foreach ($names as $name) {
    if (isset($values[$name])) {
      $consumer->$name = $values[$name];
    }
  }

  // Update or create the consumer.
  $update = $consumer->in_database;
  $consumer->write();

  if ($update) {
    drupal_set_message(t('Updated the consumer @name', array('@name' => $values['name'])));
  }
  else {
    drupal_set_message(t('Added the consumer @name', array('@name' => $values['name'])));
  }

  drupal_goto(sprintf('user/%d/oauth/consumers', $consumer->uid));
}

/**
 * Consumer deletion form.
 */
function oauth_common_form_consumer_delete($form, &$form_state, $consumer) {
  $form = array(
    'consumer_object' => array(
      '#type' => 'value',
      '#value' => $consumer,
    ),
    'confirm' => array(
      '#type' => 'item',
      '#markup' => t('Are you sure you want to delete application <strong>@a</strong>?', array('@a' => $consumer->name)),
    ),
    'actions' => array(
      '#type' => 'actions',
      'delete' => array(
        '#type' => 'submit',
        '#title' => t('Delete'),
        '#default_value' => t('Delete'),
      )
    ),
  );

  return $form;
}

/**
 * Submit handler for oauth_common_form_consumer_delete.
 */
function oauth_common_form_consumer_delete_submit($form, &$form_state) {
  $consumer = $form_state['values']['consumer_object'];

  $consumer->delete();
  drupal_set_message(t('Deleted the consumer @name', array('@name' => $consumer->name)));

  drupal_goto(sprintf('user/%d/oauth/consumers', $consumer->uid));
}
