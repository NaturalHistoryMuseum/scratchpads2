<?php

/**
 * @file
 * Provides interface and database handling for e-mail settings of a webform.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

/**
 * Overview form of all components for this webform.
 */
function webform_emails_form($form, $form_state, $node) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form['#attached']['library'][] = array('webform', 'admin');

  $form['#tree'] = TRUE;
  $form['#node'] = $node;
  $form['components'] = array();

  foreach ($node->webform['emails'] as $eid => $email) {
    $form['emails'][$eid]['email'] = array(
      '#markup' => nl2br(check_plain(implode("\n", webform_format_email_address($email['email'], NULL, $node, NULL, FALSE, FALSE)))),
    );
    $form['emails'][$eid]['subject'] = array(
      '#markup' => check_plain(webform_format_email_subject($email['subject'], $node)),
    );
    $form['emails'][$eid]['from'] = array(
      '#markup' => check_plain(webform_format_email_address($email['from_address'], $email['from_name'], $node, NULL, FALSE)),
    );
  }

  $form['add'] = array(
    '#theme' => 'webform_email_add_form',
    '#tree' => FALSE,
  );

  $form['add']['email_option'] = array(
    '#type' => 'radios',
    '#options' => array(
      'custom' => t('Address'),
      'component' => t('Component value'),
    ),
    '#default_value' => 'custom',
  );

  $form['add']['email_custom'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#maxlength' => 500,
  );

  $form['add']['email_component'] = array(
    '#type' => 'select',
    '#options' => webform_component_list($node, 'email_address', FALSE),
  );

  if (empty($form['add']['email_component']['#options'])) {
    $form['add']['email_component']['#options'][''] = t('No available components');
    $form['add']['email_component']['#disabled'] = TRUE;
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 45,
  );
  $form['actions']['add_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  $form['#validate'] = array('webform_email_address_validate');

  return $form;
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @param $form
 *   The form array.
 * @return
 *   Formatted HTML form, ready for display.
 */
function theme_webform_emails_form($variables) {
  $form = $variables['form'];
  $node = $form['#node'];

  $header = array(t('E-mail to'), t('Subject'), t('From'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();

  if (!empty($form['emails'])) {
    foreach (element_children($form['emails']) as $eid) {
      // Add each component to a table row.
      $rows[] = array(
        drupal_render($form['emails'][$eid]['email']),
        drupal_render($form['emails'][$eid]['subject']),
        drupal_render($form['emails'][$eid]['from']),
        l(t('Edit'), 'node/' . $node->nid . '/webform/emails/' . $eid),
        l(t('Clone'), 'node/' . $node->nid . '/webform/emails/' . $eid . '/clone'),
        l(t('Delete'), 'node/' . $node->nid . '/webform/emails/' . $eid . '/delete'),
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('Currently not sending e-mails, add an e-mail recipient below.'), 'colspan' => 6));
  }

  // Add a row containing form elements for a new item.
  $row_data = array(
    array('colspan' => 3, 'data' => drupal_render($form['add'])),
    array('colspan' => 3, 'data' => drupal_render($form['add_button'])),
  );
  $rows[] = array('data' => $row_data, 'class' => array('webform-add-form'));

  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE, 'attributes' => array('id' => 'webform-emails')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Theme the add new e-mail settings form on the node/x/webform/emails page.
 */
function theme_webform_email_add_form($variables) {
  $form = $variables['form'];

  // Add a default value to the custom e-mail textfield.
  $form['email_custom']['#attributes']['placeholder'] = t('email@example.com');
  $form['email_custom']['#attributes']['class'][] = 'webform-set-active';
  $form['email_custom']['#theme_wrappers'] = array();
  $form['email_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['email_option']['custom']['#title'] = t('Address: !email', array('!email' => drupal_render($form['email_custom'])));

  // Render the component value.
  $form['email_component']['#theme_wrappers'] = array();
  $form['email_component']['#attributes']['class'][] = 'webform-set-active';
  $form['email_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['email_option']['component']['#title'] = t('Component value: !component', array('!component' => drupal_render($form['email_component'])));

  return drupal_render_children($form);
}

/**
 * Submit handler for webform_emails_form().
 */
function webform_emails_form_submit($form, &$form_state) {
  if ($form_state['values']['email_option'] == 'custom') {
    $email = $form_state['values']['email_custom'];
  }
  else {
    $email = $form_state['values']['email_component'];
  }
  $form_state['redirect'] = array('node/' . $form['#node']->nid . '/webform/emails/new', array('query' => array('option' => $form_state['values']['email_option'], 'email' => trim($email))));
}

/**
 * Form for configuring an e-mail setting and template.
 */
function webform_email_edit_form($form, $form_state, $node, $email = array(), $clone = FALSE) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form['#attached']['library'][] = array('webform', 'admin');
  $form['#attached']['js'][] = array('data' => array('webform' => array('revertConfirm' => t('Are you sure you want to revert any changes to your template back to the default?'))), 'type' => 'setting');

  $form['#tree'] = TRUE;
  $form['#node'] = $node;
  $form['eid'] = array(
    '#type' => 'value',
    '#value' => isset($email['eid']) ? $email['eid'] : NULL,
  );
  $form['clone'] = array(
    '#type' => 'value',
    '#value' => $clone,
  );

  // All these fields work essentially the same, with a radio button set,
  // a textfield for custom values, and a select list for a component.
  foreach (array('email', 'subject', 'from_address', 'from_name') as $field) {
    switch ($field) {
      case 'email':
        $default_value = NULL;
        $title = t('E-mail to address');
        $description = t('Form submissions will be e-mailed to this address. Any email, select, or hidden form element may be selected as the recipient address. Multiple e-mail addresses may be separated by commas.');
        break;
      case 'subject':
        $default_value = webform_replace_tokens(webform_variable_get('webform_default_subject'), $node);
        $title = t('E-mail subject');
        $description = t('Any textfield, select, or hidden form element may be selected as the subject for e-mails.');
        break;
      case 'from_address':
        $default_value = webform_replace_tokens(webform_variable_get('webform_default_from_address'), $node);
        $title = t('E-mail from address');
        $description = t('Any email, select, or hidden form element may be selected as the sender\'s e-mail address.');
        break;
      case 'from_name':
        $default_value = webform_replace_tokens(webform_variable_get('webform_default_from_name'), $node);
        $title = t('E-mail from name');
        $description = t('Any textfield, select, or hidden form element may be selected as the sender\'s name for e-mails.');
        break;
    }

    $form[$field . '_option'] = array(
      '#title' => $title,
      '#type' => 'radios',
      '#default_value' => is_numeric($email[$field]) ? 'component' : ((empty($default_value) || ($email[$field] != 'default' && isset($email[$field]))) ? 'custom' : 'default'),
      '#description' => $description,
    );
    if (!empty($default_value)) {
      $form[$field . '_option']['#options']['default'] = t('Default: %value', array('%value' => $default_value));
    }
    $form[$field . '_option']['#options']['custom'] = t('Custom');
    $form[$field . '_option']['#options']['component'] = t('Component');

    $form[$field . '_custom'] = array(
      '#type' => 'textfield',
      '#size' => 40,
      '#default_value' => (!is_numeric($email[$field]) && $email[$field] != 'default') ? $email[$field] : NULL,
      '#maxlength' => 500,
    );
    $field_type = $field === 'from_address' || $field === 'email' ? 'email_address' : 'email_name';
    $component_options = webform_component_list($node, $field_type, FALSE);
    $form[$field . '_component'] = array(
      '#type' => 'select',
      '#default_value' =>  is_numeric($email[$field]) ? $email[$field] : NULL,
      '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
      '#disabled' => empty($component_options) ? TRUE : FALSE,
      '#weight' => 6,
    );

    // If this component is being used as an e-mail address and has multiple
    // options (such as a select list or checkboxes), we provide text fields to
    // map each option to a user-defined e-mail.
    if ($field_type === 'email_address') {
      foreach ($component_options as $cid => $component_label) {
        $component = $node->webform['components'][$cid];
        $options = webform_component_invoke($component['type'], 'options', $component, TRUE);
        // For components that don't provide multiple options (hidden or email).
        if (empty($options)) {
          continue;
        }

        // To avoid flooding the form with hundreds of textfields, skip select
        // lists that have huge numbers of options.
        if (count($options) > webform_variable_get('webform_email_select_max')) {
          unset($form[$field . '_component']['#options'][$cid]);
          continue;
        }
        $form[$field . '_mapping'][$cid] = array(
          '#title' => check_plain($component['name']),
          '#theme' => 'webform_email_component_mapping',
          '#attributes' => array('class' => array('webform-email-mapping')),
          '#webform_allow_empty' => $field === 'email',
          '#type' => 'container',
          '#states' => array(
            'visible' => array(
              'input[name=' . $field . '_option' . ']' => array('value' => 'component'),
              'select[name=' . $field . '_component' . ']' => array('value' => (string) $cid),
            ),
          ),
        );
        foreach ($options as $key => $label) {
          $form[$field . '_mapping'][$cid][$key] = array(
            '#type' => 'textfield',
            '#title' => $label,
            '#default_value' => is_numeric($email[$field]) && $email[$field] == $cid && is_array($email['extra']) && isset($email['extra'][$field . '_mapping'][$key]) ? $email['extra'][$field . '_mapping'][$key] : '',
            '#attributes' => array('placeholder' => t('email@example.com')),
            '#maxlength' => 500,
          );
        }
      }
    }
  }

  // Do not show the "E-mail from name" if using the short e-mail format.
  if (webform_variable_get('webform_email_address_format') == 'short') {
    $form['from_name_option']['#access'] = FALSE;
    $form['from_name_custom']['#access'] = FALSE;
    $form['from_name_component']['#access'] = FALSE;
  }

  // Add the template fieldset.
  $form['template'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail template'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($email['cid']) && empty($email['template']),
    '#description' => t('An e-mail template can customize the display of e-mails.'),
    '#weight' => 15,
    '#tree' => FALSE,
    '#attributes' => array('id' => 'webform-template-fieldset'),
  );

  $form['template']['template_option'] = array(
    '#type' => 'select',
    '#options' => array(
      'default' => t('Default template'),
      'custom' => t('Custom template'),
    ),
    '#default_value' => $email['template'] == 'default' ? 'default' : 'custom',
  );

  $default_template = theme(array('webform_mail_' . $node->nid, 'webform_mail', 'webform_mail_message'), array('node' => $node, 'email' => $email));
  $template = $email['template'] == 'default' ? $default_template : $email['template'];
  $form['template']['template'] = array(
    '#type' => 'textarea',
    '#rows' => max(10, min(20, count(explode("\n", $template)))),
    '#default_value' => $template,
    '#wysiwyg' => webform_variable_get('webform_email_html_capable') ? NULL : FALSE,
    '#description' => theme('webform_token_help', array('groups' => array('node', 'submission'))),
  );

  $form['template']['html'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send e-mail as HTML'),
    '#default_value' => $email['html'],
    '#access' => webform_variable_get('webform_email_html_capable') && !webform_variable_get('webform_format_override'),
  );

  $form['template']['attachments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include files as attachments'),
    '#default_value' => $email['attachments'],
    '#access' => webform_variable_get('webform_email_html_capable'),
  );

  $form['template']['components'] = array(
    '#type' => 'select',
    '#title' => t('Included e-mail values'),
    '#options' => webform_component_list($node, 'email', TRUE),
    '#default_value' => array_diff(array_keys($node->webform['components']), $email['excluded_components']),
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('The selected components will be included in the [submission:values] token. Individual values may still be printed if explicitly specified as a [submission:values:?] in the template.'),
    '#process' => array('webform_component_select'),
  );

  $form['template']['components']['suffix']['exclude_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude empty components'),
    '#default_value' => $email['exclude_empty'],
  );

  // TODO: Allow easy re-use of existing templates.
  $form['templates']['#tree'] = TRUE;
  $form['templates']['default'] = array(
    '#type' => 'textarea',
    '#value' => $default_template,
    '#resizable' => FALSE,
    '#weight' => 19,
    '#wysiwyg' => FALSE,
  );

  // Add the submit button.
  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 20,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save e-mail settings'),
  );

  $form['#validate'] = array('webform_email_address_validate', 'webform_email_edit_form_validate');

  return $form;
}

/**
 * Theme the Webform mail settings section of the node form.
 */
function theme_webform_email_edit_form($variables) {
  $form = $variables['form'];

  // Loop through fields, rendering them into radio button options.
  foreach (array('email', 'subject', 'from_address', 'from_name') as $field) {
    foreach (array('custom', 'component') as $option) {
      $form[$field . '_' . $option]['#attributes']['class'][] = 'webform-set-active';
      $form[$field . '_' . $option]['#theme_wrappers'] = array();
      $form[$field . '_option'][$option]['#theme_wrappers'] = array('webform_inline_radio');
      $form[$field . '_option'][$option]['#title'] = t('!title: !field', array('!title' => $form[$field . '_option'][$option]['#title'], '!field' => drupal_render($form[$field . '_' . $option])));
    }
    if (isset($form[$field . '_option']['#options']['default'])) {
      $form[$field]['#theme_wrappers'] = array();
      $form[$field . '_option']['default']['#theme_wrappers'] = array('webform_inline_radio');
    }
  }

  $details = '';
  $details .= drupal_render($form['subject_option']);
  $details .= drupal_render($form['from_address_option']);
  $details .= drupal_render($form['from_address_mapping']);
  $details .= drupal_render($form['from_name_option']);
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail header details'),
    '#weight' => 10,
    '#children' => $details,
    '#collapsible' => FALSE,
    '#parents' => array('details'),
    '#groups' => array('details' => array()),
    '#attributes' => array(),
  );

  // Ensure templates are completely hidden.
  $form['templates']['#prefix'] = '<div id="webform-email-templates" style="display: none">';
  $form['templates']['#suffix'] = '</div>';

  // Re-sort the elements since we added the details fieldset.
  $form['#sorted'] = FALSE;
  $children = element_children($form, TRUE);
  return drupal_render_children($form, $children);
}

/**
 * Theme the presentation of select list option to e-mail address mappings.
 */
function theme_webform_email_component_mapping($variables) {
  $element = $variables['element'];

  $header = array(t('Option'), t('E-mail address'));
  $rows = array();
  foreach (element_children($element) as $key) {
    $element[$key]['#theme_wrappers'] = array();
    $row = array();
    $row[] = array(
      'data' => theme('form_element_label', array('element' => $element[$key])),
      'class' => array('webform-email-option'),
    );
    $row[] = drupal_render($element[$key]);
    $rows[] = $row;
  }

  $empty = t('This component has no options defined yet.');
  $table = theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE, 'empty' => $empty));
  $description = t('The selected component %name has multiple options. You may enter an e-mail address for each choice.', array('%name' => $element['#title']));
  if ($element['#webform_allow_empty']) {
    $description .= ' ' . t('When that choice is selected, an e-mail will be sent to the corresponding address. If a field is left blank, no e-mail will be sent for that option.');
  }
  else {
    $description .= ' ' . t('When that choice is selected, an e-mail will be sent from the corresponding address.');
  }

  $wrapper_element = array(
    '#title' => t('Component e-mail options'),
    '#children' => $table,
    '#description' => $description,
    '#theme_wrappers' => array('form_element'),
    '#id' => NULL,
  );

  return render($wrapper_element);
}

