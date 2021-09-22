<?php

/**
 * @file
 *
 * Menu callbacks and functions for configuring and editing webforms.
 */

/**
 * Main configuration form for editing a webform node.
 */
function webform_configure_form($form, &$form_state, $node) {
  form_load_include($form_state, 'inc', 'webform', 'includes/webform.components');

  $form['#attached']['library'][] = array('webform', 'admin');

  $form['#node'] = $node;

  $form['#submit'] = array(
    'webform_configure_form_submit',
    'webform_configure_form_submit_save',
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  /* Start Edit Form */
  $form['submission'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -4,
  );

  $form['submission']['confirmation'] = array(
    '#type' => 'text_format',
    '#title' => t('Confirmation message'),
    '#description' => t('Message to be shown upon successful submission. If the redirection location is set to <em>Confirmation page</em> it will be shown on its own page, otherwise this displays as a message. Supports Webform token replacements.') . ' ' . theme('webform_token_help', array('groups' => array('node', 'submission'))),
    '#default_value' => $node->webform['confirmation'],
    '#cols' => 40,
    '#rows' => 10,
    '#format' => $node->webform['confirmation_format'],
    '#parents' => array('confirmation'),
  );

  // Redirection settings.
  if (strpos($node->webform['redirect_url'], '<') === 0) {
    $redirect = trim($node->webform['redirect_url'], '<>');
    // Redirection is set to front page.
    if ($redirect == 'front') {
      $redirect = 'url';
      $redirect_url = $node->webform['redirect_url'];
    }
    else {
      $redirect_url = '';
    }
  }
  else {
    $redirect = 'url';
    $redirect_url = $node->webform['redirect_url'];
  }
  $form['submission']['redirection'] = array(
    '#type' => 'item',
    '#title' => t('Redirection location'),
    '#theme' => 'webform_advanced_redirection_form',
    '#description' => t('Choose where to redirect the user upon successful submission.') . ' ' . t('The <em>Custom URL</em> option supports Webform token replacements.') . ' ' . theme('webform_token_help', array('groups' => array('node', 'submission'))),
  );
  $form['submission']['redirection']['redirect']= array(
    '#type' => 'radios',
    '#default_value' => $redirect,
    '#options' => array(
      'confirmation' => t('Confirmation page'),
      'url' => t('Custom URL'),
      'none' => t('No redirect (reload current page)'),
    ),
    '#parents' => array('redirect'),
  );
  $form['submission']['redirection']['redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL'),
    '#description' => t('URL to redirect the user to upon successful submission.'),
    '#default_value' => $redirect_url,
    '#maxlength' => 2048,
    '#parents' => array('redirect_url'),
    '#attributes' => array('title' => t('Custom URL value')),
  );

  // Submission limit settings for all submissions.
  $form['submission']['total_submit_limit'] = array(
    '#type' => 'item',
    '#title' => t('Total submissions limit'),
    '#theme' => 'webform_advanced_total_submit_limit_form',
    '#description' => t('Limit the total number of allowed submissions.'),
  );
  $form['submission']['total_submit_limit']['enforce_total_limit'] = array(
    '#type' => 'radios',
    '#options' => array('no' => t('Unlimited'), 'yes' => 'Limit to !count total submission(s) !timespan'),
    '#default_value' => $node->webform['total_submit_limit'] == -1 ? 'no' : 'yes',
    '#parents' => array('enforce_total_limit'),
  );
  $form['submission']['total_submit_limit']['total_submit_limit'] = array(
    '#type' => 'textfield',
    '#maxlength' => 8,
    '#size' => 8,
    '#default_value' => $node->webform['total_submit_limit'] != -1 ? $node->webform['total_submit_limit'] : '',
    '#parents' => array('total_submit_limit'),
  );
  $interval_options = array(
      '-1' => t('ever'),
      '60' => t('every minute'),
      '300' => t('every 5 minutes'),
      '3600' => t('every hour'),
      '86400' => t('every day'),
      '604800' => t('every week'),
    );
  $form['submission']['total_submit_limit']['total_submit_interval'] = array(
    '#type' => 'select',
    '#options' => $interval_options,
    '#default_value' => $node->webform['total_submit_interval'],
    '#parents' => array('total_submit_interval'),
  );

  // Submission limit per user settings.
  $form['submission']['submit_limit'] = array(
    '#type' => 'item',
    '#title' => t('Per user submission limit'),
    '#theme' => 'webform_advanced_submit_limit_form',
    '#description' => t('Limit the number of submissions <em>per user</em>. A user is identified by their user login if logged-in, or by their IP Address and Cookie if anonymous. Use of cookies may be modified in the global <a href="!url">Webform settings</a>.', array('!url' => url('admin/config/content/webform'))),
  );
  $form['submission']['submit_limit']['enforce_limit'] = array(
    '#type' => 'radios',
    '#options' => array('no' => t('Unlimited'), 'yes' => 'Limit each user to !count submission(s) !timespan'),
    '#default_value' => $node->webform['submit_limit'] == -1 ? 'no' : 'yes',
    '#parents' => array('enforce_limit'),
  );
  $form['submission']['submit_limit']['submit_limit'] = array(
    '#type' => 'textfield',
    '#maxlength' => 2,
    '#size' => 2,
    '#default_value' => $node->webform['submit_limit'] != -1 ? $node->webform['submit_limit'] : '',
    '#parents' => array('submit_limit'),
  );
  $form['submission']['submit_limit']['submit_interval'] = array(
    '#type' => 'select',
    '#options' => $interval_options,
    '#default_value' => $node->webform['submit_interval'],
    '#parents' => array('submit_interval'),
  );

  $form['submission']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status of this form'),
    '#default_value' => $node->webform['status'] == 0 ? 0 : 1,
    '#description' => t('Closing a form prevents any further submissions by any users.'),
    '#parents' => array('status'),
    '#options' => array(1 => t('Open'), 0 => t('Closed')),
  );
  /* End Edit Form */

  /* Start per-role submission control */
  $form['role_control'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission access'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These permissions affect which roles can submit this webform. It does not prevent access to the webform page. If needing to prevent access to the webform page entirely, use a content access module such as <a href="http://drupal.org/project/taxonomy_access">Taxonomy Access</a> or <a href="http://drupal.org/project/node_privacy_byrole">Node Privacy by Role</a>.'),
    '#access' => webform_variable_get('webform_submission_access_control'),
    '#weight' => 10,
  );
  $user_roles = user_roles();
  foreach ($user_roles as $rid => $rname) {
    if ($rid == DRUPAL_ANONYMOUS_RID || $rid == DRUPAL_AUTHENTICATED_RID) {
      continue;
    }
    $user_roles[$rid] = webform_tt("user:rid:$rid:name", $rname);
  }
  $form['role_control']['roles'] = array(
    '#default_value' => $node->webform['roles'],
    '#options' => $user_roles,
    '#type' => 'checkboxes',
    '#title' => t('Roles that can submit this webform'),
    '#description' => t('The %authenticated role applies to any user signed into the site, regardless of other assigned roles.', array('%authenticated' => $user_roles[2])),
  );
  /* End per-role submission control */

  /* Start progress bar settings form */
  $form['progressbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Progress bar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 15,
  );
  $progress_bar_style = array_filter(array(
    $node->webform['progressbar_page_number'] ? 'progressbar_page_number' : NULL,
    $node->webform['progressbar_percent'] ? 'progressbar_percent' : NULL,
    $node->webform['progressbar_bar'] ? 'progressbar_bar' : NULL,
    $node->webform['progressbar_pagebreak_labels'] ? 'progressbar_pagebreak_labels' : NULL,
    $node->webform['progressbar_include_confirmation'] ? 'progressbar_include_confirmation' : NULL,
  ));
  $form['progressbar']['webform_progressbar_style']  = array(
    '#type' => 'checkboxes',
    '#title' => t('Progress bar style'),
    '#options' => array(
      'progressbar_bar' => t('Show progress bar'),
      'progressbar_page_number' => t('Show page number as number of completed (i.e. Page 1 of 10)'),
      'progressbar_percent' => t('Show percentage completed (i.e. 10%)'),
      'progressbar_pagebreak_labels' => t('Show page labels from page break components'),
      'progressbar_include_confirmation' => t('Include confirmation page in progress bar'),
    ),
    '#default_value' => $progress_bar_style,
    '#description' => t('Choose how the progress bar should be displayed for multi-page forms.'),
  );
  $form['progressbar']['progressbar_label_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First page label'),
    '#default_value' => $node->webform['progressbar_label_first'],
    '#maxlength' => 255,
    '#description' => t('The first page label in the progress bar. Subseqent pages are titled by their page break label.'),
    '#states' => array(
      'visible' => array(
        ':input[name="webform_progressbar_style[progressbar_pagebreak_labels]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['progressbar']['progressbar_label_confirmation'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation page label'),
    '#default_value' => $node->webform['progressbar_label_confirmation'],
    '#maxlength' => 255,
    '#states' => array(
      'visible' => array(
        ':input[name="webform_progressbar_style[progressbar_pagebreak_labels]"]' => array('checked' => TRUE),
        ':input[name="webform_progressbar_style[progressbar_include_confirmation]"]' => array('checked' => TRUE),
      ),
    ),
  );
  /* End progress bar settings form */

  /* Preview page settings form */
  $form['preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 18,
    '#id' => 'webform-preview-fieldset',
  );

  $form['preview']['preview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable preview page'),
    '#description' => t('Add a page for previewing the form before submitting.'),
    '#default_value' => $node->webform['preview'],
    '#id' => 'webform-preview-enable'
  );
  $form['preview']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        '#webform-preview-enable' => array('checked' => TRUE),
      ),
    ),
    '#id' => 'webform-preview-settings',
    '#theme_wrappers' => array('container'),
  );
  $form['preview']['settings']['preview_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Preview page title'),
    '#default_value' => $node->webform['preview_title'],
    '#description' => t('The page title will be used in the progress bar (if enabled). If left blank, the default title %preview will be used.', array('%preview' => t('Preview'))),
  );
  $form['preview']['settings']['preview_next_button_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Preview button label'),
    '#default_value' => $node->webform['preview_next_button_label'],
    '#description' => t('The text for the button that will proceed to the preview page. If left blank, the default label %preview will be used.', array('%preview' => t('Preview'))),
    '#size' => 20,
  );
  $form['preview']['settings']['preview_prev_button_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous page button label'),
    '#default_value' => $node->webform['preview_prev_button_label'],
    '#description' => t('The text for the button to go backwards from the preview page. If left blank, the default label %previous will be used.', array('%previous' => t('< Previous'))),
    '#size' => 20,
  );
  $submit_button_text = $node->webform['submit_text'] ? t($node->webform['submit_text']) : t('Submit');
  $preview_default_message = t('Please review your submission. Your submission is not complete until you press the "@button" button!', array('@button' => $submit_button_text));
  $form['preview']['settings']['preview_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Preview message'),
    '#default_value' => $node->webform['preview_message'],
    '#format' => $node->webform['preview_message_format'],
    '#description' => t('A message to be displayed on the preview page. If left blank, the message "!default" will be used. Supports Webform token replacements.', array('!default' => $preview_default_message))  . ' ' . theme('webform_token_help', array('groups' => array('node', 'submission'))),
  );
  $form['preview']['settings']['preview_components'] = array(
    '#type' => 'select',
    '#title' => t('Included preview values'),
    '#options' => webform_component_list($node, 'email', TRUE),
    '#default_value' => array_diff(array_keys($node->webform['components']), $node->webform['preview_excluded_components']),
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('If you wish to include only parts of the submission in the preview, select the components that should be included.'),
    '#process' => array('webform_component_select'),
  );
  /* End preview page settings form */

  /* Start advanced settings form */
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 20,
  );
  $form['advanced']['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Available as block'),
    '#default_value' => $node->webform['block'],
    '#description' => t('If enabled this webform will be available as a block.'),
    '#access' => user_access('administer blocks') || user_access('administer site configuration') || user_access('use panels dashboard'),
  );
  $form['advanced']['allow_draft'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "Save draft" button'),
    '#default_value' => $node->webform['allow_draft'],
    '#description' => t('Allow your users to save and finish the form later. This option is available only for authenticated users.'),
  );
  $form['advanced']['auto_save'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically save as draft between pages and when there are validation errors'),
    '#default_value' => $node->webform['auto_save'],
    '#description' => t('Automatically save partial submissions when users click the "Next" or "Previous" buttons in a multipage form or when validation errors prevent form submission.'),
  );
  $form['advanced']['submit_notice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the notification about previous submissions'),
    '#default_value' => $node->webform['submit_notice'],
    '#description' => t('Show the previous submissions notification that appears when users have previously submitted this form.'),
  );
  $form['advanced']['submit_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Submit button label'),
    '#default_value' => $node->webform['submit_text'],
    '#description' => t('By default the submit button on this form will have the label <em>Submit</em>. Enter a new title here to override the default.'),
    '#size' => 20,
  );
  $form['advanced']['next_serial'] = array(
    '#type' => 'textfield',
    '#title' => t('Next submission number'),
    '#default_value' => $node->webform['next_serial'],
    '#description' => t('The value of the next submission number. This is usually 1 when you start and will go up with each form submission.'),
    '#size' => 8,
    '#maxlength' => 8,
  );
  /* End Advanced Settings Form */

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 300,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validate handler for webform_configure_form().
 */
