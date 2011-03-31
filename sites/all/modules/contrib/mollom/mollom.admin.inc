<?php
// $Id: mollom.admin.inc,v 1.48 2010/11/07 18:55:32 sun Exp $

/**
 * @file
 * Administrative page callbacks for Mollom module.
 */

/**
 * Menu callback; Displays a list of forms configured for Mollom.
 */
function mollom_admin_form_list() {
  _mollom_testing_mode_warning();

  // Reset the cached list of protected forms.
  mollom_form_cache(TRUE);

  $modes = array(
    MOLLOM_MODE_ANALYSIS => t('Text analysis'),
    MOLLOM_MODE_CAPTCHA => t('CAPTCHA'),
  );

  $header = array(
    t('Form'),
    t('Protection mode'),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $result = db_query('SELECT form_id FROM {mollom_form}')->fetchCol();
  $forms = array();
  $module_info = system_get_info('module');
  foreach ($result as $form_id) {
    $forms[$form_id] = mollom_form_load($form_id);
    // system_get_info() only supports enabled modules. Default to the module's
    // machine name in case it is disabled.
    $module = $forms[$form_id]['module'];
    if (!isset($module_info[$module])) {
      $module_info[$module]['name'] = $module;
    }
    $forms[$form_id]['title'] = t('!module: !form-title', array(
      '!form-title' => $forms[$form_id]['title'],
      '!module' => t($module_info[$module]['name']),
    ));
  }

  // Sort forms by title (including module name prefix).
  uasort($forms, 'drupal_sort_title');

  $rows = array();
  foreach ($forms as $form_id => $mollom_form) {
    $row = array();
    $row[] = $mollom_form['title'];
    if ($mollom_form['mode'] == MOLLOM_MODE_ANALYSIS) {
      $row[] = t('!protection-mode (@discard)', array(
        '!protection-mode' => $modes[$mollom_form['mode']],
        '@discard' => $mollom_form['discard'] ? t('discard') : t('retain'),
      ));
    }
    else {
      $row[] = $modes[$mollom_form['mode']];
    }
    $row[] = array('data' => array(
      '#type' => 'link',
      '#title' => t('Configure'),
      '#href' => 'admin/config/content/mollom/manage/' . $form_id,
    ));
    $row[] = array('data' => array(
      '#type' => 'link',
      '#title' => t('Unprotect'),
      '#href' => 'admin/config/content/mollom/unprotect/' . $form_id,
    ));
    $rows[] = $row;
  }

  $build['forms'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => l(t('Add form'), 'admin/config/content/mollom/add'),
  );

  return $build;
}

/**
 * Return registered forms as an array suitable for a 'checkboxes' form element #options property.
 */
function mollom_admin_form_options() {
  // Retrieve all registered forms.
  $form_list = mollom_form_list();

  // Remove already configured form ids.
  $result = db_query('SELECT form_id FROM {mollom_form}')->fetchCol();
  foreach ($result as $form_id) {
    unset($form_list[$form_id]);
  }
  // If all registered forms are configured already, output a message, and
  // redirect the user back to overview.
  if (empty($form_list)) {
    drupal_set_message(t('All available forms are protected already.'));
    drupal_goto('admin/config/content/mollom');
  }

  // Load module information.
  $module_info = system_get_info('module');

  // Transform form information into an associative array suitable for #options.
  $options = array();
  foreach ($form_list as $form_id => $info) {
    // system_get_info() only supports enabled modules. Default to the module's
    // machine name in case it is disabled.
    $module = $info['module'];
    if (!isset($module_info[$module])) {
      $module_info[$module]['name'] = $module;
    }
    $options[$form_id] = t('!module: !form-title', array(
      '!form-title' => $info['title'],
      '!module' => t($module_info[$module]['name']),
    ));
  }
  // Sort form options by title.
  asort($options);

  return $options;
}

/**
 * Form builder; Configure Mollom protection for a form.
 */
function mollom_admin_configure_form($form, &$form_state, $mollom_form = NULL) {
  // If no $mollom_form was passed, then we are adding a new form configuration.
  if (!isset($mollom_form)) {
    if (!isset($form_state['storage']['mollom_form'])) {
      $form_state['storage']['step'] = 'select';
    }
    else {
      $form_state['storage']['step'] = 'configure';
      $mollom_form = $form_state['storage']['mollom_form'];
    }
  }
  // When adding a new form configuration, passing form_id via path argument.
  elseif (is_string($mollom_form)) {
    $mollom_form = mollom_form_new($mollom_form);
    $form_state['storage']['step'] = 'configure';
    $form_state['storage']['mollom_form'] = $mollom_form;
  }
  // Otherwise, we are editing an existing form configuration.
  else {
    $form_state['storage']['step'] = 'configure';
    $form_state['storage']['mollom_form'] = $mollom_form;
  }

  $form['#tree'] = TRUE;
  $form['actions'] = array(
    '#type' => 'actions',
  );

  switch ($form_state['storage']['step']) {
    case 'select':
      $form['mollom']['form_id'] = array(
        '#type' => 'select',
        '#title' => t('Form'),
        '#options' => mollom_admin_form_options(),
        '#required' => TRUE,
      );
      $form['actions']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
        '#submit' => array('mollom_admin_configure_form_next_submit'),
      );
      break;

    case 'configure':
      drupal_set_title(t('Configure %form-title protection', array('%form-title' => $mollom_form['title'])), PASS_THROUGH);

      $form['mollom']['form_id'] = array(
        '#type' => 'value',
        '#value' => $mollom_form['form_id'],
      );

      $modes = array();
      // Textual analysis, if any elements are available.
      if (!empty($mollom_form['elements'])) {
        $modes[MOLLOM_MODE_ANALYSIS] = t('Text analysis');
      }
      // CAPTCHA-only, always available.
      $modes[MOLLOM_MODE_CAPTCHA] = t('CAPTCHA');

      $form['mollom']['mode'] = array(
        '#type' => 'radios',
        '#title' => t('Protection mode'),
        '#options' => $modes,
        '#default_value' => isset($mollom_form['mode']) ? $mollom_form['mode'] : key($modes),
      );

      $all_permissions = array();
      foreach (module_implements('permission') as $module) {
        if ($module_permissions = module_invoke($module, 'permission')) {
          foreach ($module_permissions as &$info) {
            $info += array('module' => $module);
          }
          $all_permissions += $module_permissions;
        }
      }
      // Prepend Mollom's global permission to the list.
      array_unshift($mollom_form['bypass access'], 'bypass mollom protection');
      $permissions = array();
      foreach ($mollom_form['bypass access'] as $permission) {
        // @todo D7: Array keys are used as CSS class for the link list item,
        //   but are not sanitized: http://drupal.org/node/98696
        $permissions[drupal_html_class($permission)] = array(
          'title' => $all_permissions[$permission]['title'],
          'href' => 'admin/people/permissions',
          'fragment' => 'module-' . $all_permissions[$permission]['module'],
          'html' => TRUE,
        );
      }
      $form['mollom']['mode']['#description'] = t('The protection is omitted for users having any of the permissions: !permission-list', array(
        '!permission-list' => theme('links', array(
          'links' => $permissions,
          // @todo D7: Something went entirely wrong: system.menus.css makes ANY
          //   ul.links appear as if it would have the .inline CSS class.
          'attributes' => array(),
        )),
      ));

      if (!empty($mollom_form['elements'])) {
        // If not re-configuring an existing protection, make it the default.
        if (!isset($mollom_form['mode'])) {
          $form['mollom']['mode']['#default_value'] = MOLLOM_MODE_ANALYSIS;
        }

        // Textual analysis filters.
        $form['mollom']['checks'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Analyze text for'),
          '#options' => array(
            'spam' => t('Spam'),
            'profanity' => t('Profanity'),
          ),
          '#default_value' => $mollom_form['checks'],
          '#states' => array(
            'visible' => array(
              ':input[name="mollom[mode]"]' => array('value' => (string) MOLLOM_MODE_ANALYSIS),
            ),
          ),
        );

        // Form elements defined by hook_mollom_form_info() use the
        // 'parent][child' syntax, which Form API also uses internally for
        // form_set_error(), and which allows us to recurse into nested fields
        // during processing of submitted form values. However, since we are using
        // those keys also as internal values to configure the fields to use for
        // textual analysis, we need to encode them. Otherwise, a nested field key
        // would result in the following checkbox attribute:
        //   '#name' => 'mollom[enabled_fields][parent][child]'
        // This would lead to a form validation error, because it is a valid key.
        // By encoding them, we prevent this from happening:
        //   '#name' => 'mollom[enabled_fields][parent%5D%5Bchild]'
        $elements = array();
        foreach ($mollom_form['elements'] as $key => $value) {
          $elements[rawurlencode($key)] = $value;
        }
        $enabled_fields = array();
        foreach ($mollom_form['enabled_fields'] as $value) {
          $enabled_fields[] = rawurlencode($value);
        }
        $form['mollom']['enabled_fields'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Text fields to analyze'),
          '#options' => $elements,
          '#default_value' => $enabled_fields,
          '#states' => array(
            'visible' => array(
              ':input[name="mollom[mode]"]' => array('value' => (string) MOLLOM_MODE_ANALYSIS),
            ),
          ),
        );

        $form['mollom']['discard'] = array(
          '#type' => 'radios',
          '#title' => t('When text analysis identifies spam'),
          '#default_value' => $mollom_form['discard'],
          '#options' => array(
            1 => t('Automatically discard the post'),
            0 => t('Retain the post for manual moderation'),
          ),
          '#required' => $mollom_form['mode'] == MOLLOM_MODE_ANALYSIS,
          // Only possible for forms supporting moderation of unpublished posts.
          '#access' => !empty($mollom_form['moderation callback']),
          // Only possible for forms protected via text analysis.
          '#states' => array(
            'visible' => array(
              ':input[name="mollom[mode]"]' => array('value' => (string) MOLLOM_MODE_ANALYSIS),
            ),
          ),
        );
      }

      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );
      break;
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/content/mollom',
  );

  return $form;
}