/**
 * Validate handler for webform_email_edit_form() and webform_emails_form().
 */
function webform_email_address_validate($form, &$form_state) {
  if ($form_state['values']['email_option'] == 'custom') {
    webform_email_validate($form_state['values']['email_custom'], 'email_custom', FALSE, TRUE, TRUE);
  }
}

/**
 * Validate handler for webform_email_edit_form().
 */
function webform_email_edit_form_validate($form, &$form_state) {
  if ($form_state['values']['from_address_option'] == 'custom') {
    webform_email_validate($form_state['values']['from_address_custom'], 'from_address_custom', FALSE, FALSE, TRUE);
  }

  // Validate component-based values for the TO and FROM address.
  foreach (array('email', 'from_address') as $field_name) {
    if ($form_state['values'][$field_name . '_option'] == 'component') {
      $cid = $form_state['values'][$field_name . '_component'];
      if (isset($form_state['values'][$field_name . '_mapping'][$cid])) {
        $empty_allowed = $field_name === 'email';
        $multiple_allowed = $field_name === 'email';
        foreach ($form_state['values'][$field_name . '_mapping'][$cid] as $key => &$value) {
          webform_email_validate($value, "{$field_name}_mapping][$cid][$key", $empty_allowed, $multiple_allowed, TRUE);
        }
      }
    }
  }

}