function webform_configure_form_validate($form, &$form_state) {
  // Ensure the entered e-mail addresses are valid.
  if (!empty($form_state['values']['email'])) {
    $emails = explode(',', $form_state['values']['email']);
    foreach ($emails as $email) {
      if (!valid_email_address(trim($email))) {
        form_error($form['submission']['redirect_url'], t('The entered email address %address is not a valid address.', array('%address' => $email)));
        break;
      }
    }
  }

  // Ensure the entered redirect URL is valid.
  if ($form_state['values']['redirect'] == 'url') {
    $redirect_url = trim($form_state['values']['redirect_url']);
    if (empty($redirect_url)) {
      form_error($form['submission']['redirection']['redirect_url'], t('A valid URL is required for custom redirection.'));
    }
    elseif (strpos($redirect_url, 'http') === 0 && !valid_url(webform_replace_tokens($redirect_url, $form['#node']), TRUE)) {
      form_error($form['submission']['redirection']['redirect_url'], t('The entered URL is not a valid address.'));
    }
    else {
      form_set_value($form['submission']['redirection']['redirect_url'], $redirect_url, $form_state);
    }
  }
  elseif ($form_state['values']['redirect'] == 'confirmation') {
    form_set_value($form['submission']['redirection']['redirect_url'], '<confirmation>', $form_state);
  }
  else {
    form_set_value($form['submission']['redirection']['redirect_url'], '<none>', $form_state);
  }

  // Ensure only positive integers are entered as submission limits.
  foreach (array('total_submit_limit', 'submit_limit') as $field) {
    $limit = $form['submission'][$field][$field]['#value'];
    if ($limit !== '' && (int) $limit < 1 && (int) $limit !== -1) {
      form_error($form['submission'][$field][$field], t('The submission limit must be at least 1.'));
    }
  }

  // Serial number must be a positive integer greater than any existing serial number.
  $next_min = _webform_submission_serial_next_value_used($form['#node']->nid);
  if ((int) $form['advanced']['next_serial']['#value'] < $next_min) {
    form_error($form['advanced']['next_serial'], t('The next submission number must be at least %min (greater than any existing serial number).', array('%min' => $next_min)));
  }
}