/**
 * Form submit handler for 'Next' button on Mollom form configuration form.
 */
function mollom_admin_configure_form_next_submit($form, &$form_state) {
  $form_id = $form_state['values']['mollom']['form_id'];
  $form_state['redirect'] = $_GET['q'] . '/' . $form_id;
}

/**
 * Form validation handler for mollom_admin_configure_form().
 */
function mollom_admin_configure_form_validate(&$form, &$form_state) {
  // For the 'configure' step, output custom #required form element errors for
  // 'checks' and 'enabled_fields', as their labels do not work with the default
  // #required form error message.
  if ($form_state['storage']['step'] == 'configure') {
    // Make field checkboxes required, if protection mode is textual analysis.
    // @see http://drupal.org/node/875722
    $required = ($form_state['values']['mollom']['mode'] == MOLLOM_MODE_ANALYSIS);
    $form['mollom']['checks']['#required'] = $required;
    $form['mollom']['enabled_fields']['#required'] = $required;
    $form['mollom']['discard']['#required'] = $required;

    if ($required && !array_filter($form_state['values']['mollom']['checks'])) {
      form_error($form['mollom']['checks'], t('At least one text analysis check is required.'));
    }
    if ($required && !array_filter($form_state['values']['mollom']['enabled_fields'])) {
      form_error($form['mollom']['enabled_fields'], t('At least one field is required for text analysis.'));
    }
  }
}