/**
 * Submit handler for webform_email_edit_form().
 */
function webform_email_edit_form_submit($form, &$form_state) {
  // Ensure a webform record exists.
  $node = $form['#node'];
  webform_ensure_record($node);

  // Remove duplicate email To: addresses.
  $form_state['values']['email_custom'] = implode(',', array_unique(array_map('trim', explode(',', $form_state['values']['email_custom']))));

  // Merge the e-mail, name, address, and subject options into single values.
  $email = array(
    'eid' => $form_state['values']['eid'],
    'nid' => $node->nid,
  );

  foreach (array('email', 'from_name', 'from_address', 'subject') as $field) {
    $option = $form_state['values'][$field . '_option'];
    if ($option == 'default') {
      $email[$field] = 'default';
    }
    else {
      $email[$field] = $form_state['values'][$field . '_' . $option];

      // Merge the email mapping(s) into single value(s)
      $cid = $form_state['values'][$field . '_' . $option];
      if (is_numeric($cid) && isset($form_state['values'][$field . '_mapping'][$cid])) {
        $email['extra'][$field . '_mapping'] = $form_state['values'][$field . '_mapping'][$cid];
      }
    }
  }

  // Ensure templates are unaffected by differences in line breaks.
  $form_state['values']['template'] = str_replace(array("\r", "\n"), array('', "\n"), $form_state['values']['template']);
  $form_state['values']['templates']['default'] = str_replace(array("\r", "\n"), array('', "\n"), $form_state['values']['templates']['default']);

  // Set the template value.
  // TODO: Support reuse of templates.
  if (strcmp(trim($form_state['values']['templates']['default']), trim($form_state['values']['template'])) == 0) {
    $email['template'] = 'default';
  }
  else {
    $email['template'] = $form_state['values']['template'];
  }

  // Save the attachment and HTML options provided by MIME mail.
  $email['html'] = empty($form_state['values']['html']) ? 0 : 1;
  $email['attachments'] = empty($form_state['values']['attachments']) ? 0 : 1;

  // Save the list of included components.
  // We actually maintain an *exclusion* list, so any new components will
  // default to being included in the [submission:values] token until unchecked.
  $included = array_keys(array_filter((array) $form_state['values']['components']));
  $excluded = array_diff(array_keys($node->webform['components']), $included);
  $email['excluded_components'] = $excluded;

  $email['exclude_empty'] = empty($form_state['values']['exclude_empty']) ? 0 : 1;

  if ($form_state['values']['clone']) {
    drupal_set_message(t('Email settings cloned.'));
    $form_state['values']['eid'] = webform_email_clone($email);
  }
  elseif (empty($form_state['values']['eid'])) {
    drupal_set_message(t('Email settings added.'));
    $form_state['values']['eid'] = webform_email_insert($email);
  }
  else {
    drupal_set_message(t('Email settings updated.'));
    webform_email_update($email);
  }

  // Refresh the entity cache, should it be cached in persistent storage.
  entity_get_controller('node')->resetCache(array($node->nid));

  $form_state['redirect'] = array('node/' . $node->nid . '/webform/emails');
}