/**
 * Submit handler for webform_configure_form().
 */
function webform_configure_form_submit($form, &$form_state) {
  // Edit the node by reference just to shorten it up.
  $node = &$form['#node'];

  // Save the confirmation.
  $node->webform['confirmation'] = $form_state['values']['confirmation']['value'];
  $node->webform['confirmation_format'] = $form_state['values']['confirmation']['format'];

  // Save the redirect URL
  $node->webform['redirect_url'] = $form_state['values']['redirect_url'];

  // Overall form status.
  $node->webform['status'] = $form_state['values']['status'];

  // Save roles.
  $node->webform['roles'] = array_keys(array_filter($form_state['values']['roles']));

  // Set the block option.
  $node->webform['block'] = $form_state['values']['block'];

  // Set the draft option.
  $node->webform['allow_draft'] = $form_state['values']['allow_draft'];

  // Set the auto-save draft option.
  $node->webform['auto_save'] = $form_state['values']['auto_save'];

  // Set the submit limit to -1 if set to unlimited.
  if ($form_state['values']['enforce_limit'] == 'no') {
    $node->webform['submit_limit'] = -1;
    $node->webform['submit_interval'] = -1;
  }
  else {
    $node->webform['submit_limit'] = (int) $form_state['values']['submit_limit'];
    $node->webform['submit_interval'] = (int) $form_state['values']['submit_interval'];
  }

  // Set the total submit limit to -1 if set to unlimited.
  if ($form_state['values']['enforce_total_limit'] == 'no') {
    $node->webform['total_submit_limit'] = -1;
    $node->webform['total_submit_interval'] = -1;
  }
  else {
    $node->webform['total_submit_limit'] = (int) $form_state['values']['total_submit_limit'];
    $node->webform['total_submit_interval'] = (int) $form_state['values']['total_submit_interval'];
  }

  // Set the progress bar preferences.
  $progress_bar_settings = array_filter($form_state['values']['webform_progressbar_style']);
  $node->webform['progressbar_page_number'] = in_array('progressbar_page_number', $progress_bar_settings);
  $node->webform['progressbar_percent'] = in_array('progressbar_percent', $progress_bar_settings);
  $node->webform['progressbar_bar'] = in_array('progressbar_bar', $progress_bar_settings);
  $node->webform['progressbar_pagebreak_labels'] = in_array('progressbar_pagebreak_labels', $progress_bar_settings);
  $node->webform['progressbar_include_confirmation'] = in_array('progressbar_include_confirmation', $progress_bar_settings);
  $node->webform['progressbar_label_first'] = $form_state['values']['progressbar_label_first'];
  $node->webform['progressbar_label_confirmation'] = $form_state['values']['progressbar_label_confirmation'];

  // Set the preview settings.
  $node->webform['preview'] = (int) $form_state['values']['preview'];
  $node->webform['preview_next_button_label'] = $form_state['values']['preview_next_button_label'];
  $node->webform['preview_prev_button_label'] = $form_state['values']['preview_prev_button_label'];
  $node->webform['preview_title'] = $form_state['values']['preview_title'];
  $node->webform['preview_message'] = $form_state['values']['preview_message']['value'];
  $node->webform['preview_message_format'] = $form_state['values']['preview_message']['format'];

  // Save the list of included preview components.
  // We actually maintain an *exclusion* list, so any new components will
  // default to being included in the preview until unchecked.
  $included = array_keys(array_filter((array) $form_state['values']['preview_components']));
  $excluded = array_diff(array_keys($node->webform['components']), $included);
  $node->webform['preview_excluded_components'] = $excluded;

  // Set submit notice.
  $node->webform['submit_notice'] = $form_state['values']['submit_notice'];

  // Set submit button text.
  $node->webform['submit_text'] = $form_state['values']['submit_text'];

  // Set next serial number.
  $node->webform['next_serial'] = (int) $form_state['values']['next_serial'];
}

