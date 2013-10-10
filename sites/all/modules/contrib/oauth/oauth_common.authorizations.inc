<?php
/**
 * @file
 * Functions related to a user's authorization section
 */

function oauth_common_page_user_authorizations($account) {
  $header = array(
    array('data' => t('Application'), 'class' =>  array("oauth-common-authorization-application")),
    array('data' => t('Key'),         'class' =>  array("oauth-common-authorization-key")),
    array('data' => t('Created'),     'class' =>  array("oauth-common-authorization-created")),
    array('data' => t('Expires'),     'class' =>  array("oauth-common-authorization-expires")),
    array('data' => t('Operations'),  'class' =>  array("oauth-common-authorization-operations")),
  );

  $access_tokens = oauth_common_get_user_provider_tokens($account->uid);
  $rows = array();

  foreach ($access_tokens as $token) {
    $consumer = $token->consumer;

    $data = array(
      'application' => array(
        'data'  => check_plain($consumer->name),
        'class' =>  array("oauth-common-authorization-application"),
      ),
      'key' => array(
        'data'  => substr($token->key, 0, 6) . '...',
        'class' =>  array("oauth-common-authorization-key"),
      ),
      'created' => array(
        'data'  => format_date($token->created),
        'class' =>  array("oauth-common-authorization-created"),
      ),
    );
    $operations = array();

    $operations[] = array(
      'title' => t('Edit'),
      'href'  => sprintf('user/%d/oauth/authorizations/%s', $account->uid, $token->tid),
      'query' => array('destination' => $_GET['q']),
    );

    $operations[] = array(
      'title' => t('Delete'),
      'href'  => sprintf('user/%d/oauth/authorizations/%s', $account->uid, $token->tid) . '/delete',
      'query' => array('destination' => $_GET['q']),
    );

    $data['expires'] = array(
      'data'  => $token->expires ? format_date($token->expires) : t('Never'),
      'class' =>  array("oauth-common-authorization-expires"),
    );

    $rows[] = array(
      'data' => $data + array(
        'operations' => array(
          'data'  => theme('links', array('links' => $operations)),
          'class' =>  array("oauth-common-authorization-operations"),
        ),
      ),
    );
  }

  $table = theme('table', array(
    'header' => $header,
    'rows'   => $rows,
    'attributes' => array('id' => 'oauth-common-list-authorization')
  ));

  return $table;
}

function oauth_common_authorization_add($consumer) {
  $token = new DrupalOAuthToken(user_password(32), user_password(32), $consumer, array(
    'uid' => $account->uid,
  ));
  return drupal_get_form('oauth_common_form_authorization', $token);
}

/**
 * Provide a form to edit and add authorizations. 
 *
 * Despite what appears above, this function is actually invoked by 
 * `drupal_retrieve_form` (by way of `drupal_build_form`, by way of 
 * `drupal_get_form`), so the second argument isn't the token, but a reference 
 * to the form state. Luckily, PHP made that incredibly non-obvious by 
 * neglecting to notify me that this function was being called with one too few 
 * arguments. Go team.
 */