/**
 * Form for deleting an e-mail setting.
 */
function webform_email_delete_form($form, $form_state, $node, $email) {
  $eid = $email['eid'];

  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['email'] = array(
    '#type' => 'value',
    '#value' => $email,
  );

  $question = t('Delete e-mail settings?');
  if (is_numeric($email['email'])) {
    $description = t('This will immediately delete the e-mail settings based on the @component component.', array('@component' => $email['email']));
  }
  else {
    $description = t('This will immediately delete the e-mail settings sending to the @address address.', array('@address' => $email['email']));
  }

  return confirm_form($form, $question, 'node/' . $node->nid . '/webform/emails', $description, t('Delete'));
}

/**
 * Submit handler for webform_email_delete_form().
 */
function webform_email_delete_form_submit($form, &$form_state) {
  // Delete the e-mail settings.
  $node = $form_state['values']['node'];
  $email = $form_state['values']['email'];
  webform_email_delete($node, $email);
  drupal_set_message(t('E-mail settings deleted.'));

  // Check if this webform still contains any information.
  unset($node->webform['emails'][$email['eid']]);
  webform_check_record($node);

  // Refresh the entity cache, should it be cached in persistent storage.
  entity_get_controller('node')->resetCache(array($node->nid));

  $form_state['redirect'] = 'node/' . $node->nid . '/webform/emails';
}