/**
 * Form submit handler for mollom_admin_configure_form().
 */
function mollom_admin_configure_form_submit($form, &$form_state) {
  $mollom_form = $form_state['values']['mollom'];
  // Merge in form information from $form_state.
  $mollom_form += $form_state['storage']['mollom_form'];

  // Only store a list of enabled textual analysis checks.
  $mollom_form['checks'] = array_keys(array_filter($mollom_form['checks']));
  // Prepare selected fields for storage.
  $enabled_fields = array();
  foreach (array_keys(array_filter($mollom_form['enabled_fields'])) as $field) {
    $enabled_fields[] = rawurldecode($field);
  }
  $mollom_form['enabled_fields'] = $enabled_fields;

  $status = mollom_form_save($mollom_form);
  if ($status === SAVED_NEW) {
    drupal_set_message(t('The form protection has been added.'));
  }
  else {
    drupal_set_message(t('The form protection has been updated.'));
  }

  $form_state['redirect'] = 'admin/config/content/mollom';
}

/**
 * Form builder; Remove Mollom protection from a form.
 */
function mollom_admin_unprotect_form($form, &$form_state, $mollom_form) {
  $form['#tree'] = TRUE;
  $form['form'] = array(
    '#type' => 'item',
    '#title' => t('Form'),
    '#markup' => $mollom_form['title'],
  );
  $form['mollom']['form_id'] = array(
    '#type' => 'value',
    '#value' => $mollom_form['form_id'],
  );

  return confirm_form($form,
    t('Are you sure you want to unprotect this form?'),
    'admin/config/content/mollom',
    t('Mollom will no longer protect this form from spam.')
  );
}