function oauth_common_form_authorization($form_id, &$form_state, $token) {
  $form = array();

  $consumer = $token->consumer;
  $context = oauth_common_context_load($consumer->context);

  drupal_set_title(t('Authorization for @app', array('@app' => $consumer->name)), PASS_THROUGH);

  $form['token_object'] = array(
    '#type' => 'value',
    '#value' => $token,
  );

  $form['authorized'] = array(
    '#type' => 'checkbox',
    '#title' => t('Authorized'),
    '#default_value' => $token->authorized,
  );

  $form['created'] = array(
    '#type' => 'item',
    '#title' => t('Created'),
    '#markup' => format_date($token->created),
  );

  $form['changed'] = array(
    '#type' => 'item',
    '#title' => t('Changed'),
    '#markup' => format_date($token->changed),
  );

  $form['key'] = array(
    '#type' => 'item',
    '#title' => t('Key'),
    '#markup' => $token->key,
  );

  if ($token->in_database) {
    $form['secret'] = array(
      '#type' => 'item',
      '#prefix' => '<div id="token-secret-wrapper">',
      '#title' => t('Secret'),
      '#markup' => substr($token->secret, 0, 6) . '...',
    );

    $form['show_secret'] = array(
      '#type' => 'button',
      '#value' => t('Show secret'),
      '#ajax' => array(
        'callback' => 'oauth_common_form_authorization_secret_ajax_callback',
        'wrapper' => 'token-secret-wrapper',
      ),
      '#suffix' => '</div>',
    );
  }
  else {
    $form['secret'] = array(
      '#type' => 'item',
      '#title' => t('Secret'),
      '#markup' => $token->secret
    );
  }

  $form['allowed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Permissions'),
  );

  global $user;
  oauth_common_permissions_form($user, $form['allowed'], $consumer, $context, $token->services);

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
function oauth_common_form_authorization_secret_ajax_callback($form, $form_state) {
  $form['secret']['#markup'] = $form_state['values']['token_object']->secret;
  $form['secret']['#prefix'] = '';
  return $form['secret'];
}

function oauth_common_permissions_form($account, &$form, $consumer, $context, $default_services = array('*')) {
  $tvars = array(
    '@appname' => $consumer->name,
    '@user' => $account->name,
    '@sitename' => variable_get('site_name', ''),
  );

  if ($context) {
    foreach ($context->authorization_levels as $name => $level) {
      $auth_opt = array(
        '#type' => 'checkbox',
        '#title' => t($level['title'], $tvars),
        '#description' => t($level['description'], $tvars),
        '#default_value' => in_array($name, $default_services),
      );
      $form['authorization']['levels'][$name] = $auth_opt;
    }
  }
}

function oauth_common_form_authorization_submit($form, &$form_state) {
  $values = $form_state['values'];
  $token = $values['token_object'];
  $consumer = $token->consumer;

  // Collect the authorization levels
  if (isset($values['levels'])) {
    $token->services = array_keys(array_filter($values['levels']));
  }

  $token->authorized = $values['authorized'];
  $token->write(TRUE);

  drupal_set_message(t('The @consumer token @token was updated.', array(
    '@consumer' => $consumer->name,
    '@token' => $token->key)));

  drupal_goto(sprintf('user/%d/applications', $token->uid));
}

/**
 * Provide a form for deleting an authorization.
 *
 * We've got the same symptom here that we do with 
 * `oauth_common_form_authorization`, i.e., this is actually called by 
 * `drupal_retrieve_form`.
 */
function oauth_common_form_authorization_delete($form_id, &$form_state, $user, $token) {
  $consumer = $token->consumer;

  $cancel_url = 'user/%d/oauth/authorizations';

  if (!empty($_GET['destination'])) {
    $cancel_url = $_GET['destination'];
  }

  drupal_set_title(t('Deleting authorization for "@consumer"', array(
    '@consumer' => $consumer->name,
  )), PASS_THROUGH);

  $form = array(
    'token_object' => array(
      '#type' => 'value',
      '#value' => $token,
    ),
  );

  $form['user'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  $form['key'] = array(
    '#type' => 'value',
    '#value' => $token->key,
  );

  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t('Are you sure that you want to delete the authorization for "@consumer"?', array(
      '@consumer' => $consumer->name,
    )),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), sprintf($cancel_url, $user->uid, $token->key)),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

function oauth_common_form_authorization_delete_submit($form, &$form_state) {
  $values = $form_state['values'];
  $token = $values['token_object'];
  $consumer = $token->consumer;
  $token->delete();
  drupal_set_message(t('The @consumer token @token was deleted.', array(
    '@consumer' => $consumer->name,
    '@token' => $token->key)));
}