/**
 * Load an e-mail setting from the database or initialize a new e-mail.
 */
function webform_email_load($eid, $nid) {
  $node = node_load($nid);
  if ($eid == 'new') {
    $email = array(
      'email' => '',
      'subject' => 'default',
      'from_name' => 'default',
      'from_address' => 'default',
      'template' => 'default',
      'excluded_components' => array(),
      'exclude_empty' => 0,
      'html' => webform_variable_get('webform_default_format'),
      'attachments' => 0,
      'extra' => '',
    );
  }
  else {
    $email = isset($node->webform['emails'][$eid]) ? $node->webform['emails'][$eid] : FALSE;
    if ($email && webform_variable_get('webform_format_override')) {
      $email['html'] = webform_variable_get('webform_default_format');
    }
  }

  return $email;
}

/**
 * Insert a new e-mail setting into the database.
 *
 * @param $email
 *   An array of settings for sending an e-mail.
 */
function webform_email_insert($email) {
  // TODO: This is not race-condition safe. Switch to using transactions?
  if (!isset($email['eid'])) {
    $next_id_query = db_select('webform_emails')->condition('nid', $email['nid']);
    $next_id_query->addExpression('MAX(eid) + 1', 'eid');
    $email['eid'] = $next_id_query->execute()->fetchField();
    if ($email['eid'] == NULL) {
      $email['eid'] = 1;
    }
  }

  $email['excluded_components'] = implode(',', $email['excluded_components']);
  $email['extra'] = empty($email['extra']) ? '' : serialize($email['extra']);
  $success = drupal_write_record('webform_emails', $email);

  return $success ? $email['eid'] : FALSE;
}

/**
 * Clone an existing e-mail setting.
 *
 * @param $email
 *   An array of settings for sending an e-mail.
 */
function webform_email_clone($email) {
  $email['eid'] = NULL;
  return webform_email_insert($email);
}

/**
 * Update an existing e-mail setting with new values.
 *
 * @param $email
 *   An array of settings for sending an e-mail containing a nid, eid, and all
 *   other fields from the e-mail form.
 */
function webform_email_update($email) {
  $email['excluded_components'] = implode(',', $email['excluded_components']);
  $email['extra'] = empty($email['extra']) ? '' : serialize($email['extra']);
  return drupal_write_record('webform_emails', $email, array('nid', 'eid'));
}

/**
 * Delete an e-mail setting.
 */
function webform_email_delete($node, $email) {
  db_delete('webform_emails')
    ->condition('nid', $node->nid)
    ->condition('eid', $email['eid'])
    ->execute();
}
