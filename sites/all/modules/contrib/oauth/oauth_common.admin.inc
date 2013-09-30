<?php
/**
 * @file
 * Administration pages for OAuth module.
 */

function _oauth_common_admin() {
  $form = array();

  $form['oauth_common_enable_provider'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the oauth provider'),
    '#default_value' => variable_get('oauth_common_enable_provider', TRUE),
    '#description' => t('This option controls whether this site should act as a OAuth provider or not'),
  );

  $form['oauth_common_request_token_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Request token lifetime (in seconds)'),
    '#default_value' => variable_get('oauth_common_request_token_lifetime', 7200),
  );

  $form['oauth_common_login_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Login page'),
    '#description' => t('Specify an alternative login page. This is useful when, for example, you want to show a mobile-enhanced login page.'),
    '#default_value' => variable_get('oauth_common_login_path', OAUTH_COMMON_LOGIN_PATH),
  );

  $form['#validate'][] = '_oauth_common_admin_settings_validate';

  return system_settings_form($form);
}

function _oauth_common_admin_settings_validate($form, $form_state) {
  $values = $form_state['values'];

  $lifetime = intval($values['oauth_common_request_token_lifetime'], 10);
  if (!$lifetime) {
    form_set_error('oauth_common_request_token_lifetime', t('The request token lifetime must be a non-zero integer value.'));
  }
}

/**
 * Output a list of contexts.
 */