/**
 * Form submit handler for mollom_admin_unprotect_form().
 */
function mollom_admin_unprotect_form_submit($form, &$form_state) {
  mollom_form_delete($form_state['values']['mollom']['form_id']);

  drupal_set_message(t('The form protection has been removed.'));

  $form_state['redirect'] = 'admin/config/content/mollom';
}

/**
 * Form constructor to configure the blacklist.
 *
 * @param $type
 *   The type of blacklist; i.e., 'spam', 'profanity', or 'unwanted'.
 */
function mollom_admin_blacklist_form($form, &$form_state, $type = 'spam') {
  $form['#tree'] = TRUE;

  // Translate internal reason values for rendering and select list in form.
  $contexts = array(
    'everything' => t('All fields'),
    'links' => t('Links'),
    'author' => t('Author name'),
  );
  $matches = array(
    'contains' => t('contains'),
    'exact' => t('exact'),
  );

  $form['blacklist'] = array();
  // Do not retrieve the current blacklist when submitting the form.
  $blacklist = (empty($form_state['input']) ? mollom('mollom.listBlacklistText') : array());
  if (is_array($blacklist)) {
    foreach ($blacklist as $id => $entry) {
      if ($entry['reason'] != $type) {
        continue;
      }
      // #class property is internally used by
      // theme_mollom_admin_blacklist_form().
      $row = array(
        'context' => array(
          '#markup' => check_plain($contexts[$entry['context']]),
          '#class' => 'mollom-blacklist-context value-' . check_plain($entry['context']),
        ),
        'match' => array(
          '#markup' => check_plain($matches[$entry['match']]),
          '#class' => 'mollom-blacklist-match value-' . check_plain($entry['match']),
        ),
        'text' => array(
          '#markup' => check_plain($entry['text']),
          '#class' => 'mollom-blacklist-text',
        ),
      );
      $row['actions']['delete'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/config/content/mollom/blacklist/delete',
        '#options' => array(
          'query' => array(
            'text' => $entry['text'],
            'context' => $entry['context'],
            'reason' => $entry['reason'],
          ) + drupal_get_destination(),
        ),
      );
      $form['blacklist'][$id] = $row;
    }
  }

  // The value NULL in the following select lists will intentionally trigger a
  // form validation error, since the elements are #required.
  // @see http://drupal.org/node/140783
  $form['entry']['context'] = array(
    '#type' => 'select',
    '#title' => t('Context'),
    '#title_display' => 'invisible',
    '#options' => array(NULL => t('- Any -')) + $contexts,
    '#required' => TRUE,
    '#id' => 'mollom-blacklist-filter-context',
  );
  $form['entry']['match'] = array(
    '#type' => 'select',
    '#title' => t('Match'),
    '#title_display' => 'invisible',
    '#options' => array(NULL => t('- Any -')) + $matches,
    '#required' => TRUE,
    '#id' => 'mollom-blacklist-filter-match',
  );
  $form['entry']['text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#title_display' => 'invisible',
    '#size' => 40,
    '#required' => TRUE,
    '#maxlength' => 64,
    '#id' => 'mollom-blacklist-filter-text',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['entry']['reason'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['entry']['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
  );
  $form['entry']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Form submit handler to save a text string to the Mollom blacklist.
 */
function mollom_admin_blacklist_form_submit($form, &$form_state) {
  $data = array(
    'text' => $form_state['values']['entry']['text'],
    'context' => $form_state['values']['entry']['context'],
    'match' => $form_state['values']['entry']['match'],
    'reason' => $form_state['values']['entry']['reason'],
  );
  $result = mollom('mollom.addBlacklistText', $data);

  $args = array(
    '@text' => $data['text'],
    '@context' => $data['context'],
    '@match' => $data['match'],
    '@reason' => $data['reason'],
  );
  if ($result === TRUE) {
    drupal_set_message(t('The entry was added to the blacklist.'));
    _mollom_watchdog(array(
      'Added @text (@context, @match) to @reason blacklist.' => $args,
      'Data:<pre>@data</pre>' => array('@data' => $data),
      'Result:<pre>@result</pre>' => array('@result' => $result),
    ));
  }
  else {
    drupal_set_message(t('An error occurred upon trying to add the text to the blacklist.'), 'error');
    _mollom_watchdog(array(
      'Failed to add @text (@context, @match) to @reason blacklist.' => $args,
      'Data:<pre>@data</pre>' => array('@data' => $data),
      'Result:<pre>@result</pre>' => array('@result' => $result),
    ), WATCHDOG_ERROR);
  }
}

/**
 * Formats the blacklist form as table to embed the form.
 */
function theme_mollom_admin_blacklist_form($variables) {
  $form = $variables['form'];
  $header = array(
    t('Context'),
    t('Matches'),
    t('Text'),
    '',
  );
  $rows = array();

  $rows[] = array(
    drupal_render($form['entry']['context']),
    drupal_render($form['entry']['match']),
    drupal_render($form['entry']['text']),
    drupal_render($form['entry']['actions']),
  );

  foreach (element_children($form['blacklist']) as $id) {
    $rows[] = array(
      array(
        'data' => drupal_render($form['blacklist'][$id]['context']),
        'class' => $form['blacklist'][$id]['context']['#class'],
      ),
      array(
        'data' => drupal_render($form['blacklist'][$id]['match']),
        'class' => $form['blacklist'][$id]['match']['#class'],
      ),
      array(
        'data' => drupal_render($form['blacklist'][$id]['text']),
        'class' => $form['blacklist'][$id]['text']['#class'],
      ),
      drupal_render($form['blacklist'][$id]['actions']),
    );
  }

  // This table is never empty due to the form.
  $output  = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'mollom-blacklist')));
  $output .= drupal_render_children($form);

  drupal_add_js(drupal_get_path('module', 'mollom') . '/mollom.admin.js');

  return $output;
}