/**
 * Submit handler for webform_configure_form() that saves the node.
 *
 * This is separate from webform_configure_form_submit() to allow other modules
 * to add properties if needed into the $form['#node'] object before save.
 */
function webform_configure_form_submit_save($form, &$form_state) {
  node_save($form['#node']);
  drupal_set_message(t('The form settings have been updated.'));

  $node = &$form['#node'];
  if (!$node->webform['block'] && function_exists('block_load') && 
      ($block = block_load('webform', 'client-block-' . $node->nid)) &&
      !empty($block->bid)) {
    // An existing block for this not-currently-available block was already configured.
    // See https://www.drupal.org/node/2365825
    // The core block module contains a bug which causes it to not remove blocks
    // which are no longer available in code. Therefore, to prevent the block from
    // being processed, it must be removed manually here.
    // Remove the block for all themes.
    db_delete('block')
      ->condition('module', 'webform')
      ->condition('delta', $block->delta)
      ->execute();
    // Remove any roles associated with the block
    db_delete('block_role')
      ->condition('module', 'webform')
      ->condition('delta', $block->delta)
      ->execute();
    drupal_set_message(t('The block %info was deleted.', array('%info' => t('Webform: !title', array('!title' => $node->title)))));
    cache_clear_all();
  }
}

/**
 * Theme the redirection setting on the webform node form.
 */