function oauth_common_list_context($js = NULL) {
  $header = array(
    array('data' => t('Title'),      'class' => array('oauth-common-contexts-title')),
    array('data' => t('Storage'),    'class' => array('oauth-common-contexts-storage')),
    array('data' => t('Operations'), 'class' => array('oauth-common-contexts-operations')),
  );

  $contexts = oauth_common_context_load_all();
  $rows = array();

  if (!$contexts) {
    $contexts = array();
  }

  foreach ($contexts as $context) {
    $operations = array();

    if (empty($context->disabled)) {
      $operations[] = array(
        'title' => t('Edit'),
        'href'  => 'admin/config/services/oauth/' . $context->name . '/edit',
      );
      $operations[] = array(
        'title' => t('Export'),
        'href'  => 'admin/config/services/oauth/' . $context->name . '/export',
      );
    }

    if ($context->export_type == (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
      $operations[] = array(
        'title' => t('Revert'),
        'href'  => 'admin/config/services/oauth/' . $context->name . '/delete',
      );
    }
    elseif ($context->export_type != EXPORT_IN_CODE) {
      $operations[] = array(
        'title' => t('Delete'),
        'href'  => 'admin/config/services/oauth/' . $context->name . '/delete',
      );
    }
    elseif (empty($context->disabled)) {
      $operations[] = array(
        'title' => t('Disable'),
        'href'  => 'admin/config/services/oauth/' . $context->name . '/disable',
        'query' => drupal_get_destination(),
      );
    }
    else {
      $operations[] = array(
        'title' => t('Enable'),
        'href'  => 'admin/config/services/oauth/' . $context->name . '/enable',
        'query' => drupal_get_destination(),
      );
    }

    $rows[$context->name] = array(
      'data' => array(
        'title' => array(
          'data'  => check_plain($context->title),
          'class' => array('oauth-common-contexts-title'),
        ),
        'storage' => array(
          'data'  => ($context->export_type == EXPORT_IN_CODE) ? t('In code') : t('In database'),
          'class' => array('oauth-common-contexts-storage'),
        ),
        'operations' => array(
          'data'  => theme('links', array('links' => $operations)),
          'class' => array('oauth-common-contexts-operations'),
        ),
      ),
      'class' => array('oauth-common-contexts-' . $context->name)
    );

    if (!empty($context->disabled)) {
      $rows[$context->name]['class'][] = 'oauth-common-contexts-disabled';
    }
  }

  $table = theme('table', array(
    'header' => $header,
    'rows'   => $rows,
    'attributes' => array('id' => 'oauth-common-list-contexts')
  ));

  return $table;
}

/**
 * Handle the add context page.
 */
function oauth_common_add_context() {
  $context = oauth_common_context_new();
  drupal_set_title(t('Add context'));
  if (!$context) {
    drupal_set_message(t("Can't create contexts, check that you've installed !ctools.", array(
      '!ctools' => l('Chaos tool suite', 'http://drupal.org/project/ctools'),
    )), 'error');
    $result = '';
  }
  else {
    $result = oauth_common_edit_context($context);
  }
  return $result;
}

/**
 * Edit a context.
 *
 * Called from both the add and edit points to provide for common flow.
 */
function oauth_common_edit_context($context) {
  if (!is_object($context)) {
    $context = oauth_common_context_load($context);
  }
  if ($context && !empty($context->title)) {
    drupal_set_title($context->title);
  }
  return drupal_get_form('oauth_common_edit_form_context', $context);
}

/**
 * Form to edit the settings of a context.
 */
function oauth_common_edit_form_context($form, &$form_state, $context) {
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'oauth_common') . '/css/admin.css'
  );

  $form['cid'] = array(
    '#type'  => 'value',
    '#value' => isset($context->cid) ? $context->cid : '',
  );

  $form['context_object'] = array(
    '#type'  => 'value',
    '#value' => $context,
  );

  $form['title'] = array(
    '#type'          => 'textfield',
    '#size'          => 24,
    '#maxlength'     => 100,
    '#default_value' => $context->title,
    '#title'         => t('Context title'),
    '#required'      => TRUE,
  );

  $form['name'] = array(
    '#type'          => 'machine_name',
    '#size'          => 24,
    '#maxlength'     => 32,
    '#default_value' => $context->name,
    '#title'         => t('Context name'),
    '#description'   => t('A unique name used to identify this preset internally. It ' .
                          'must be only alpha characters and underscores. No spaces, ' .
                          'numbers or uppercase characters.'),
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => 'oauth_common_edit_form_context_exists',
    ),
    '#required'      => TRUE,
  );

  $sign_methods = array(
    'PLAINTEXT' => t('Plaintext'),
  );
  foreach (hash_algos() as $algo) {
    $sign_methods['HMAC-' . strtoupper($algo)] = 'HMAC-' . strtoupper($algo);
  }

  $form['signature_methods'] = array(
    '#type' => 'fieldset',
    '#title' => t('Signature methods'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    'selected' => array(
      '#type' => 'checkboxes',
      '#title' => t('Supported signature methods'),
      '#options' => $sign_methods,
      '#default_value' => !empty($context->authorization_options['signature_methods']) ?
         $context->authorization_options['signature_methods'] :
         array('HMAC-SHA1', 'HMAC-SHA256', 'HMAC-SHA384', 'HMAC-SHA512'),
    )
  );

  $form['authorization_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authorization options'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['authorization_options']['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#description' => t('The title of the authorization page.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => empty($context->authorization_options['page_title']) ? '' : $context->authorization_options['page_title'],
  );

  $form['authorization_options']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('The message shown to the user when authorizing.'),
    '#default_value' => empty($context->authorization_options['message']) ? '' : $context->authorization_options['message'],
  );

  $form['authorization_options']['warning'] = array(
    '#type' => 'textarea',
    '#title' => t('Warning'),
    '#description' => t('The warning shown to the user when authorizing.'),
    '#default_value' => empty($context->authorization_options['warning']) ? '' : $context->authorization_options['warning'],
  );

  $form['authorization_options']['deny_access_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Deny access title'),
    '#description' => t('The title of deny access link.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => empty($context->authorization_options['deny_access_title']) ? '' : $context->authorization_options['deny_access_title'],
  );

  $form['authorization_options']['grant_access_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Grant access title'),
    '#description' => t('The title of grant access button.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => empty($context->authorization_options['grant_access_title']) ? '' : $context->authorization_options['grant_access_title'],
  );

  $form['authorization_options']['access_token_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token lifetime'),
    '#description' => t('The time, in seconds, for which an access token should be valid, use 0 to never expire access tokens.'),
    '#size' => 10,
    '#maxlength' => 255,
    '#default_value' => empty($context->authorization_options['access_token_lifetime']) ? 0 : $context->authorization_options['access_token_lifetime'],
  );

  $form['authorization_options']['disable_auth_level_selection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable authorization level selection'),
    '#description' => t('If this is checked the user won\'t be able to choose the authorization level, and the default authorization level(s) will be used.'),
    '#default_value' => !empty($context->authorization_options['disable_auth_level_selection']),
  );

  $form['authorization_levels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authorization levels'),
    '#tree' => TRUE,
    '#prefix' => '<div id="auth-level-wrapper">',
    '#suffix' => '</div>',
    'add_authorization_level' => array(
      '#type' => 'submit',
      '#value' => t('Add authorization level'),
      '#weight' => 10,
      '#submit' => array('oauth_common_edit_form_auth_level_ajax_add'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'oauth_common_edit_form_auth_level_ajax_callback',
        'wrapper' => 'auth-level-wrapper',
      )
    ),
  );

  foreach ($context->authorization_levels as $name => $level) {
    $title = !empty($name) ? check_plain($name) : t('New level');
    if ($title == '*') {
      $title = t('Full access');
    }
    $l = oauth_common_edit_form_auth_level($context, $title, $name, $level);
    $form['authorization_levels'][] = $l;
  }

  if (!isset($form_state['authorization_level_new'])) {
    $form_state['authorization_level_new'] = 0;
  }
  for ($i = 0; $i < $form_state['authorization_level_new']; $i++) {
    $form['authorization_levels'][] = oauth_common_edit_form_auth_level($context, t('Authorization level'));
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Helper function for constructing an auth level fieldset.
 *
 * @param object $context
 * @param int $idx
 * @param string $title
 * @param string $name
 * @param array $level
 * @return array.
 */
function oauth_common_edit_form_auth_level($context, $title, $name = '', $level = array()) {
  $level = $level + array(
    'title' => '',
    'description' => '',
  );

  $element = array(
    "name" => array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('The name of the authorization level.'),
      '#size' => 40,
      '#maxlength' => 32,
      '#default_value' => $name,
    ),
    "title" => array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('The title of the authorization level.'),
      '#size' => 40,
      '#maxlength' => 100,
      '#default_value' => $level['title'],
    ),
    "default" => array(
      '#type' => 'checkbox',
      '#title' => t('Selected by default'),
      '#description' => t('Whether the authentication level should be checked by default.'),
      '#default_value' => is_array($context->authorization_options['default_authorization_levels']) && in_array($name, $context->authorization_options['default_authorization_levels']),
    ),
    "delete" => array(
      '#type' => 'checkbox',
      '#title' => t('Delete'),
      '#description' => t('Check this to delete the authorization level.'),
      '#default_value' => FALSE,
    ),
    "description" => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#description' => t('The description of the authorization level.'),
      '#default_value' => $level['description'],
    ),
  );
  return $element;
}