/**
 * Form builder; Builds the confirmation form for deleting a blacklist item.
 *
 * @param $key
 *   The blacklist entry text to remove, base64-encoded.
 * @param $context
 *   The context of the blacklist entry.
 * @param $reason
 *   The reason of the blacklist entry.
 *
 * @ingroup forms
 * @see mollom_admin_blacklist_delete_submit()
 */
function mollom_admin_blacklist_delete($form, &$form_state) {
  $form['text'] = array(
    '#type' => 'value',
    '#value' => $_GET['text'],
  );
  $form['context'] = array(
    '#type' => 'value',
    '#value' => $_GET['context'],
  );
  $form['reason'] = array(
    '#type' => 'value',
    '#value' => $_GET['reason'],
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete %text from the blacklist?', array('%text' => $_GET['text'])),
    'admin/config/content/mollom/blacklist',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Form submit handler to delete an entry from the blacklist.
 */
function mollom_admin_blacklist_delete_submit($form, &$form_state) {
  $data = array(
    'text' => $form_state['values']['text'],
    'context' => $form_state['values']['context'],
    'reason' => $form_state['values']['reason'],
  );
  $result = mollom('mollom.removeBlacklistText', $data);

  $args = array(
    '@text' => $data['text'],
    '@context' => $data['context'],
    '@reason' => $data['reason'],
  );
  if ($result === TRUE) {
    drupal_set_message(t('The entry was removed from the blacklist.'));
    _mollom_watchdog(array(
      'Removed @text (@context) from @reason blacklist.' => $args,
      'Data:<pre>@data</pre>' => array('@data' => $data),
      'Result:<pre>@result</pre>' => array('@result' => $result),
    ));
  }
  else {
    drupal_set_message(t('An error occurred upon trying to remove the item from the blacklist.'), 'error');
    _mollom_watchdog(array(
      'Failed to removed @text (%context) from @reason blacklist.' => $args,
      'Data:<pre>@data</pre>' => array('@data' => $data),
      'Result:<pre>@result</pre>' => array('@result' => $result),
    ), WATCHDOG_ERROR);
  }

  $form_state['redirect'] = 'admin/config/content/mollom/blacklist';
}

/**
 * Form builder; Global Mollom settings form.
 *
 * This form does not validate Mollom API keys, since the fallback method still
 * needs to be able to be reconfigured in case Mollom services are down.
 * mollom.verifyKey would invalidate the keys and throw an error; hence,
 * _mollom_fallback() would invoke form_set_error(), effectively preventing this
 * form from submitting.
 */
function mollom_admin_settings($form, &$form_state) {
  // Output a positive status message, since users keep on asking whether
  // Mollom should work or not. Re-check on every regular visit of this form to
  // verify the module's configuration.
  if (empty($form_state['input'])) {
    $status = _mollom_status();
    // If there is any configuration error, then mollom_init() will have output
    // it already.
    if ($status === TRUE) {
      drupal_set_message(t('Mollom servers verified your keys. The services are operating correctly.'));
    }
  }

  $form['access-keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access keys'),
    '#description' => t('To use Mollom, you need a public and private key. To obtain your keys, <a href="@mollom-login-url">register and login on mollom.com</a>, and <a href="@mollom-manager-add-url">create a subscription</a> for your site. Once you created a subscription, copy your private and public access keys from the <a href="@mollom-manager-url">site manager</a> into the form fields below, and you are ready to go.', array(
      '@mollom-login-url' => 'http://mollom.com/user',
      '@mollom-manager-add-url' => 'http://mollom.com/site-manager/add',
      '@mollom-manager-url' => 'http://mollom.com/site-manager',
    )),
    '#collapsible' => TRUE,
    // Only show key configuration fields if they are not configured or invalid.
    '#collapsed' => !isset($status) ? FALSE : $status === TRUE,
  );
  // Keys are not #required to allow to install this module and configure it
  // later.
  $form['access-keys']['mollom_public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public key'),
    '#default_value' => variable_get('mollom_public_key', ''),
    '#description' => t('Used to uniquely identify you.'),
  );
  $form['access-keys']['mollom_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key'),
    '#default_value' => variable_get('mollom_private_key', ''),
    '#description' => t('Used to prevent someone else from hijacking your requests. Similar to a password, it should never be shared with anyone.'),
  );

  $form['mollom_fallback'] = array(
    '#type' => 'radios',
    '#title' => t('When Mollom is down or unreachable'),
    // Default to treating everything as inappropriate.
    '#default_value' => variable_get('mollom_fallback', MOLLOM_FALLBACK_BLOCK),
    '#options' => array(
      MOLLOM_FALLBACK_BLOCK => t('Block all form submissions'),
      MOLLOM_FALLBACK_ACCEPT => t('Accept all form submissions'),
    ),
    '#description' => t('In case the Mollom services are unreachable, no text analysis can be performed and no CAPTCHAs can be generated. Subscribers to <a href="@pricing-url">Mollom Plus</a> receive access to <a href="@sla-url">Mollom\'s high-availability backend infrastructure</a>, not available to free users, reducing potential downtime.', array(
      '@pricing-url' => 'http://mollom.com/pricing',
      '@sla-url' => 'http://mollom.com/standard-service-level-agreement',
    )),
  );

  $form['mollom_privacy_link'] = array(
    '#type' => 'checkbox',
    '#title' => t("Link to Mollom's privacy policy on forms protected by textual analysis"),
    '#return_value' => 1,
    '#default_value' => variable_get('mollom_privacy_link', 1),
    '#description' => t('Displays a link to the recommended <a href="@privacy-policy-url">privacy policy on mollom.com</a> on all forms that are protected via <a href="@help-url">textual analysis</a>. When disabling this option, you are required to inform visitors about data privacy through other means, as stated in the <a href="@terms-of-service-url">terms of service</a>.', array(
      '@privacy-policy-url' => 'http://mollom.com/web-service-privacy-policy',
      '@help-url' => url('admin/help/mollom'),
      '@terms-of-service-url' => 'http://mollom.com/terms-of-service',
    )),
  );

  $form['mollom_testing_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable testing mode'),
    '#default_value' => variable_get('mollom_testing_mode', 0),
    '#description' => t('Submitting "ham", "unsure", or "spam" on a protected form will trigger the corresponding behavior, and similarly, word verifications will only respond to "correct" and "incorrect", instead of the actual characters asked for. This option should be disabled in production environments.'),
  );

  return system_settings_form($form);
}

/**
 * Menu callback; Displays the administrative reports page.
 */
function mollom_reports_page($form, &$form_state) {
  $embed_attributes = array(
    'src' => 'http://mollom.com/statistics.swf?key=' . check_plain(variable_get('mollom_public_key', '')),
    'quality' => 'high',
    'width' => '100%',
    'height' => '430',
    'name' => 'Mollom',
    'align' => 'middle',
    'play' => 'true',
    'loop' => 'false',
    'allowScriptAccess' => 'sameDomain',
    'type' => 'application/x-shockwave-flash',
    'pluginspage' => 'http://www.adobe.com/go/getflashplayer',
    'wmode' => 'transparent',
  );
  $form['chart'] = array(
    '#type' => 'item',
    '#title' => t('Statistics'),
    '#markup' => '<embed' . drupal_attributes($embed_attributes) . '></embed>',
  );

  return $form;
}