function theme_webform_advanced_redirection_form($variables) {
  $form = $variables['form'];
  // Add special class for setting the active radio button.
  $form['redirect_url']['#attributes']['class'][] = 'webform-set-active';

  // Remove wrappers around the inline Redirect URL field.
  $form['redirect_url']['#theme_wrappers'] = array();

  $form['redirect']['confirmation']['#theme_wrappers'] = array('webform_inline_radio');
  $form['redirect']['url']['#theme_wrappers'] = array('webform_inline_radio');
  $form['redirect']['none']['#theme_wrappers'] = array('webform_inline_radio');
  $form['redirect']['url']['#title'] = t('Custom URL: !redirect_url', array('!redirect_url' => drupal_render($form['redirect_url'])));

  return drupal_render_children($form);
}

/**
 * Theme the submit limit fieldset on the webform node form.
 */
function theme_webform_advanced_submit_limit_form($variables) {
  $form = $variables['form'];
  $form['submit_limit']['#attributes']['class'][] = 'webform-set-active';
  $form['submit_interval']['#attributes']['class'][] = 'webform-set-active';
  // Remove div wrappers around limit options.
  $form['submit_limit']['#theme_wrappers'] = array();
  $form['submit_interval']['#theme_wrappers'] = array();

  $form['enforce_limit']['no']['#theme_wrappers'] = array('webform_inline_radio');
  $form['enforce_limit']['yes']['#theme_wrappers'] = array('webform_inline_radio');
  $form['enforce_limit']['yes']['#title'] = t('Limit each user to !count submission(s) !timespan', array('!count' => drupal_render($form['submit_limit']), '!timespan' => drupal_render($form['submit_interval'])));

  return drupal_render_children($form);
}

/**
 * Theme the total submit limit fieldset on the webform node form.
 */
function theme_webform_advanced_total_submit_limit_form($variables) {
  $form = $variables['form'];
  $form['total_submit_limit']['#attributes']['class'][] = 'webform-set-active';
  $form['total_submit_interval']['#attributes']['class'][] = 'webform-set-active';
  // Remove div wrappers around limit options.
  $form['total_submit_limit']['#theme_wrappers'] = array();
  $form['total_submit_interval']['#theme_wrappers'] = array();

  $form['enforce_total_limit']['no']['#theme_wrappers'] = array('webform_inline_radio');
  $form['enforce_total_limit']['yes']['#theme_wrappers'] = array('webform_inline_radio');
  $form['enforce_total_limit']['yes']['#title'] = t('Limit to !count total submission(s) !timespan', array('!count' => drupal_render($form['total_submit_limit']), '!timespan' => drupal_render($form['total_submit_interval'])));

  return drupal_render_children($form);
}