/**
 * Submit handler for adding auth levels
 *
 * @return void
 */
function oauth_common_edit_form_auth_level_ajax_add($form, &$form_state) {
  $form_state['authorization_level_new']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback for dealing with auth levels
 *
 * @return void
 */
function oauth_common_edit_form_auth_level_ajax_callback($form, $form_state) {
  return $form['authorization_levels'];
}

/**
 * Check whether a given context exists.
 *
 * @param $name the name parameter for the context.
 * @return boolean
 */
function oauth_common_edit_form_context_exists($name) {
  $exists = FALSE;

  if (!empty($name)) {
    $result = db_query('SELECT cid FROM {oauth_common_context} WHERE name = :name', array(
      ':name' => $name
    ))->fetchField();
    $exists = $result ? TRUE : FALSE;
  }

  return $exists;
}

/**
 * Validate submission of the preset edit form.
 */
function oauth_common_edit_form_context_validate(&$form, &$form_state) {
  $values = $form_state['values'];

  // Check that the authorization level names are unique within the context
  $levels = array();
  $default_exists = FALSE;
  foreach ($values['authorization_levels'] as $key => $level) {
    if (is_numeric($key) && !empty($level['name']) && !$level['delete']) {
      if (!empty($levels[$level['name']])) {
        form_error($form['authorization_levels'][$key]['name'], t('Authorization level name must be unique.'));
      }
      else if (preg_match("/[^A-Za-z0-9_\*]/", $level['name'])) {
        form_error($form['authorization_levels'][$key]['name'],
          t('Authorization level name must be alphanumeric or underscores only.'));
      }
      if (empty($level['title'])) {
        form_error($form['authorization_levels'][$key]['title'], t('Authorization levels must have a title.'));
      }
      $default_exists = $default_exists || $level['default'];
      $levels[$level['name']] = TRUE;
    }
  }

  // Check that we actually got a number as access token lifetime
  if (!is_numeric($values['authorization_options']['access_token_lifetime'])) {
    form_error($form['authorization_options']['access_token_lifetime'], t('The access token lifetime must be numeric.'));
  }

  // Check that at least one default authorization level is checked when
  // authorization level selection is disabled.
  if (!$default_exists && $values['authorization_options']['disable_auth_level_selection']) {
    form_error($form['authorization_options']['disable_auth_level_selection'],
      t('You must select at least one default authorirization level if level selection is disabled.'));
  }
}

/**
 * Process submission of the mini panel edit form.
 */
function oauth_common_edit_form_context_submit($form, &$form_state) {
  $context = $form_state['values']['context_object'];
  $values = $form_state['values'];

  $context->name = $values['name'];
  $context->title = $values['title'];

  $auth_options = array(
    'access_token_lifetime' => 0,
  );

  foreach ($values['authorization_options'] as $key => $value) {
    $auth_options[$key] = empty($value) ? null : $value;
  }
  $context->authorization_options = $auth_options;

  // Collect the names of the selected signature methods.
  $sig_options = array();
  foreach ($values['signature_methods']['selected'] as $name => $selected) {
    if ($selected) {
      $sig_options[] = $name;
    }
  }
  $context->authorization_options['signature_methods'] = $sig_options;

  // Set the auth levels and default levels for the context
  $levels = array();
  $default_levels = array();
  foreach ($values['authorization_levels'] as $key => $level) {
    if (is_numeric($key) && !empty($level['name']) && !$level['delete']) {
      $name = $level['name'];
      if ($level['default']) {
        $default_levels[] = $name;
      }
      $levels[$name] = $level;
    }
  }
  $context->authorization_levels = $levels;
  $context->authorization_options['default_authorization_levels'] = $default_levels;

  oauth_common_context_save($context);

  if (empty($context->cid)) {
    drupal_set_message(t('Your new context %title has been saved.', array('%title' => $context->title)));
    $form_state['values']['cid'] = $context->cid;
  }
  else {
    drupal_set_message(t('Your changes have been saved.'));
  }

  $form_state['redirect'] = 'admin/config/services/oauth';
}

/**
 * Provide a form to confirm deletion of a context.
 */
function oauth_common_delete_confirm_context($form, &$form_state, $context) {
  if (!is_object($context)) {
    $context = oauth_common_context_load($context);
  }
  if ($context->export_type == (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
    $title  = t('Are you sure you want to revert the context "@title"?', array('@title' => $context->title));
    $submit = t('Revert');
  }
  elseif ($context->export_type != EXPORT_IN_CODE) {
    $title  = t('Are you sure you want to delete the context "@title"?', array('@title' => $context->title));
    $submit = t('Delete');
  }
  else {
    drupal_not_found();
    die;
  }
  $form['context'] = array('#type' => 'value', '#value' => $context->name);
  $form['cid']      = array('#type' => 'value', '#value' => $context->cid);
  return confirm_form($form,
    $title,
    !empty($_GET['destination']) ? $_GET['destination'] : 'admin/config/services/oauth',
    t('This action cannot be undone.'),
    $submit, t('Cancel')
  );
}

/**
 * Handle the submit button to delete a context.
 */
function oauth_common_delete_confirm_context_submit($form, &$form_state) {
  $context = oauth_common_context_load($form_state['values']['context']);
  if ($context->cid == $form_state['values']['cid']) {
    oauth_common_context_delete($context);
    $form_state['redirect'] = 'admin/config/services/oauth';
  }
}

/**
 * Page callback to export a context to PHP code.
 */
function oauth_common_export_context($form, &$form_state, $context) {
  if (!is_object($context)) {
    $context = oauth_common_context_load($context);
  }
  drupal_set_title($context->title);
  $code = oauth_common_context_export($context);

  $lines = substr_count($code, "\n") + 4;
  $form['code'] = array(
    '#type'          => 'textarea',
    '#title'         => $context->title,
    '#default_value' => $code,
    '#rows'          => $lines,
  );

  return $form;
}
